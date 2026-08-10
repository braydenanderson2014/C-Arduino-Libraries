/*
 * PinControl.ino
 *
 * Demonstrates bidirectional Arduino ↔ Python bridge communication.
 *
 * ═══════════════════════════════════════════════════════════════════════
 * DIRECTION 1 — Python → Arduino (Python controls pins)
 * ═══════════════════════════════════════════════════════════════════════
 *
 * Python calls these registered functions via Bridge.call():
 *
 *   Bridge.call("pin_write",  pin, value)   → bool
 *   Bridge.call("pin_read",   pin)           → int  (0 or 1)
 *   Bridge.call("pwm_write",  pin, value)   → bool  (value 0-255)
 *   Bridge.call("adc_read",   pin)           → int  (0-4095, 12-bit)
 *   Bridge.call("pin_mode",   pin, mode)    → bool  (0=IN, 1=OUT, 2=PULLUP)
 *   Bridge.call("led_set",    value)         → bool
 *   Bridge.call("multi_write", pinMask)      → bool  (bitmask: pin N = bit N)
 *
 * ═══════════════════════════════════════════════════════════════════════
 * DIRECTION 2 — Arduino → Python (sketch pushes data to Python)
 * ═══════════════════════════════════════════════════════════════════════
 *
 * The sketch calls Python functions registered with Bridge.provide()
 * in main.py. Use Bridge.notify() for fire-and-forget (no result needed),
 * Bridge.call() when you need a return value from Python.
 *
 *   Bridge.notify("on_sensor_data", pin, value)  — push ADC reading
 *   Bridge.notify("on_gpio_event",  pin, state)  — push edge event
 *   Bridge.call("fs_write", path, content)        — store data in a file
 *   Bridge.call("tm_record", thread, message)     — log to thread-memory
 *
 * ═══════════════════════════════════════════════════════════════════════
 * ⚠️  DEADLOCK RULE
 * ═══════════════════════════════════════════════════════════════════════
 *
 * NEVER call Bridge.call() or Monitor.print() inside a Bridge.provide()
 * or Bridge.provide_safe() callback. Doing so causes a communication
 * deadlock. If you need to react to a Python command by reading a sensor
 * or calling back to Python, set a flag in the callback and act on it
 * in loop().
 *
 * ═══════════════════════════════════════════════════════════════════════
 * HOW TO USE
 * ═══════════════════════════════════════════════════════════════════════
 *
 * 1. Copy this file into App Lab's sketch/ folder.
 * 2. Make sure python/main.py (with mcu_* wrappers) is in place.
 * 3. Click Run. Open Serial Monitor and Python tabs.
 * 4. Python loop() will call pin_write/adc_read every 5 s automatically.
 * 5. Sketch loop() pushes sensor readings to Python every 10 s.
 */

#include <Arduino_RouterBridge.h>

// ─── Shared state ────────────────────────────────────────────────────────────

// Flags set by Bridge callbacks, acted on in loop() to avoid deadlock.
static volatile int  _pendingPinMode = -1;
static volatile int  _pendingPinModeMode = -1;
static volatile bool _pendingPinModeReady = false;

// ─── Functions Python can call ───────────────────────────────────────────────

// All use provide_safe — they call Arduino APIs (digitalWrite etc.) which must
// run in the main loop context on Zephyr / mbed RTOS.

bool pin_write(int pin, bool value) {
    digitalWrite(pin, value ? HIGH : LOW);
    return true;
}

int pin_read(int pin) {
    return (int)digitalRead(pin);
}

bool pwm_write(int pin, int value) {
    analogWrite(pin, constrain(value, 0, 255));
    return true;
}

int adc_read(int pin) {
    return analogRead(pin);
}

// mode: 0 = INPUT, 1 = OUTPUT, 2 = INPUT_PULLUP
bool pin_mode_set(int pin, int mode) {
    if      (mode == 0) pinMode(pin, INPUT);
    else if (mode == 1) pinMode(pin, OUTPUT);
    else if (mode == 2) pinMode(pin, INPUT_PULLUP);
    return true;
}

bool led_set(bool value) {
    digitalWrite(LED_BUILTIN, value ? HIGH : LOW);
    return true;
}

// bitmask: bit N high → set pin N HIGH; supports pins 0-7
bool multi_write(int pinMask) {
    for (int p = 0; p < 8; ++p) {
        if (pinMask & (1 << p)) {
            digitalWrite(p, HIGH);
        } else {
            digitalWrite(p, LOW);
        }
    }
    return true;
}

// ─── Setup ───────────────────────────────────────────────────────────────────

void setup() {
    Monitor.begin();
    Bridge.begin();
    pinMode(LED_BUILTIN, OUTPUT);

    // Register all MCU functions so Python can call them.
    // provide_safe ensures execution in main loop — required for Arduino APIs.
    Bridge.provide_safe("pin_write",  pin_write);
    Bridge.provide_safe("pin_read",   pin_read);
    Bridge.provide_safe("pwm_write",  pwm_write);
    Bridge.provide_safe("adc_read",   adc_read);
    Bridge.provide_safe("pin_mode",   pin_mode_set);
    Bridge.provide_safe("led_set",    led_set);
    Bridge.provide_safe("multi_write",multi_write);

    Monitor.println("[MCU] Pin control bridge ready");
    Monitor.println("[MCU] Python can now call: pin_write, pin_read, pwm_write, adc_read, pin_mode, led_set, multi_write");
}

// ─── Loop ────────────────────────────────────────────────────────────────────

static unsigned long _lastPush = 0;

void loop() {
    // Push ADC readings to Python every 10 s.
    // Bridge.notify() is fire-and-forget — no result, no deadlock risk.
    if (millis() - _lastPush >= 10000UL) {
        _lastPush = millis();

        int a0 = analogRead(A0);
        int a1 = analogRead(A1);

        // Python receives these via Bridge.provide("on_sensor_data", fn) in main.py
        Bridge.notify("on_sensor_data", (int)A0, a0);
        Bridge.notify("on_sensor_data", (int)A1, a1);

        Monitor.print("[MCU] pushed A0="); Monitor.print(a0);
        Monitor.print("  A1="); Monitor.println(a1);
    }
}
