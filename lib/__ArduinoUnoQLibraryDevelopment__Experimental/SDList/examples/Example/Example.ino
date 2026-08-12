/*
    Example Name: SDList Library – Basic Usage

    Demonstrates binary batch-buffered storage on an SD card.
    The list stores and retrieves typed values (int, float, struct)
    without any text serialisation, so data integrity is guaranteed.

    Circuit:
      SD card module connected via SPI; chip-select on pin 4.

    Created  2024-02-18  braydenanderson2014 (Brayden Anderson)
    Updated  2025-07-24  braydenanderson2014 (Brayden Anderson) – v2 rewrite

    https://github.com/braydenanderson2014/C-Arduino-Libraries
*/

#include <Arduino.h>
#include "SDList.h"

// ── Example 1: integer list on SD card ───────────────────────────────────────
//   BATCH_SIZE = 8  (8 ints live in RAM at once; the rest stay on the card)
//   reserveCapacity = 32  (32 slots pre-allocated in the file)
SDList<int, 8> intList(SDCARD, 32);

// ── Example 2: float list on SD card with a unique filename ──────────────────
SDList<float, 4> floatList(SDCARD, 16);

// ── Example 3: pure memory mode (no SD card required) ────────────────────────
SDList<int, 8> memList(MEMORY, 8);

// ── Example 4: custom struct ─────────────────────────────────────────────────
struct Point { int16_t x; int16_t y; };
SDList<Point, 4> pointList(SDCARD, 8);

void setup() {
    Serial.begin(9600);
    while (!Serial) {}

    // ── Init SD lists ─────────────────────────────────────────────────────────
    // Each instance uses a unique filename to avoid conflicts.
    if (!intList.begin(4, "ints.bin")) {
        Serial.println("SD init failed for intList – running in memory mode");
    }
    if (!floatList.begin(4, "floats.bin")) {
        Serial.println("SD init failed for floatList – running in memory mode");
    }
    if (!pointList.begin(4, "points.bin")) {
        Serial.println("SD init failed for pointList – running in memory mode");
    }
    // MEMORY mode needs no begin() call, but it is harmless to call it.
    memList.begin(4);

    // ── Append ───────────────────────────────────────────────────────────────
    for (int i = 0; i < 20; i++) {
        intList.append(i * 10);   // 0, 10, 20 … 190
    }
    floatList.append(1.1f);
    floatList.append(2.2f);
    floatList.append(3.3f);

    Point p; p.x = 10; p.y = 20;
    pointList.append(p);
    p.x = 30; p.y = 40;
    pointList.append(p);

    for (int i = 0; i < 5; i++) memList.append(i);

    // ── get / set ─────────────────────────────────────────────────────────────
    Serial.print("intList[5]  = "); Serial.println(intList.get(5));   // 50
    Serial.print("floatList[1]= "); Serial.println(floatList.get(1)); // 2.20
    Point q = pointList.get(0);
    Serial.print("pointList[0]= ("); Serial.print(q.x);
    Serial.print(", "); Serial.print(q.y); Serial.println(")");       // (10, 20)

    intList.set(5, 999);
    Serial.print("intList[5] after set = "); Serial.println(intList.get(5)); // 999

    // ── insert / remove ───────────────────────────────────────────────────────
    intList.insert(0, -1);  // prepend -1
    Serial.print("intList[0] after insert = "); Serial.println(intList.get(0)); // -1

    intList.remove(0);      // remove the -1 we just inserted
    Serial.print("intList[0] after remove = "); Serial.println(intList.get(0)); // 0

    // ── size / capacity ───────────────────────────────────────────────────────
    Serial.print("intList size     = "); Serial.println(intList.size());     // 20
    Serial.print("intList capacity = "); Serial.println(intList.capacity()); // >= 20
    Serial.print("floatList size   = "); Serial.println(floatList.size());   // 3
    Serial.print("memList size     = "); Serial.println(memList.size());     // 5

    // ── explicit flush ────────────────────────────────────────────────────────
    // The batch is also flushed automatically when the window shifts or on
    // destruction, but call flush() explicitly for maximum durability.
    intList.flush();

    // ── clear ─────────────────────────────────────────────────────────────────
    memList.clear();
    Serial.print("memList size after clear = "); Serial.println(memList.size()); // 0
}

void loop() {
    // Nothing here – all work done in setup().
}

