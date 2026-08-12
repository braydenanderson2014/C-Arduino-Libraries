/*
 * BridgeBlink.ino
 *
 * Blinks LED3_R (the red MCU-controlled RGB LED) two ways:
 *   1. Standalone: sketch blinks automatically every 500 ms on its own.
 *   2. Bridge-controlled: Python calls set_led(bool) to override the state.
 *
 * This lets you confirm the LED works before worrying about the Python side.
 *
 * ═══════════════════════════════════════════════════════════════════
 * PYTHON SIDE — add to loop() in main.py
 * ═══════════════════════════════════════════════════════════════════
 *
 *   import time
 *   _led = False
 *   def loop():
 *       global _led
 *       _led = not _led
 *       Bridge.call("set_led", _led)   # True = ON, False = OFF
 *       print(f"[blink] set_led -> {_led}")
 *       time.sleep(1)
 *
 * ═══════════════════════════════════════════════════════════════════
 * NOTE: Uno Q LEDs are active-LOW
 * ═══════════════════════════════════════════════════════════════════
 * HIGH = OFF, LOW = ON. The set_led function inverts automatically.
 */

#include <Arduino_RouterBridge.h>

// Returns bool so MsgPack has a concrete return type to serialise
bool set_led(bool state) {
    // LED3_R is active-LOW on Uno Q
    digitalWrite(LED3_R, state ? LOW : HIGH);
    Serial.print("[blink] LED3_R ");
    Serial.println(state ? "ON" : "OFF");
    return state;
}

static bool          _localState   = false;
static unsigned long _lastToggle   = 0;

void setup() {
    Serial.begin(115200);
    Bridge.begin();

    pinMode(LED3_R, OUTPUT);
    digitalWrite(LED3_R, HIGH);  // OFF initially (active-LOW)

    Bridge.provide_safe("set_led", set_led);

    Serial.println("[blink] local blink active — Python can call set_led(bool) to override");
}

void loop() {
    // Local blink runs continuously; provide_safe fires set_led() whenever Python calls it
    if (millis() - _lastToggle >= 500UL) {
        _lastToggle = millis();
        _localState = !_localState;
        digitalWrite(LED3_R, _localState ? LOW : HIGH);
    }
}

