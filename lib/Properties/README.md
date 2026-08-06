# SimpleProperties


<!-- HEALTH_BADGES_START -->
[![Health: Unsure](https://img.shields.io/badge/Health-Unsure-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Unmanaged](https://img.shields.io/badge/Testing-Unmanaged-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

[![SimpleProperties](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SimpleProperties.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SimpleProperties)

A C++ library for managing Java-style key/value properties on Arduino.  
Supports **SD cards**, **LittleFS** (ESP32 / ESP8266), and any other `fs::FS`-compatible filesystem.  
Backed by the `Hashtable` library and serialises to eight file formats.

---

## What's new in 1.2.1
- Fully rewrote the example sketch with detailed, section-by-section coverage of every API.
- Expanded README to include a complete API reference, all-format code examples, and corrected LittleFS patterns.
- Documented `LittleFSProperties` shim, `setBypassSDBegin`, `setFilesystem`, and the `PropertiesIterator`.
- Bumped version to 1.2.1.

## What's new in 1.2.0
- Added broader filesystem support for SD, LittleFS, and injected `fs::FS` backends.
- Improved YAML, JSON, TOML, INI, and CSV parsing/serialisation behaviour.
- Fixed issues around trailing commas, blank/comment line handling, and file-loading edge cases.
- Refined API documentation and compatibility with the latest Hashtable and SimpleVector.

---

## Table of Contents
1. [Installation](#installation)
2. [Choosing a Backend](#choosing-a-backend)
3. [Quick-Start Examples](#quick-start-examples)
4. [API Reference](#api-reference)
5. [Supported File Formats](#supported-file-formats)
6. [Key/Value Separators (IDENTIFIERTYPE)](#keyvalue-separators-identifiertype)
7. [LittleFSProperties Shim](#littlefspropertiesshim)
8. [ChangeLog](#changelog)

---

## Installation

**PlatformIO** – add to `platformio.ini`:
```ini
lib_deps =
    braydenanderson2014/SimpleProperties
    braydenanderson2014/Hashtable
    braydenanderson2014/SimpleVector
```

**Arduino IDE** – install via the Library Manager, or clone manually:
```bash
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries.git"
```
Then copy `lib/Properties`, `lib/Hashtable`, and `lib/SimpleVector` into your Arduino `libraries/` folder.

---

## Choosing a Backend

`Properties.h` supports three backends. All file I/O (save, load, store, delete) is transparently routed through whichever backend is active.

| Backend | How to activate | SD.begin() called? |
|---|---|---|
| **SD card** (default) | `props.begin(csPin)` | Yes (automatic) |
| **LittleFS / SPIFFS / SD_MMC** | `props.begin(LittleFS)` or constructor injection | No |
| **Bypass SD auto-init** | `props.setBypassSDBegin(true)` | Never (caller manages it) |

> **Note:** `LittleFSProperties.h` is a thin backwards-compatible shim around `Properties.h`.
> New code should use `Properties.h` directly.

---

## Quick-Start Examples

### SD card (default)
```cpp
#include <Properties.h>

Properties props;

void setup() {
    Serial.begin(115200);

    // begin(csPin) calls SD.begin(csPin) internally.
    if (!props.begin(4)) {
        Serial.println("SD init failed");
        return;
    }

    props.setProperty("app.name", "MyApp");
    props.setProperty("version",  "1.0.0");

    // Save to key=value file
    props.save("/config.properties");

    // Load back
    props.clear();
    props.load("/config.properties");
    Serial.println(props.getProperty("app.name")); // MyApp
}

void loop() {}
```

### LittleFS (ESP32 / ESP8266)
```cpp
#include <Properties.h>
#include <LittleFS.h>

Properties props;

void setup() {
    Serial.begin(115200);

    // Mount LittleFS first; pass true to format on first use.
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS mount failed");
        return;
    }

    // Pass the mounted filesystem to begin().
    props.begin(LittleFS);

    props.setProperty("device.id", "ESP32-001");
    props.save("/device.properties");

    props.clear();
    props.load("/device.properties");
    Serial.println(props.getProperty("device.id")); // ESP32-001
}

void loop() {}
```

### Inject a filesystem at construction time
```cpp
#include <Properties.h>
#include <LittleFS.h>

// Pass the filesystem to the constructor; no begin() call required.
Properties props(LittleFS);

void setup() {
    LittleFS.begin(true);
    props.setProperty("key", "value");
    props.save("/data.properties");
}

void loop() {}
```

### Switch filesystem at any time
```cpp
props.setFilesystem(LittleFS);   // all subsequent operations use LittleFS
props.setFilesystem(SD);         // switch back to SD (as a fs::FS reference)
```

### Bypass automatic SD.begin()
```cpp
// When your sketch already calls SD.begin(), prevent the library from
// calling it again to avoid re-initialisation conflicts.
SD.begin(4);

Properties props;
props.setChipSelect(4);
props.setBypassSDBegin(true);
props.save("/config.properties");
```

### store() – save with a comment header
```cpp
// Writes a millis() timestamp and your comment before the key=value pairs.
props.store("/config.properties", "Application configuration");
```
Output file:
```
#12345
# Application configuration
app.name=MyApp
version=1.0.0
```

### PropertiesIterator – iterate all pairs
```cpp
for (Properties::PropertiesIterator it = props.begin(); it != props.end(); ++it) {
    Serial.print(it.key());
    Serial.print(" = ");
    Serial.println(it.value());
}
```

---

## API Reference

### Constructors

| Signature | Description |
|---|---|
| `Properties()` | Default constructor – uses SD card backend. |
| `Properties(fs::FS& filesystem)` | Injects a filesystem at construction. No `SD.begin()` is ever called. |

### Initialisation

| Method | Description |
|---|---|
| `bool begin(size_t cs, IDENTIFIERTYPE = EQUALS)` | Use SD card with the given CS pin. Calls `SD.begin(cs)` unless bypassed. |
| `bool begin(fs::FS& filesystem, IDENTIFIERTYPE = EQUALS)` | Use an injected filesystem (LittleFS, SPIFFS, SD_MMC, …). Never calls `SD.begin()`. |
| `void setChipSelect(size_t cs)` | Change the CS pin used for internal `SD.begin()` calls. |
| `size_t getChipSelect()` | Returns the current CS pin. |
| `void setBypassSDBegin(bool bypass)` | When `true`, skip internal `SD.begin()` calls (caller manages SD init). |
| `bool getBypassSDBegin()` | Returns the current bypass flag. |
| `void setFilesystem(fs::FS& filesystem)` | Swap the backing filesystem without recreating the object. |
| `void identify(IDENTIFIERTYPE)` | Change the key/value separator used for `.properties` files. |

### In-Memory Operations

| Method | Description |
|---|---|
| `void setProperty(const String& key, const String& value)` | Store a key/value pair in memory. |
| `void setProperty(const String& key, const String& value, const String& filePath)` | Load `filePath`, set the pair, save back. |
| `String getProperty(const String& key)` | Return the value, or an error string if not found. |
| `String getProperty(const String& key, const String& defaultValue, const String& filePath)` | Load `filePath`, return value or `defaultValue`. |
| `void removeProperty(const String& key)` | Remove a key from memory. |
| `void clear()` | Remove all keys from memory (does **not** touch files). |
| `int size()` | Capacity of the internal hash table. |
| `int elements()` | Number of key/value pairs currently stored. |
| `bool isEmpty()` | Returns `true` when no pairs are stored. |
| `bool exists(const String& key)` | Returns `true` if the key is present. |
| `bool exists(const String& key, const String& value)` | Returns `true` if the key exists with that exact value. |
| `bool containsKey(const String& key)` | Alias for `exists(key)`. |

### File Operations

| Method | Description |
|---|---|
| `bool save(const String& filename)` | Save in key=value format (alias for `saveToSD`). |
| `bool saveToSD(const String& filename)` | Save in key=value format. |
| `bool load(const String& filename)` | Load from key=value format (alias for `loadFromSD`). |
| `bool loadFromSD(const String& filename)` | Load from key=value format. Comment lines (`#`) and blank lines are skipped. |
| `bool store(const String& filename, const String& comments)` | Save with a `millis()` timestamp comment header. |
| `bool deleteFile(const String& filename)` | Delete a file from the active filesystem. |
| `bool storeToJSON(const String& filename, const String& comments)` | Save in JSON format. |
| `bool loadFromJSON(const String& filename)` | Load from JSON format. |
| `bool storeToXML(const String& filename, const String& comments)` | Save in XML format. |
| `bool loadFromXML(const String& filename)` | Load from XML format. |
| `bool storeToYAML(const String& filename, const String& comments)` | Save in YAML format. |
| `bool loadFromYAML(const String& filename)` | Load from YAML format. |
| `bool storeToINI(const String& filename, const String& comments)` | Save in INI format. |
| `bool loadFromINI(const String& filename)` | Load from INI format. |
| `bool storeToCSV(const String& filename, const String& comments)` | Save in CSV format. |
| `bool loadFromCSV(const String& filename)` | Load from CSV format. |
| `bool storeToTOML(const String& filename, const String& comments)` | Save in TOML format. |
| `bool loadFromTOML(const String& filename)` | Load from TOML format. |
| `bool storeToMsgPack(const String& filename, const String& comments)` | Save in binary MessagePack format. |
| `bool loadFromMsgPack(const String& filename)` | Load from binary MessagePack format. |

> **Path normalisation:** All file paths are automatically prefixed with `/` if they do not already start with one, ensuring compatibility with LittleFS and other `fs::FS` implementations that require absolute paths.

### Iterator

```cpp
// Forward-only iteration over all in-memory key/value pairs:
for (Properties::PropertiesIterator it = props.begin(); it != props.end(); ++it) {
    String k = it.key();
    String v = it.value();
}
```

---

## Supported File Formats

| Format | Store method | Load method | File extension |
|---|---|---|---|
| Properties (key=value) | `save()` / `saveToSD()` / `store()` | `load()` / `loadFromSD()` | `.properties`, `.txt`, or any |
| JSON | `storeToJSON()` | `loadFromJSON()` | `.json` |
| XML | `storeToXML()` | `loadFromXML()` | `.xml` |
| YAML | `storeToYAML()` | `loadFromYAML()` | `.yaml` / `.yml` |
| INI | `storeToINI()` | `loadFromINI()` | `.ini` |
| CSV | `storeToCSV()` | `loadFromCSV()` | `.csv` |
| TOML | `storeToTOML()` | `loadFromTOML()` | `.toml` |
| MessagePack | `storeToMsgPack()` | `loadFromMsgPack()` | `.msgpack` |

### Format examples

#### JSON
```json
{
  "app.name": "MyApp",
  "version": "1.0.0"
}
```
```cpp
props.storeToJSON("/config.json", "App config");
props.loadFromJSON("/config.json");
```

#### XML
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- App config -->
<properties>
  <property>
    <key>app.name</key>
    <value>MyApp</value>
  </property>
</properties>
```
```cpp
props.storeToXML("/config.xml", "App config");
props.loadFromXML("/config.xml");
```

#### YAML
```yaml
# App config
app.name: MyApp
version: 1.0.0
```
```cpp
props.storeToYAML("/config.yaml", "App config");
props.loadFromYAML("/config.yaml");
```

#### INI
```ini
# App config
[properties]
app.name=MyApp
version=1.0.0
```
```cpp
props.storeToINI("/config.ini", "App config");
props.loadFromINI("/config.ini");
```

#### CSV
```csv
key,value
app.name,MyApp
version,1.0.0
```
```cpp
props.storeToCSV("/config.csv", "App config");
props.loadFromCSV("/config.csv");
```

#### TOML
```toml
# App config
app.name = "MyApp"
version = "1.0.0"
```
```cpp
props.storeToTOML("/config.toml", "App config");
props.loadFromTOML("/config.toml");
```

#### MessagePack
Binary format — no human-readable representation.
```cpp
props.storeToMsgPack("/config.msgpack", "");
props.loadFromMsgPack("/config.msgpack");
```

---

## Key/Value Separators (IDENTIFIERTYPE)

The default properties format uses `=` as the separator. You can change it at initialisation or at any time with `identify()`.

| Enum value | Character | Example line |
|---|---|---|
| `EQUALS` (default) | `=` | `key=value` |
| `COLEN` | `:` | `key:value` |
| `SEMICOLEN` | `;` | `key;value` |
| `HYPHEN` | `-` | `key-value` |
| `COMMA` | `,` | `key,value` |
| `FORWARD_SLASH` | `/` | `key/value` |
| `BACKWARD_SLASH` | `\` | `key\value` |

```cpp
// Set at begin():
props.begin(4, Properties::COLEN);

// Change later:
props.identify(Properties::SEMICOLEN);
```

> **Important:** The same separator must be used when saving and loading.
> Mixing separators produces incorrect key/value splits.

---

## LittleFSProperties Shim

`LittleFSProperties.h` is a thin subclass kept for backwards compatibility with sketches written before `Properties.h` gained direct `fs::FS` support.

```cpp
#include <LittleFSProperties.h>

LittleFSProperties props;

void setup() {
    // Mount LittleFS and configure Properties in one call.
    if (!props.beginLFS()) {
        Serial.println("LittleFS init failed");
        return;
    }
    props.setProperty("key", "value");
    props.save("/data.properties");
}
```

| Method | Description |
|---|---|
| `bool beginLFS(IDENTIFIERTYPE = EQUALS)` | Calls `LittleFS.begin()` and configures the LittleFS backend. Returns `false` if the platform does not support LittleFS or mounting fails. |
| `bool beginSD(size_t cs = 4, IDENTIFIERTYPE = EQUALS)` | Delegates to `Properties::begin(cs, identifierType)` – uses the SD card backend. |

> All other `Properties` methods are inherited and work identically.

**Platform support for `beginLFS()`:**

| Platform | Support |
|---|---|
| ESP8266 | ✅ (via `LittleFSWrapper.h`) |
| ESP32, ESP32-S2, ESP32-C3 | ✅ (via `<LittleFS.h>`) |
| All others | ❌ `beginLFS()` returns `false`; use `Properties::begin(LittleFS)` directly if your core provides LittleFS. |

---

## ChangeLog

### Version 1.2.1: [CURRENT]
* Fully rewrote the example sketch (`examples/example.cpp`) with highly detailed, section-by-section coverage of every API feature including SD, LittleFS, all eight file formats, custom separators, iterator, existence checks, file-path overloads, and bypass SD.begin().
* Overhauled README: complete API reference table, all-format code examples with expected file output, corrected LittleFS patterns, LittleFSProperties shim documentation, and separator reference table.
* Documented `setBypassSDBegin` / `getBypassSDBegin`, `setFilesystem`, `setChipSelect` / `getChipSelect`, and the `PropertiesIterator` in detail.
* Fixed README installation instructions (removed broken git-clone paths; corrected library header list).
* Fixed README LittleFS example to show correct `LittleFS.begin(true)` usage with the `formatOnFail` argument.

### Version 1.2.0:
* Added broader filesystem support for SD, LittleFS, and injected `fs::FS` backends.
* Improved YAML, JSON, TOML, INI, and CSV parsing/serialisation behaviour.
* Fixed issues around trailing commas, blank/comment line handling, and file-loading edge cases.
* Refined the Properties API documentation and compatibility with the latest Hashtable and SimpleVector behaviour.

### Version 1.1.2:
* Added Support for custom chipSelect pin. You can now `setChipSelect` and `getChipSelect` which allows you to customize which pin gets used as the ChipSelect pin on an SD card reader.
* This Version moves the library back out of Beta status. Though the library may still have issues — if you spot any bugs, please file an issue report.

### Version 1.1.2_beta:
* EMERGENCY PATCH: Fixed a redefinition compile error.

### Version 1.1.1:
* Added Support for MsgPack (`storeToMsgPack` / `loadFromMsgPack`).
* Added `exists(key)` function.
* Added overload `exists(key, value)`.

### Version 1.1.0:
* Added Support for Several New File types Including `.csv`, `.json`, `.xml`, `.toml`, `.ini`, `.yaml`.
* Added Support for creating custom Key-Value Pair files via `IDENTIFIERTYPE` enum: `EQUALS`, `COLEN`, `SEMICOLEN`, `HYPHEN`, `COMMA`, `FORWARD_SLASH`, `BACKWARD_SLASH`.

### Version 1.0.9:
* Updated `saveToSD()` – now returns a boolean.
* Updated `loadFromSD()` – now returns a boolean.
* Added `save()` – alias for `saveToSD()`.
* Added `load()` – alias for `loadFromSD()`.
* Added `store()` – like `saveToSD()` but prepends a comment header.
* Added function documentation to all methods.

### Version 1.0.8:
* Removed debug `Serial.print()` statements to keep library footprint small.

### Version 1.0.7:
* Modified debug output to be opt-in via a constructor parameter.
* Debug variable is now passed through to the Hashtable constructor.

### Version 1.0.6:
* Added `[PROPERTIES]:` prefix to debug print statements.
* Added `deleteFile()` to remove a file from the SD card.
* Updated README.

### Version 1.0.5:
* Patched the iterator.
* Patched the save function — files are now created correctly.
* Added `elements()` — returns the number of stored key/value pairs.
* Adjusted `size()` — returns the capacity of the underlying hash table.

### Version 1.0.4:
* Patched iterator and save function (partially tested).
* Library considered largely stable.

### Version 1.0.3:
* Updated library JSON file.
* Updated to work with the latest Hashtable release.
* Library considered semi-stable.

### Version 1.0.2:
* Updated library JSON file.

### Version 1.0.1:
* Added `elements()` — returns the number of properties in the file.
* Updated library JSON file.

### Version 1.0.0:
* Initial Release.
