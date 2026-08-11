/*
 * MatrixService.ino
 *
 * MCU-side Bridge service — the sketch counterpart to main.py.
 * Runs indefinitely, accepting Bridge.call() requests from the Python container.
 *
 * ═══════════════════════════════════════════════════════════════════════
 * HOW TO USE
 * ═══════════════════════════════════════════════════════════════════════
 *
 * 1. Copy this sketch into App Lab (sketch/ folder).
 * 2. Ensure python/main.py, python/handlers/ledmatrix.py are in place.
 * 3. Click Run. The sketch waits for the Bridge, then reports "ready".
 * 4. From Python, call any mcu_* function via Bridge.call("mcu_*", ...).
 *    The Python webserver (matrixwebserver.py) uses these automatically.
 *
 * ═══════════════════════════════════════════════════════════════════════
 * PYTHON → MCU CALLS REGISTERED HERE
 * ═══════════════════════════════════════════════════════════════════════
 *
 * LED Matrix (Python calls these; sketch handles via LedMatrixStory):
 *   mcu_matrix_load_scene(csv, index)  → bool  store a scene frame
 *   mcu_matrix_preview(csv)            → bool  render frame immediately
 *   mcu_matrix_play(delayMs, loop)     → bool  start animation
 *   mcu_matrix_pause()                 → bool  pause playback
 *   mcu_matrix_stop()                  → bool  stop and rewind
 *   mcu_matrix_next()                  → bool  advance one frame
 *   mcu_matrix_prev()                  → bool  go back one frame
 *   mcu_matrix_goto(index)             → bool  jump to frame N
 *   mcu_matrix_clear()                 → bool  erase all scenes, blank display
 *   mcu_matrix_set_delay(ms)           → bool  change frame interval
 *   mcu_matrix_get_info()              → String "sceneCount,frame,playing"
 *
 * Pin control (Python calls these for GPIO):
 *   mcu_pin_mode(pin, mode)            → bool  set a pin mode (0=INPUT, 1=OUTPUT, 2=INPUT_PULLUP)
 *   mcu_digital_write(pin, value)      → bool  set a digital pin HIGH/LOW
 *   mcu_digital_read(pin)              → int   read a digital pin (0 or 1)
 *   mcu_analog_read(pin)               → int   read an analog pin (0–1023)
 *
 * MCU → PYTHON CALLS (this sketch calls Bridge.call() on Python):
 *   These let the sketch push data to Python; add more as needed.
 *   Example: Bridge.call("tm_record", String("mcu"), String("sensor reading"))
 *
 * ═══════════════════════════════════════════════════════════════════════
 * ARCHITECTURE NOTES
 * ═══════════════════════════════════════════════════════════════════════
 *
 * - Bridge.provide_safe()  → handler runs in loop() context; required for GPIO/matrix
 * - story.update()         → MUST be called every loop() iteration for animation to advance
 * - No blocking delays in loop() — use millis() for timing
 * - All Bridge function names use "mcu_" prefix to avoid routing loopback
 *   (Python's own Bridge.provide("matrix_*", ...) would intercept unprefixed calls)
 */

#include <Arduino_RouterBridge.h>
#include "LedMatrixStory.h"

// ─── Hardware ────────────────────────────────────────────────────────────────

Arduino_LED_Matrix hw;
LedMatrixStory story(hw);

// ─── Matrix handlers ─────────────────────────────────────────────────────────

bool mcu_matrix_load_scene(String csv, int index) {
    LedMatrixScene s = LedMatrixScene::fromString(csv);
    return story.setScene(static_cast<uint8_t>(index), s);
}

bool mcu_matrix_preview(String csv) {
    story.previewScene(LedMatrixScene::fromString(csv));
    return true;
}

bool mcu_matrix_play(int delayMs, bool looping) {
    story.play(static_cast<unsigned long>(delayMs), looping);
    return true;
}

bool mcu_matrix_pause() {
    story.pause();
    return true;
}

bool mcu_matrix_stop() {
    story.stop();
    return true;
}

bool mcu_matrix_next() {
    story.nextFrame();
    return true;
}

bool mcu_matrix_prev() {
    story.prevFrame();
    return true;
}

bool mcu_matrix_goto(int index) {
    story.gotoFrame(static_cast<uint8_t>(index));
    return true;
}

bool mcu_matrix_clear() {
    story.clearAll();
    story.blank();
    return true;
}

bool mcu_matrix_set_delay(int ms) {
    story.setFrameDelay(static_cast<unsigned long>(ms));
    return true;
}

String mcu_matrix_get_info() {
    // Returns "sceneCount,currentFrame,playing" — mirrors Python's matrix_status()
    String s;
    s += story.sceneCount();
    s += ',';
    s += story.currentFrame();
    s += ',';
    s += story.isPlaying() ? '1' : '0';
    return s;
}

// ─── Pin control handlers ─────────────────────────────────────────────────────

bool mcu_pin_mode(int pin, int mode) {
    // mode: 0 = INPUT, 1 = OUTPUT, 2 = INPUT_PULLUP
    uint8_t m = (mode == 1) ? OUTPUT : (mode == 2) ? INPUT_PULLUP : INPUT;
    pinMode(static_cast<uint8_t>(pin), m);
    return true;
}

bool mcu_digital_write(int pin, int value) {
    digitalWrite(static_cast<uint8_t>(pin), value ? HIGH : LOW);
    return true;
}

int mcu_digital_read(int pin) {
    return digitalRead(static_cast<uint8_t>(pin));
}

int mcu_analog_read(int pin) {
    return analogRead(static_cast<uint8_t>(pin));
}

// ─── Setup ───────────────────────────────────────────────────────────────────

void setup() {
    Monitor.begin(115200);
    Bridge.begin();

    hw.begin();
#if LEDMATRIX_HAS_GRAYSCALE
    hw.setGrayscaleBits(3);  // 8 brightness levels on Uno Q
#endif

    // Matrix handlers
    Bridge.provide_safe("mcu_matrix_load_scene", mcu_matrix_load_scene);
    Bridge.provide_safe("mcu_matrix_preview",    mcu_matrix_preview);
    Bridge.provide_safe("mcu_matrix_play",       mcu_matrix_play);
    Bridge.provide_safe("mcu_matrix_pause",      mcu_matrix_pause);
    Bridge.provide_safe("mcu_matrix_stop",       mcu_matrix_stop);
    Bridge.provide_safe("mcu_matrix_next",       mcu_matrix_next);
    Bridge.provide_safe("mcu_matrix_prev",       mcu_matrix_prev);
    Bridge.provide_safe("mcu_matrix_goto",       mcu_matrix_goto);
    Bridge.provide_safe("mcu_matrix_clear",      mcu_matrix_clear);
    Bridge.provide_safe("mcu_matrix_set_delay",  mcu_matrix_set_delay);
    Bridge.provide_safe("mcu_matrix_get_info",   mcu_matrix_get_info);

    // Pin control handlers
    Bridge.provide_safe("mcu_pin_mode",          mcu_pin_mode);
    Bridge.provide_safe("mcu_digital_write",     mcu_digital_write);
    Bridge.provide_safe("mcu_digital_read",      mcu_digital_read);
    Bridge.provide_safe("mcu_analog_read",       mcu_analog_read);

    Monitor.println("[MatrixService] waiting for Bridge...");
}

// ─── Loop ────────────────────────────────────────────────────────────────────

void loop() {
    // Advance animation — MUST be called every iteration for frames to advance
    story.update();

    // Wait for Python to be ready before sending anything
    static bool _ready = false;
    if (!_ready) {
        static uint8_t _attempts = 0;
        bool ok = false;
        Bridge.call("tm_record", String("mcu"), String("boot")).result(ok);
        if (ok) {
            _ready = true;
            Monitor.println("[MatrixService] bridge ready — service running");
        } else {
            ++_attempts;
            if (_attempts % 5 == 0) {  // print every 10s
                Monitor.print("[MatrixService] waiting for Python... ");
                Monitor.print(_attempts * 2);
                Monitor.println("s");
            }
            delay(2000);
        }
        return;
    }

    // Heartbeat every 30s so Python tab stays active
    static unsigned long _lastHeartbeat = 0;
    unsigned long now = millis();
    if (now - _lastHeartbeat >= 30000UL) {
        _lastHeartbeat = now;
        bool ok = false;
        Bridge.call("tm_record", String("mcu"), String("heartbeat")).result(ok);
    }
}
