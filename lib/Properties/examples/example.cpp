/**
 * @file example.cpp
 * @brief Comprehensive example for the SimpleProperties library.
 *
 * This example demonstrates every major feature of the Properties class:
 *   - Initializing with an SD card backend
 *   - Initializing with a LittleFS backend (ESP32 / ESP8266)
 *   - Injecting any custom fs::FS-compatible filesystem
 *   - Setting and getting properties in memory
 *   - Saving and loading the default key=value format
 *   - Using the `store()` method to embed a comment header
 *   - Saving and loading every supported format (JSON, XML, YAML, INI, CSV, TOML, MsgPack)
 *   - Using different key/value separator types (IDENTIFIERTYPE)
 *   - The setProperty / getProperty file-path overloads
 *   - Iterating over all properties with PropertiesIterator
 *   - Checking existence, removing, and clearing properties
 *   - Deleting a file from the filesystem
 *   - Bypassing the automatic SD.begin() call
 *
 * Hardware assumptions for the SD examples:
 *   - SD card reader CS pin is connected to pin 4.
 *
 * For the LittleFS examples compile for ESP32 or ESP8266.
 *
 * Dependencies (install via Library Manager or PlatformIO):
 *   - SimpleProperties  (this library)
 *   - Hashtable         (required dependency)
 *   - SimpleVector      (required dependency)
 *   - SD                (for SD card examples, already bundled with most Arduino cores)
 *   - LittleFS          (for LittleFS examples, included with ESP32 / ESP8266 cores)
 */

#include <Arduino.h>
#include <Properties.h>

// ─── Forward declarations ─────────────────────────────────────────────────────
void demoSDBackend();
void demoLittleFSBackend();
void demoAllFormats(Properties& props);
void demoCustomSeparator();
void demoFilePathOverloads();
void demoIterator(Properties& props);
void demoExistenceAndRemoval(Properties& props);
void demoBypassSDBegin();

// ─── setup() ─────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    while (!Serial) { /* wait for USB CDC on Leonardo / Micro */ }
    delay(500);

    Serial.println(F("=== SimpleProperties Comprehensive Example ==="));
    Serial.println();

    // ── 1. SD card backend demo ───────────────────────────────────────────────
    demoSDBackend();

    // ── 2. LittleFS backend demo (ESP32 / ESP8266 only) ──────────────────────
    //
    // Uncomment the line below when targeting an ESP32 or ESP8266 board.
    // demoLittleFSBackend();

    // ── 3. Custom separator demo ──────────────────────────────────────────────
    demoCustomSeparator();

    // ── 4. File-path overload demo ────────────────────────────────────────────
    demoFilePathOverloads();

    // ── 5. Bypass SD.begin() demo ─────────────────────────────────────────────
    demoBypassSDBegin();

    Serial.println(F("=== Example complete ==="));
}

void loop() {
    // Nothing to do here – all demos run once in setup().
}

// ─── SD card backend demo ─────────────────────────────────────────────────────

/**
 * @brief Shows the full lifecycle using an SD card as the backing store.
 *
 * Steps:
 *  1. Construct a Properties object and call begin() with the CS pin.
 *  2. Set several properties in memory.
 *  3. Save to and load from the default key=value (.properties) format.
 *  4. Use store() to write a timestamped comment header.
 *  5. Save and load every other supported format.
 *  6. Iterate, check existence, remove, and clear.
 *  7. Delete the test files from the SD card.
 */
void demoSDBackend() {
    Serial.println(F("--- SD Card Backend Demo ---"));

    // ── Construction ─────────────────────────────────────────────────────────
    //
    // The default constructor targets the SD card.  Call begin() to supply the
    // CS pin and (optionally) the separator type.  begin() also calls
    // SD.begin(cs) internally, so you do not need a separate SD.begin() call.
    //
    Properties props;                           // SD backend
    if (!props.begin(4)) {                      // CS pin 4; default separator '='
        Serial.println(F("[ERROR] SD card init failed – check wiring and CS pin."));
        return;
    }
    Serial.println(F("[OK] SD card initialised."));

    // ── Setting properties ────────────────────────────────────────────────────
    //
    // setProperty(key, value) stores the pair in memory only.
    // Nothing is written to the filesystem until save/store/saveToSD is called.
    //
    props.setProperty("app.name",    "MyApp");
    props.setProperty("app.version", "2.0.0");
    props.setProperty("wifi.ssid",   "HomeNetwork");
    props.setProperty("wifi.pass",   "s3cr3t");
    props.setProperty("led.pin",     "13");
    props.setProperty("timeout",     "5000");

    Serial.println(F("[OK] Properties set in memory."));
    Serial.print(F("     Count: ")); Serial.println(props.elements());
    Serial.print(F("     Capacity: ")); Serial.println(props.size());

    // ── Saving (key=value format) ─────────────────────────────────────────────
    //
    // save() and saveToSD() are identical – both write the current in-memory
    // properties to a file using the configured separator (default '=').
    //
    if (props.save("/config.properties")) {
        Serial.println(F("[OK] Saved to /config.properties"));
    }

    // ── store() – save with a comment header ──────────────────────────────────
    //
    // store() prepends a millis() timestamp line and your custom comment before
    // the key=value pairs, which is useful for auditing configuration changes.
    //
    if (props.store("/config_commented.properties", "Application configuration")) {
        Serial.println(F("[OK] Stored to /config_commented.properties with comment header."));
    }

    // ── Loading (key=value format) ────────────────────────────────────────────
    //
    // load() / loadFromSD() reads back a previously saved file.
    // Blank lines and lines starting with '#' are treated as comments and skipped.
    //
    props.clear();  // wipe memory so we can prove the load works
    if (props.load("/config.properties")) {
        Serial.println(F("[OK] Loaded from /config.properties"));
        Serial.print(F("     app.name = "));
        Serial.println(props.getProperty("app.name"));
    }

    // ── All supported formats ─────────────────────────────────────────────────
    demoAllFormats(props);

    // ── Iterator ─────────────────────────────────────────────────────────────
    demoIterator(props);

    // ── Existence check, removal, clear ──────────────────────────────────────
    demoExistenceAndRemoval(props);

    // ── getProperty with default value ────────────────────────────────────────
    //
    // If the key is not present, the supplied default is returned instead of an
    // error string.  The three-argument overload also loads the file first.
    //
    String missing = props.getProperty("no.such.key");
    Serial.print(F("     getProperty (missing key) -> "));
    Serial.println(missing);  // prints the error string

    // ── Delete files ──────────────────────────────────────────────────────────
    props.deleteFile("/config.properties");
    props.deleteFile("/config_commented.properties");
    Serial.println(F("[OK] Test files deleted."));

    Serial.println();
}

// ─── LittleFS backend demo ───────────────────────────────────────────────────

/**
 * @brief Shows how to use LittleFS instead of (or alongside) an SD card.
 *
 * LittleFS is the recommended flash filesystem for ESP32 and ESP8266 boards.
 * It is stored on the MCU's internal flash memory – no SD card required.
 *
 * Two equivalent patterns are shown:
 *   A) Use the constructor that accepts an fs::FS reference.
 *   B) Use the default constructor and call begin(LittleFS).
 *
 * The `LittleFSProperties` helper class (LittleFSProperties.h) wraps these
 * two patterns into a single beginLFS() convenience call for backwards
 * compatibility with older sketches.
 */
void demoLittleFSBackend() {
    Serial.println(F("--- LittleFS Backend Demo ---"));

#if defined(ESP32) || defined(ESP8266)
    #include <LittleFS.h>   // included here for clarity; normally placed at the top

    // ── Pattern A: inject LittleFS at construction time ───────────────────────
    //
    // Mounting LittleFS must be done by the application before calling begin().
    // Pass formatOnFail=true the first time to create the filesystem partition.
    //
    if (!LittleFS.begin(/* formatOnFail= */ true)) {
        Serial.println(F("[ERROR] LittleFS mount failed."));
        return;
    }

    Properties propsA(LittleFS);    // filesystem injected via constructor
    // No begin() call needed when the constructor takes a filesystem.
    // You can still call begin(LittleFS) to change the separator:
    propsA.begin(LittleFS, Properties::EQUALS);

    propsA.setProperty("device.id",   "ESP32-001");
    propsA.setProperty("mqtt.broker", "192.168.1.100");
    propsA.setProperty("mqtt.port",   "1883");

    if (propsA.save("/device.properties")) {
        Serial.println(F("[OK] Saved to LittleFS /device.properties (pattern A)"));
    }

    // ── Pattern B: default constructor + begin(LittleFS) ─────────────────────
    Properties propsB;
    propsB.begin(LittleFS);         // switch to LittleFS backend

    propsB.load("/device.properties");
    Serial.print(F("     device.id = "));
    Serial.println(propsB.getProperty("device.id"));

    // ── setFilesystem() – switch filesystem after construction ────────────────
    //
    // You can also swap the backing filesystem at any time without recreating
    // the Properties object.
    //
    Properties propsC;
    propsC.setFilesystem(LittleFS); // equivalent to propsC.begin(LittleFS)

    // ── Saving JSON to LittleFS ────────────────────────────────────────────────
    propsB.storeToJSON("/device.json", "Device configuration");
    Serial.println(F("[OK] Saved JSON to LittleFS /device.json"));

    // Clean up
    propsA.deleteFile("/device.properties");
    propsB.deleteFile("/device.json");
    LittleFS.end();
    Serial.println(F("[OK] LittleFS demo complete."));

#else
    Serial.println(F("[SKIP] LittleFS demo requires ESP32 or ESP8266."));
#endif

    Serial.println();
}

// ─── All formats demo ────────────────────────────────────────────────────────

/**
 * @brief Exercises every store/load format pair supported by Properties.
 *
 * Format overview:
 * ┌──────────┬──────────────────────────────────────────────────────────────────┐
 * │ Format   │ Notes                                                            │
 * ├──────────┼──────────────────────────────────────────────────────────────────┤
 * │ .props   │ Plain key=value (or custom separator).  Default format.          │
 * │ JSON     │ {"key":"value"} pairs inside a top-level object.                 │
 * │ XML      │ <properties><property><key/><value/></property></properties>     │
 * │ YAML     │ key: value lines.                                                │
 * │ INI      │ key=value inside a [properties] section.                        │
 * │ CSV      │ key,value rows with a header row.                                │
 * │ TOML     │ key = "value" pairs.                                             │
 * │ MsgPack  │ Binary MessagePack encoding of key-value pairs.                  │
 * └──────────┴──────────────────────────────────────────────────────────────────┘
 *
 * @param props  An already-initialised Properties object.
 */
void demoAllFormats(Properties& props) {
    Serial.println(F("  [Formats] Testing all store/load formats..."));

    // Seed with a small set of properties for the format round-trip tests.
    props.clear();
    props.setProperty("color",    "blue");
    props.setProperty("count",    "42");
    props.setProperty("enabled",  "true");

    // ── JSON ──────────────────────────────────────────────────────────────────
    if (props.storeToJSON("/data.json", "Format test")) {
        Serial.println(F("  [OK] storeToJSON -> /data.json"));
    }
    props.clear();
    if (props.loadFromJSON("/data.json")) {
        Serial.print(F("  [OK] loadFromJSON -> color = "));
        Serial.println(props.getProperty("color"));
    }

    // ── XML ───────────────────────────────────────────────────────────────────
    if (props.storeToXML("/data.xml", "Format test")) {
        Serial.println(F("  [OK] storeToXML -> /data.xml"));
    }
    props.clear();
    if (props.loadFromXML("/data.xml")) {
        Serial.print(F("  [OK] loadFromXML -> count = "));
        Serial.println(props.getProperty("count"));
    }

    // ── YAML ──────────────────────────────────────────────────────────────────
    if (props.storeToYAML("/data.yaml", "Format test")) {
        Serial.println(F("  [OK] storeToYAML -> /data.yaml"));
    }
    props.clear();
    if (props.loadFromYAML("/data.yaml")) {
        Serial.print(F("  [OK] loadFromYAML -> enabled = "));
        Serial.println(props.getProperty("enabled"));
    }

    // ── INI ───────────────────────────────────────────────────────────────────
    if (props.storeToINI("/data.ini", "Format test")) {
        Serial.println(F("  [OK] storeToINI -> /data.ini"));
    }
    props.clear();
    if (props.loadFromINI("/data.ini")) {
        Serial.print(F("  [OK] loadFromINI -> color = "));
        Serial.println(props.getProperty("color"));
    }

    // ── CSV ───────────────────────────────────────────────────────────────────
    if (props.storeToCSV("/data.csv", "Format test")) {
        Serial.println(F("  [OK] storeToCSV -> /data.csv"));
    }
    props.clear();
    if (props.loadFromCSV("/data.csv")) {
        Serial.print(F("  [OK] loadFromCSV -> count = "));
        Serial.println(props.getProperty("count"));
    }

    // ── TOML ──────────────────────────────────────────────────────────────────
    if (props.storeToTOML("/data.toml", "Format test")) {
        Serial.println(F("  [OK] storeToTOML -> /data.toml"));
    }
    props.clear();
    if (props.loadFromTOML("/data.toml")) {
        Serial.print(F("  [OK] loadFromTOML -> enabled = "));
        Serial.println(props.getProperty("enabled"));
    }

    // ── MsgPack ───────────────────────────────────────────────────────────────
    //
    // MsgPack produces a compact binary file that is not human-readable.
    // It is useful when storage space is limited.
    //
    if (props.storeToMsgPack("/data.msgpack", "Format test")) {
        Serial.println(F("  [OK] storeToMsgPack -> /data.msgpack"));
    }
    props.clear();
    if (props.loadFromMsgPack("/data.msgpack")) {
        Serial.print(F("  [OK] loadFromMsgPack -> color = "));
        Serial.println(props.getProperty("color"));
    }

    // Clean up format test files
    props.deleteFile("/data.json");
    props.deleteFile("/data.xml");
    props.deleteFile("/data.yaml");
    props.deleteFile("/data.ini");
    props.deleteFile("/data.csv");
    props.deleteFile("/data.toml");
    props.deleteFile("/data.msgpack");

    Serial.println(F("  [OK] All format tests complete."));
}

// ─── Custom separator demo ───────────────────────────────────────────────────

/**
 * @brief Shows how to change the key/value separator used when reading and
 *        writing the default properties format.
 *
 * Available separator types (Properties::IDENTIFIERTYPE):
 *   EQUALS        '='   (default)
 *   COLEN         ':'
 *   SEMICOLEN     ';'
 *   HYPHEN        '-'
 *   COMMA         ','
 *   FORWARD_SLASH '/'
 *   BACKWARD_SLASH'\'
 *
 * IMPORTANT: The separator must be the same when saving and loading.
 *            Mixing separators will produce incorrect key/value splits.
 */
void demoCustomSeparator() {
    Serial.println(F("--- Custom Separator Demo ---"));

    // ── Using COLEN (':') separator ───────────────────────────────────────────
    Properties colon;
    if (!colon.begin(4, Properties::COLEN)) {
        Serial.println(F("[ERROR] SD init failed."));
        return;
    }

    colon.setProperty("host", "localhost");
    colon.setProperty("port", "8080");
    colon.save("/colon.properties");       // writes: host:localhost\n port:8080\n

    // Load back – must use the same separator
    colon.clear();
    colon.load("/colon.properties");
    Serial.print(F("     host = "));
    Serial.println(colon.getProperty("host"));  // -> "localhost"

    // ── Changing separator after construction ─────────────────────────────────
    //
    // identify() updates the separator type at any time.
    //
    Properties props;
    props.begin(4);
    props.identify(Properties::SEMICOLEN);     // switch to ';' separator
    props.setProperty("key1", "value1");
    props.save("/semicolon.properties");       // writes: key1;value1\n

    // Restore default for other demos
    props.identify(Properties::EQUALS);

    colon.deleteFile("/colon.properties");
    props.deleteFile("/semicolon.properties");
    Serial.println(F("[OK] Custom separator demo complete."));
    Serial.println();
}

// ─── File-path overload demo ─────────────────────────────────────────────────

/**
 * @brief Shows the convenience overloads of setProperty() and getProperty()
 *        that accept a file path and perform a load/save automatically.
 *
 * setProperty(key, value, filePath):
 *   1. Loads filePath into memory (merging with any existing in-memory props).
 *   2. Sets the key/value pair.
 *   3. Saves all properties back to filePath.
 *
 * getProperty(key, defaultValue, filePath):
 *   1. Loads filePath into memory.
 *   2. Returns the value for key, or defaultValue if the key is absent.
 *
 * These overloads are handy for quick one-liner reads and writes, but they
 * reload/resave the whole file on every call.  For bulk operations, prefer
 * explicit load() / setProperty() / save() sequences.
 */
void demoFilePathOverloads() {
    Serial.println(F("--- File-Path Overload Demo ---"));

    Properties props;
    if (!props.begin(4)) {
        Serial.println(F("[ERROR] SD init failed."));
        return;
    }

    // Write a single key directly to a file (no explicit save() needed).
    props.setProperty("theme", "dark",  "/ui.properties");
    props.setProperty("lang",  "en-US", "/ui.properties");

    // Read a key directly from a file (no explicit load() needed).
    String theme = props.getProperty("theme", "light", "/ui.properties");
    Serial.print(F("     theme = "));
    Serial.println(theme);  // -> "dark"

    // If the key is absent, the default is returned.
    String font = props.getProperty("font", "Roboto", "/ui.properties");
    Serial.print(F("     font (default) = "));
    Serial.println(font);   // -> "Roboto"

    props.deleteFile("/ui.properties");
    Serial.println(F("[OK] File-path overload demo complete."));
    Serial.println();
}

// ─── Iterator demo ───────────────────────────────────────────────────────────

/**
 * @brief Shows how to iterate over all key/value pairs in memory.
 *
 * PropertiesIterator provides forward-only iteration using the standard
 * begin()/end()/operator++/operator!= pattern.  Use it.key() and it.value()
 * to access each pair, or dereference *it to get a KeyValuePair struct.
 *
 * @param props  An already-populated Properties object.
 */
void demoIterator(Properties& props) {
    Serial.println(F("  [Iterator] Listing all in-memory properties:"));
    int idx = 0;
    for (Properties::PropertiesIterator it = props.begin(); it != props.end(); ++it) {
        Serial.print(F("    ["));
        Serial.print(idx++);
        Serial.print(F("] "));
        Serial.print(it.key());
        Serial.print(F(" = "));
        Serial.println(it.value());
    }
    Serial.print(F("  [Iterator] Total: "));
    Serial.println(props.elements());
}

// ─── Existence check, removal, and clear demo ────────────────────────────────

/**
 * @brief Shows the various ways to check for and remove properties.
 *
 * exists(key)          – returns true if the key is present (any value).
 * exists(key, value)   – returns true only if key is present with that exact value.
 * containsKey(key)     – identical to exists(key).
 * removeProperty(key)  – removes a single key from memory.
 * clear()              – removes all keys from memory (does not touch files).
 *
 * @param props  An already-populated Properties object.
 */
void demoExistenceAndRemoval(Properties& props) {
    Serial.println(F("  [Existence] Checking and removing properties:"));

    // exists() / containsKey()
    bool hasSSID = props.exists("wifi.ssid");
    Serial.print(F("    exists('wifi.ssid')            = "));
    Serial.println(hasSSID ? F("true") : F("false"));

    bool exactMatch = props.exists("led.pin", "13");
    Serial.print(F("    exists('led.pin', '13')        = "));
    Serial.println(exactMatch ? F("true") : F("false"));

    bool wrongValue = props.exists("led.pin", "99");
    Serial.print(F("    exists('led.pin', '99')        = "));
    Serial.println(wrongValue ? F("true") : F("false"));

    bool hasKey = props.containsKey("app.name");
    Serial.print(F("    containsKey('app.name')        = "));
    Serial.println(hasKey ? F("true") : F("false"));

    // Remove a single property
    props.removeProperty("wifi.pass");
    Serial.print(F("    After removeProperty('wifi.pass'), exists = "));
    Serial.println(props.exists("wifi.pass") ? F("true") : F("false"));

    // Clear all
    props.clear();
    Serial.print(F("    After clear(), isEmpty() = "));
    Serial.println(props.isEmpty() ? F("true") : F("false"));
}

// ─── Bypass SD.begin() demo ──────────────────────────────────────────────────

/**
 * @brief Shows how to prevent the library from calling SD.begin() internally.
 *
 * By default Properties calls SD.begin(chipSelect) before every SD operation.
 * If your sketch already initialises the SD card (e.g. to share it with
 * another library), call setBypassSDBegin(true) to skip the redundant
 * SD.begin() calls and avoid potential re-initialisation conflicts.
 *
 * Alternatively, pass a pre-mounted fs::FS object (SD as a fs::FS reference)
 * to begin() or the constructor – this also bypasses the internal SD.begin().
 */
void demoBypassSDBegin() {
    Serial.println(F("--- Bypass SD.begin() Demo ---"));

    // Initialise SD card once, externally
    if (!SD.begin(4)) {
        Serial.println(F("[ERROR] SD card init failed."));
        return;
    }

    Properties props;
    props.setChipSelect(4);
    props.setBypassSDBegin(true);   // skip internal SD.begin() from now on
    props.identify(Properties::EQUALS);

    props.setProperty("bypass.test", "passed");
    if (props.save("/bypass.properties")) {
        Serial.println(F("[OK] Saved with bypassSDBegin=true."));
    }

    props.clear();
    props.load("/bypass.properties");
    Serial.print(F("     bypass.test = "));
    Serial.println(props.getProperty("bypass.test"));

    Serial.print(F("     bypassSDBegin = "));
    Serial.println(props.getBypassSDBegin() ? F("true") : F("false"));
    Serial.print(F("     chipSelect    = "));
    Serial.println(props.getChipSelect());

    props.deleteFile("/bypass.properties");
    Serial.println(F("[OK] Bypass SD.begin() demo complete."));
    Serial.println();
}
