/*
 * MatrixStoryDemo.ino
 *
 * Demonstrates LedMatrixStory on the Arduino Uno Q (8×13 blue matrix).
 *
 * ═══════════════════════════════════════════════════════════════════
 * STANDALONE MODE (no Python)
 * ═══════════════════════════════════════════════════════════════════
 * Builds a story locally and plays it in a loop.
 *
 * ═══════════════════════════════════════════════════════════════════
 * BRIDGE MODE (Python web UI controls the matrix)
 * ═══════════════════════════════════════════════════════════════════
 * Python sends 104-value CSV strings via Bridge.call().
 * The sketch renders or queues them into the story.
 *
 * Python calls (all use Bridge.call / Bridge.notify from main.py):
 *
 *   matrix_load_scene(csv, index)  — load CSV frame into story slot
 *   matrix_preview(csv)            — preview a frame without storing it
 *   matrix_play(delayMs, looping)  — start animation
 *   matrix_pause("")               — pause
 *   matrix_stop("")                — stop, go to frame 0
 *   matrix_next("")                — advance one frame
 *   matrix_prev("")                — go back one frame
 *   matrix_goto(index)             — jump to frame N
 *   matrix_clear("")               — erase all scenes
 *   matrix_set_delay(ms)           — change frame delay while playing
 *   matrix_get_info("")            — returns "count,current,playing"
 *
 * ═══════════════════════════════════════════════════════════════════
 * REQUIRED LIBRARIES (add via App Lab Sketch Library Manager)
 * ═══════════════════════════════════════════════════════════════════
 *   Arduino_LED_Matrix
 *   Arduino_RouterBridge  (included by default in Zephyr core ≥ 0.55.0)
 */

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>
#include "LedMatrixStory.h"

Arduino_LED_Matrix hw;
LedMatrixStory     story(hw);

// ─── Built-in demo scenes ─────────────────────────────────────────────────────

static LedMatrixScene makeAllOn(uint8_t brightness = 1) {
    LedMatrixScene s;
    s.fill(brightness);
    snprintf(s.name, sizeof(s.name), "all_%d", brightness);
    return s;
}

static LedMatrixScene makeChecker(uint8_t a = 0, uint8_t b = 1) {
    LedMatrixScene s;
    for (uint8_t r = 0; r < MATRIX_ROWS; ++r)
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            s.setPixel(r, c, ((r + c) % 2 == 0) ? a : b);
    strlcpy(s.name, "checker", sizeof(s.name));
    return s;
}

static LedMatrixScene makeBorder(uint8_t brightness = 1) {
    LedMatrixScene s;
    s.setRow(0,              brightness);
    s.setRow(MATRIX_ROWS-1, brightness);
    s.setCol(0,              brightness);
    s.setCol(MATRIX_COLS-1, brightness);
    strlcpy(s.name, "border", sizeof(s.name));
    return s;
}

static LedMatrixScene makeDiagonalSweep(uint8_t diagIndex, uint8_t brightness = 1) {
    LedMatrixScene s;
    for (uint8_t r = 0; r < MATRIX_ROWS; ++r)
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            if ((r + c) % MATRIX_ROWS == diagIndex % MATRIX_ROWS)
                s.setPixel(r, c, brightness);
    return s;
}

// ─── Bridge callbacks (Python → Arduino) ─────────────────────────────────────
// ALL use provide_safe — they call matrix.draw() which must run in loop context.
// NEVER call Bridge.call() or Monitor.print() inside these functions.

bool matrix_load_scene(const char* csv, int index) {
    LedMatrixScene s = LedMatrixScene::fromString(csv);
    snprintf(s.name, sizeof(s.name), "scene_%d", index);
    bool ok = story.setScene(static_cast<uint8_t>(index), s);
    return ok;
}

bool matrix_preview(const char* csv) {
    LedMatrixScene s = LedMatrixScene::fromString(csv);
    story.previewScene(s);
    return true;
}

bool matrix_play(int delayMs, bool looping) {
    story.play(static_cast<unsigned long>(delayMs), looping);
    return true;
}

bool matrix_pause(const char*) {
    story.pause();
    return true;
}

bool matrix_stop(const char*) {
    story.stop();
    return true;
}

bool matrix_next(const char*) {
    story.nextFrame();
    return true;
}

bool matrix_prev(const char*) {
    story.prevFrame();
    return true;
}

bool matrix_goto(int index) {
    story.gotoFrame(static_cast<uint8_t>(index));
    return true;
}

bool matrix_clear(const char*) {
    story.clearAll();
    story.blank();
    return true;
}

bool matrix_set_delay(int ms) {
    story.setFrameDelay(static_cast<unsigned long>(ms));
    return true;
}

// Returns "sceneCount,currentFrame,isPlaying" — small string, safe for Bridge
String matrix_get_info(const char*) {
    String info = String(story.sceneCount()) + "," +
                  String(story.currentFrame()) + "," +
                  String(story.isPlaying() ? 1 : 0);
    return info;
}

// ─── Setup ───────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    Bridge.begin();

    hw.begin();
    hw.setGrayscaleBits(3);   // 8 brightness levels (0-7)

    // ── Build a demo story ──────────────────────────────────────────────────
    story.addScene(makeBorder(7));
    story.addScene(makeChecker(0, 5));
    story.addScene(makeAllOn(3));
    for (uint8_t d = 0; d < MATRIX_ROWS; ++d)
        story.addScene(makeDiagonalSweep(d, 7));
    story.addScene(makeAllOn(0));

    story.play(150);           // 150 ms per frame, loops

    // ── Register Bridge callbacks ────────────────────────────────────────────
    Bridge.provide_safe("matrix_load_scene", matrix_load_scene);
    Bridge.provide_safe("matrix_preview",    matrix_preview);
    Bridge.provide_safe("matrix_play",       matrix_play);
    Bridge.provide_safe("matrix_pause",      matrix_pause);
    Bridge.provide_safe("matrix_stop",       matrix_stop);
    Bridge.provide_safe("matrix_next",       matrix_next);
    Bridge.provide_safe("matrix_prev",       matrix_prev);
    Bridge.provide_safe("matrix_goto",       matrix_goto);
    Bridge.provide_safe("matrix_clear",      matrix_clear);
    Bridge.provide_safe("matrix_set_delay",  matrix_set_delay);
    Bridge.provide_safe("matrix_get_info",   matrix_get_info);

    Serial.println("[matrix] ready — " + String(story.sceneCount()) + " scenes");
}

// ─── Loop ────────────────────────────────────────────────────────────────────

void loop() {
    story.update();   // drives timed playback
}
