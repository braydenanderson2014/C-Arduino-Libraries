/*
 * BridgeConnectivityTest.ino
 *
 * End-to-end test of the Arduino_RouterBridge RPC link to the Python container.
 * The sketch drives the full test sequence; Python does the heavy lifting.
 *
 * ═══════════════════════════════════════════════════════════════════════
 * HOW TO USE
 * ═══════════════════════════════════════════════════════════════════════
 *
 * 1. Copy this file into your App Lab sketch/ folder.
 * 2. Make sure python/main.py and python/handlers/ are in place.
 * 3. Click Run in App Lab.
 * 4. Open the Serial Monitor tab → see [PASS]/[FAIL] per test.
 * 5. Open the Python tab → see full file contents and directory listings.
 *
 * ═══════════════════════════════════════════════════════════════════════
 * WHAT IS TESTED
 * ═══════════════════════════════════════════════════════════════════════
 *
 * File operations — the full suite:
 *   fs_write   → create a new file with arbitrary content
 *   fs_exists  → confirm file is present / absent
 *   fs_read_size + fs_read_chunk → read any file in safe chunks
 *   fs_mkdir   → create a directory (parents auto-created)
 *   fs_copy    → copy a file to a new path
 *   fs_move    → rename a file (same dir) or move it (different dir)
 *   fs_delete  → delete a file; delete a directory recursively
 *   fs_list    → walk a directory tree; summary to Serial, full tree to Python tab
 *   fs_search  → find files by name pattern from a given root
 *   fs_hello   → write timestamped helloworld.txt, return contents
 *
 * Chunked read helper:
 *   fs_read_full() — assembles fs_read_size + fs_read_chunk calls automatically
 *
 * Networking:
 *   net_ping   → TCP reachability (works in Docker without root)
 *   net_check  → HTTP 2xx check (bool only — avoids Bridge string timeout)
 *
 * ═══════════════════════════════════════════════════════════════════════
 * BRIDGE STRING TIMEOUT NOTE
 * ═══════════════════════════════════════════════════════════════════════
 *
 * Bridge.call().result(String&) can time out if:
 *   - Python takes a long time (e.g. HTTP call with slow server), OR
 *   - The returned string is very large (serialisation time exceeds RPC timeout)
 *
 * Solutions used in this sketch:
 *   - net_check instead of net_get (returns bool, not body)
 *   - fs_read_size + fs_read_chunk instead of fs_read (bounded slices)
 *   - fs_list returns a short "Nf Md" summary; full tree is printed to Python tab
 *   - fs_search returns ≤200 chars; full results printed to Python tab
 */

#include <Arduino_RouterBridge.h>

static uint8_t  passed    = 0;
static uint8_t  failed    = 0;
static bool     testsDone = false;

static void check(const char* label, bool condition) {
    if (condition) { Monitor.print("[PASS] "); ++passed; }
    else           { Monitor.print("[FAIL] "); ++failed; }
    Monitor.println(label);
}

// ─── fs_read_full ─────────────────────────────────────────────────────────────
// Reads a file of any size by fetching it in chunks.
// Uses fs_read_size first to know how many bytes to expect, then
// calls fs_read_chunk repeatedly until the whole file is assembled.
// chunkSize ≤ 200 is recommended to stay well within the Bridge timeout.
static String fs_read_full(const String& path, int chunkSize = 128) {
    int fileSize = -1;
    if (!Bridge.call("fs_read_size", path).result(fileSize) || fileSize <= 0) {
        return "";
    }
    String content;
    content.reserve(static_cast<unsigned int>(fileSize) + 1);
    int offset = 0;
    while (offset < fileSize) {
        String chunk;
        if (!Bridge.call("fs_read_chunk", path, offset, chunkSize).result(chunk)) break;
        if (chunk.length() == 0) break;
        content += chunk;
        offset  += static_cast<int>(chunk.length());
    }
    return content;
}

void setup() {
    Monitor.begin(115200);
    Bridge.begin();
    // Bridge.call() in setup() always fails — the TCP link to the container
    // hasn't finished establishing yet. Tests run from loop() after a warm-up.
    Monitor.println("[UnoQ] waiting for bridge...");
}

static void runTests() {
    Monitor.println("=== Bridge Connectivity Test ===");

    // ─── Create files ─────────────────────────────────────────────────────────

    Monitor.println("--- Create ---");

    // fs_write: create any file with arbitrary content.
    // Intermediate directories are created automatically.
    // Calling fs_write on an existing path overwrites it.
    bool ok, wrote = false;
    ok = Bridge.call("fs_write",
                     String("notes/note_a.txt"),
                     String("This is note A")).result(wrote);
    check("fs_write notes/note_a.txt",         ok && wrote);

    ok = Bridge.call("fs_write",
                     String("notes/note_b.txt"),
                     String("This is note B")).result(wrote);
    check("fs_write notes/note_b.txt",         ok && wrote);

    // Overwrite — same path, different content.
    ok = Bridge.call("fs_write",
                     String("notes/note_a.txt"),
                     String("Note A — updated")).result(wrote);
    check("fs_write overwrite ok",             ok && wrote);

    // fs_exists: confirm both files are present.
    bool exists = false;
    ok = Bridge.call("fs_exists", String("notes/note_a.txt")).result(exists);
    check("note_a.txt exists",                 ok && exists);
    ok = Bridge.call("fs_exists", String("notes/note_b.txt")).result(exists);
    check("note_b.txt exists",                 ok && exists);

    // ─── Chunked Read ────────────────────────────────────────────────────────

    Monitor.println("--- Read ---");

    // fs_read_size: get character count as an int — always fast, no timeout risk.
    int sz = -1;
    ok = Bridge.call("fs_read_size", String("notes/note_a.txt")).result(sz);
    check("fs_read_size > 0",                  ok && sz > 0);
    Monitor.print("  size: "); Monitor.println(sz);

    // fs_read_full assembles 128-char chunks until the whole file is in memory.
    // Each individual Bridge.call stays well within the RPC timeout.
    String content = fs_read_full("notes/note_a.txt");
    check("fs_read_full content correct",      content == "Note A \xe2\x80\x94 updated");
    Monitor.print("  content: "); Monitor.println(content);

    // ─── Directory operations ─────────────────────────────────────────────────

    Monitor.println("--- Directory ---");

    // fs_mkdir: create a folder; parent dirs are made automatically.
    bool madeDir = false;
    ok = Bridge.call("fs_mkdir", String("archive/2026")).result(madeDir);
    check("fs_mkdir archive/2026/",            ok && madeDir);

    // ─── Copy ────────────────────────────────────────────────────────────────

    Monitor.println("--- Copy ---");

    // fs_copy: copy a file to a new path (overwrites destination if it exists).
    // Use this to duplicate a file without removing the original.
    bool copied = false;
    ok = Bridge.call("fs_copy",
                     String("notes/note_a.txt"),
                     String("archive/2026/note_a_backup.txt")).result(copied);
    check("fs_copy note_a -> archive/",        ok && copied);

    // Both the original and the copy should now exist.
    ok = Bridge.call("fs_exists", String("notes/note_a.txt")).result(exists);
    check("original still exists after copy",  ok && exists);
    ok = Bridge.call("fs_exists",
                     String("archive/2026/note_a_backup.txt")).result(exists);
    check("copy exists in archive/",           ok && exists);

    // ─── Rename / Move ───────────────────────────────────────────────────────

    Monitor.println("--- Rename / Move ---");

    // fs_move used as rename: same directory, different name.
    bool moved = false;
    ok = Bridge.call("fs_move",
                     String("notes/note_b.txt"),
                     String("notes/note_b_renamed.txt")).result(moved);
    check("fs_move rename in-place",           ok && moved);
    ok = Bridge.call("fs_exists", String("notes/note_b.txt")).result(exists);
    check("old name gone after rename",        ok && !exists);
    ok = Bridge.call("fs_exists", String("notes/note_b_renamed.txt")).result(exists);
    check("new name present after rename",     ok && exists);

    // fs_move used as move: different directory.
    ok = Bridge.call("fs_move",
                     String("notes/note_b_renamed.txt"),
                     String("archive/2026/note_b.txt")).result(moved);
    check("fs_move to archive/",               ok && moved);
    ok = Bridge.call("fs_exists", String("archive/2026/note_b.txt")).result(exists);
    check("note_b in archive/ after move",     ok && exists);

    // ─── List & Search ───────────────────────────────────────────────────────

    Monitor.println("--- List & Search ---");

    // fs_list: walk a directory tree.
    // "" or "." = filesystem root; pass a sub-path to narrow the scope.
    // Second arg: 1 = recursive, 0 = top-level only.
    // Returns "Nf Md" summary to sketch; full tree (with sizes) in Python tab.
    String summary;
    ok = Bridge.call("fs_list", String(""), (int)1).result(summary);
    check("fs_list root ok",                   ok);
    Monitor.print("  root: "); Monitor.println(summary);

    ok = Bridge.call("fs_list", String("archive"), (int)1).result(summary);
    check("fs_list archive/ ok",               ok);
    Monitor.print("  archive/: "); Monitor.println(summary);

    // fs_search: find paths containing a pattern (case-insensitive).
    // Second arg: start path ("" = root).
    // Returns ≤200 chars newline-joined; full results in Python tab.
    String matches;
    ok = Bridge.call("fs_search", String(".txt"), String("")).result(matches);
    check("fs_search .txt ok",                 ok);
    Monitor.print("  .txt: "); Monitor.println(matches);

    ok = Bridge.call("fs_search", String("backup"), String("archive")).result(matches);
    check("fs_search backup in archive/",      ok);
    Monitor.print("  backup: "); Monitor.println(matches);

    // ─── Delete ──────────────────────────────────────────────────────────────

    Monitor.println("--- Delete ---");

    // fs_delete with recursive=0: removes a single file.
    bool deleted = false;
    ok = Bridge.call("fs_delete",
                     String("notes/note_a.txt"), (int)0).result(deleted);
    check("fs_delete single file",             ok && deleted);
    ok = Bridge.call("fs_exists", String("notes/note_a.txt")).result(exists);
    check("file gone after delete",            ok && !exists);

    // fs_delete with recursive=1: removes a directory and everything inside it.
    ok = Bridge.call("fs_delete", String("archive"), (int)1).result(deleted);
    check("fs_delete archive/ recursive",      ok && deleted);
    ok = Bridge.call("fs_exists", String("archive")).result(exists);
    check("archive/ gone after recursive del", ok && !exists);

    // ─── fs_hello ────────────────────────────────────────────────────────────

    Monitor.println("--- Hello World ---");

    // fs_hello: Python writes helloworld.txt stamped with the current timestamp
    // and the message you pass, then returns the full file contents as a String.
    // Useful for a quick sanity check that both sides are alive.
    String hello;
    ok = Bridge.call("fs_hello", String("Greenhouse online")).result(hello);
    check("fs_hello succeeded",                ok && hello.length() > 0);
    Monitor.println("  (see Python tab for file contents)");

    // ─── Networking ──────────────────────────────────────────────────────────

    Monitor.println("--- Networking ---");

    bool reachable = false;
    ok = Bridge.call("net_ping", String("8.8.8.8")).result(reachable);
    check("net_ping 8.8.8.8",                 ok && reachable);

    bool httpOk = false;
    ok = Bridge.call("net_check", String("http://example.com")).result(httpOk);
    check("net_check http://example.com",     ok && httpOk);

    // ─── Summary ─────────────────────────────────────────────────────────────

    Monitor.println("================================");
    Monitor.print(passed); Monitor.print(" passed, ");
    Monitor.print(failed); Monitor.println(" failed");
    Monitor.println(failed == 0 ? "=== ALL TESTS PASSED ===" : "=== SOME TESTS FAILED ===");
}

void loop() {
    if (!testsDone) {
        // 5 s warm-up gives the Bridge TCP connection time to establish.
        // Increase to 8000 if tests still show ok=0 on first call.
        delay(5000);
        Monitor.println("[UnoQ] bridge ready — running tests");
        runTests();
        testsDone = true;
        return;
    }

    // Periodic smoke-test — keeps the Python tab active so you can see heartbeats.
    delay(15000);
    bool recorded = false;
    bool ok = Bridge.call("tm_record", String("sketch"), String("loop ping")).result(recorded);
    Monitor.print("[loop] ping ");
    Monitor.println((ok && recorded) ? "ok" : "FAIL");
}


#include <Arduino_RouterBridge.h>

static uint8_t  passed    = 0;
static uint8_t  failed    = 0;
static bool     testsDone = false;

static void check(const char* label, bool condition) {
    if (condition) { Monitor.print("[PASS] "); ++passed; }
    else           { Monitor.print("[FAIL] "); ++failed; }
    Monitor.println(label);
}

// Read any file in safe chunks regardless of size.
// Returns full content or "" on error. chunkSize <= 200 is recommended.
static String fs_read_full(const String& path, int chunkSize = 128) {
    int fileSize = -1;
    if (!Bridge.call("fs_read_size", path).result(fileSize) || fileSize <= 0) {
        return "";
    }
    String content;
    content.reserve(static_cast<unsigned int>(fileSize) + 1);
    int offset = 0;
    while (offset < fileSize) {
        String chunk;
        if (!Bridge.call("fs_read_chunk", path, offset, chunkSize).result(chunk)) break;
        if (chunk.length() == 0) break;
        content += chunk;
        offset  += static_cast<int>(chunk.length());
    }
    return content;
}

void setup() {
    Monitor.begin(115200);
    Bridge.begin();
    Monitor.println("[UnoQ] waiting for bridge...");
}

static void runTests() {
    Monitor.println("=== Bridge Connectivity Test ===");

    // -----------------------------------------------------------------------
    // File lifecycle: create → verify → mkdir → move → verify moved
    // -----------------------------------------------------------------------
    Monitor.println("--- File Lifecycle ---");

    String fileContents;
    bool ok = Bridge.call("fs_hello", String("Hello from Arduino Uno Q!")).result(fileContents);
    check("fs_hello call succeeded",           ok);
    check("fs_hello returned content",         fileContents.length() > 0);
    Monitor.println("  (see Python tab for file contents)");

    bool exists = false;
    ok = Bridge.call("fs_exists", String("helloworld.txt")).result(exists);
    check("helloworld.txt exists before move", ok && exists);

    bool madeDir = false;
    ok = Bridge.call("fs_mkdir", String("greenhouse_data")).result(madeDir);
    check("fs_mkdir greenhouse_data/",         ok && madeDir);

    bool moved = false;
    ok = Bridge.call("fs_move",
                     String("helloworld.txt"),
                     String("greenhouse_data/helloworld.txt")).result(moved);
    check("fs_move succeeded",                 ok && moved);

    bool stillThere = true;
    ok = Bridge.call("fs_exists", String("helloworld.txt")).result(stillThere);
    check("helloworld.txt gone from root",     ok && !stillThere);

    bool atNewPath = false;
    ok = Bridge.call("fs_exists",
                     String("greenhouse_data/helloworld.txt")).result(atNewPath);
    check("file exists at greenhouse_data/",   ok && atNewPath);

    // List entire filesystem root — full tree printed to Python tab
    String summary;
    ok = Bridge.call("fs_list", String(""), (int)1).result(summary);
    check("fs_list root ok",                   ok);
    Monitor.print("  root contents: "); Monitor.println(summary);

    // Chunked read — safe for any file size
    Monitor.println("--- Chunked Read ---");
    int fileSize = -1;
    ok = Bridge.call("fs_read_size",
                     String("greenhouse_data/helloworld.txt")).result(fileSize);
    check("fs_read_size > 0",                  ok && fileSize > 0);
    Monitor.print("  file size: "); Monitor.println(fileSize);

    String fullContent = fs_read_full("greenhouse_data/helloworld.txt");
    check("fs_read_full assembled correctly",  fullContent.length() == (unsigned)fileSize);
    Monitor.print("  assembled "); Monitor.print(fullContent.length()); Monitor.println(" chars:");
    Monitor.println(fullContent);

    // -----------------------------------------------------------------------
    // Networking: ping + HTTP GET
    // -----------------------------------------------------------------------
    Monitor.println("--- Networking ---");

    bool reachable = false;
    ok = Bridge.call("net_ping", String("8.8.8.8")).result(reachable);
    check("net_ping 8.8.8.8",                 ok && reachable);

    // net_check returns just a bool — avoids RPC timeout on large response bodies
    bool httpOk = false;
    ok = Bridge.call("net_check", String("http://example.com")).result(httpOk);
    check("net_check http://example.com",     ok && httpOk);
    Monitor.println("  (Python tab shows full response)");

    // -----------------------------------------------------------------------
    // Summary
    // -----------------------------------------------------------------------
    Monitor.println("================================");
    Monitor.print(passed); Monitor.print(" passed, ");
    Monitor.print(failed); Monitor.println(" failed");
    Monitor.println(failed == 0 ? "=== ALL TESTS PASSED ===" : "=== SOME TESTS FAILED ===");
}

void loop() {
    if (!testsDone) {
        // Wait for bridge to fully establish before first call
        delay(5000);
        Monitor.println("[UnoQ] bridge ready — running tests");
        runTests();
        testsDone = true;
        return;
    }

    delay(15000);

    bool recorded = false;
    bool ok = Bridge.call("tm_record", String("sketch"), String("loop ping")).result(recorded);
    Monitor.print("[loop] ping ");
    Monitor.println((ok && recorded) ? "ok" : "FAIL");
}



