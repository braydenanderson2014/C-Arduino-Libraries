# Dynamic Storage Library (DSL)

📌 Overview

The Dynamic Storage Library (DSL) is an Arduino-compatible storage system that dynamically switches between RAM (ArrayList, Hashtable) and SD storage (JSON-based blocks) depending on memory constraints.

DSL supports two template parameters:

T: Type of stored elements for ArrayList and Hashtable

K: Type of keys for Hashtable

This update introduces variant-based storage, allowing automatic handling of both single values and lists under keys within the Hashtable.

🚀 Features

✅ Automatic Memory Mode Switching (RAM, SD, or AUTO)✅ Efficient Block-Based Storage (reduces SD writes)✅ Supports Both Lists & Key-Value Storage (with dynamic value detection)✅ Batch Insertions for Lists & Mapped Lists✅ Multiple File Support (custom file names per instance)✅ Optimized JSON Serialization for Multiple Types✅ Supports Variant Storage for Mixed Values

🔧 Installation

Copy the DynamicStorage.h and related dependency files to your Arduino library folder.

Include the library in your project:

```cpp#include "DynamicStorage.h"```

🔥 Basic Usage

1️⃣ Creating a Dynamic Storage Instance
```cpp
DynamicStorage<String, int> storage(DynamicStorage<String, int>::AUTO);
```
AUTO: Automatically switches between RAM & SD.

RAM: Forces RAM-only mode.

SD: Forces SD-only mode.

2️⃣ Adding a Single Item (List Mode)
```cpp
storage.add("Hello, World!");
Serial.println(storage.get(0)); // Output: Hello, World!
```
3️⃣ Storing and Retrieving Key-Value Pairs
```cpp
storage.put("highscore", 500);
Serial.println(storage.get("highscore")); // Output: 500
```
4️⃣ Inserting Multiple Values into a List
```cpp
ArrayList<int> scores;
scores.add(100);
scores.add(200);
storage.put("scores", scores);

ArrayList<int> retrievedScores = storage.getList("scores");
Serial.println(retrievedScores.get(1)); // Output: 200
```
5️⃣ Switching Storage Mode
```cpp
storage.changeMode(DynamicStorage<String, int>::SD); // Move to SD storage
```
6️⃣ Using Multiple Files
```cpp
storage.setFilename("player_data.json");
Serial.println(storage.getFilename()); // Output: player_data.json
```
🔄 Advanced Features

Batch Insertions (List + Key-Value Pairs)
```cpp
ArrayList<float> temperatures;
temperatures.add(36.6);
temperatures.add(37.2);
storage.put("room_temps", temperatures);
```
Retrieving an Entire List
```cpp
ArrayList<float> temps = storage.getList("room_temps");
Serial.println(temps.get(0)); // Output: 36.6
```
Handling Variant Storage
```cpp
storage.put("setting", 42); // Initially stores a single value
storage.put("setting", 55); // Converts to a list under the same key
storage.put("setting", 100);

ArrayList<int> settingsList = storage.getList("setting");
Serial.println(settingsList.get(2)); // Output: 100
```
📅 Future/Possible Enhancements

* Support for CSV, Binary Storage Formats
* Indexed Searching for Large Datasets
* Data Compression for SD Storage

🚀 Contributions & Feedback Welcome! 🚀

📜 Change Log

Arduino Library Manager Version

v1.0.0 - Added variant storage support, improved SD read/write performance.
         Optimized block-based storage system.
         Introduced auto-switching between RAM and SD storage.

PlatformIO Version

v1.0.0 - Added variant storage support, improved SD read/write performance.
         Optimized block-based storage system.
         Introduced auto-switching between RAM and SD storage.
