#include <Arduino.h>
#include "SimpleArduinoTimer.h"

/*
 * Timer Example
 *
 * Demonstrates the SimpleArduinoTimer library in two modes:
 *   1. Standalone Arduino mode: the timer runs locally and prints state using
 *      the built-in Serial output.
 *   2. UnoQ bridge mode: the timer is exposed to Python over the Router Bridge
 *      so a Python app can call timer_start(), timer_set_target_seconds(),
 *      timer_elapsed_ms(), timer_has_reached_target(), and friends.
 *
 * This is the easiest way to validate the timer library in the UnoQ/App Lab
 * environment, especially when you want to inspect live values from Python and
 * verify the timer callback / repeating behavior without rewriting the C++ timer
 * logic itself.
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * PYTHON SIDE — example usage
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 *   import time
 *   def loop():
 *       print("[py] set target")
 *       Bridge.call("timer_set_target_seconds", 5)
 *       Bridge.call("timer_start")
 *       for i in range(10):
 *           status = Bridge.call("timer_status")
 *           print(status)
 *           time.sleep(0.25)
 *
 *   # Optional: the sketch also emits timer_snapshot notifications on a timer
 *   # while the app is running, which the Python side can receive as a push event.
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * THREADING NOTE
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * If TIMER_ENABLE_THREADING is enabled, the Timer monitor thread polls for
 * expiration in the background, so the sketch does not need to call
 * hasReachedTarget() in every loop. If threading is not enabled, the timer is
 * still fully usable, and loop() can call hasReachedTarget() directly.
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 */

Timer timer(false);

void onTimerEvent();

#ifdef TIMER_USE_BRIDGE
static unsigned long _nextBridgePush = 0;

void emitTimerSnapshot() {
    const bool reached = timer.hasReachedTarget();
    const unsigned long elapsedMs = timer.elapsed();
    const unsigned long targetMs = timer.getTargetDuration();

    Serial.print("[timer] snapshot: elapsed=");
    Serial.print(elapsedMs);
    Serial.print("ms target=");
    Serial.print(targetMs);
    Serial.print("ms running=");
    Serial.print(timer.isTimerRunning() ? "true" : "false");
    Serial.print(" paused=");
    Serial.print(timer.isTimerPaused() ? "true" : "false");
    Serial.print(" repeating=");
    Serial.print(timer.getRepeating() ? "true" : "false");
    Serial.print(" reached=");
    Serial.println(reached ? "true" : "false");

    Bridge.notify("timer_snapshot",
                  static_cast<int>(elapsedMs),
                  static_cast<int>(targetMs),
                  timer.isTimerRunning(),
                  timer.isTimerPaused(),
                  timer.getRepeating(),
                  reached);
}

#else

enum AppState {
    STATE_5MIN_RUN,
    STATE_5MIN_DONE,
    STATE_5MIN_PRINT_RUN,
    STATE_5MIN_PRINT_DONE,
    STATE_10SEC_RUN,
    STATE_10SEC_DONE,
    STATE_2HR_RUN,
    STATE_2HR_DONE,
    STATE_IDLE
};

AppState state = STATE_5MIN_RUN;
unsigned long nextPrint = 0;

#endif

void onTimerEvent() {
    Serial.println("[timer] target reached callback");
}

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;

    timer.setTimerName("demo");
    timer.setTargetSeconds(5);
    timer.setRepeating(true);
    timer.onTargetReached(onTimerEvent);

#ifdef TIMER_USE_BRIDGE
    Bridge.begin();
    timer.beginBridge("timer");
    Serial.println("[timer] UnoQ bridge enabled");
    Serial.println("[timer] Python can call: timer_start, timer_set_target_seconds, timer_elapsed_ms, timer_has_reached_target");
#  ifdef TIMER_ENABLE_THREADING
    Serial.println("[timer] background monitor enabled");
    timer.startMonitor(100);
#  else
    Serial.println("[timer] monitor disabled; loop() polls timer.hasReachedTarget()");
#  endif
#else
    Serial.println("[timer] standalone mode");
    nextPrint = 1000;
#endif

    timer.start();
}

void loop() {
#ifdef TIMER_USE_BRIDGE
    #ifdef TIMER_ENABLE_THREADING
    // Monitor thread handles expiration checks. Keep serial/logging in loop().
    #else
    timer.hasReachedTarget();
    #endif

    if (millis() - _nextBridgePush >= 250UL) {
        _nextBridgePush = millis();
        emitTimerSnapshot();
    }
#else
    switch (state) {
        case STATE_5MIN_RUN:
            if (timer.checkTimer(nextPrint)) {
                Serial.println(String(timer.remainingTime()));
                nextPrint += 10000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("[timer] target reached");
                state = STATE_5MIN_DONE;
            }
            break;

        case STATE_5MIN_DONE:
            timer.clear();
            timer.setTargetMinutes(5);
            timer.start();
            nextPrint = 1000;
            state = STATE_5MIN_PRINT_RUN;
            break;

        case STATE_5MIN_PRINT_RUN:
            if (timer.checkTimer(nextPrint)) {
                timer.printTimeRemaining();
                nextPrint += 1000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("[timer] target reached");
                state = STATE_5MIN_PRINT_DONE;
            }
            break;

        case STATE_5MIN_PRINT_DONE:
            timer.clear();
            timer.setTargetSeconds(10);
            timer.start();
            nextPrint = 1000;
            state = STATE_10SEC_RUN;
            break;

        case STATE_10SEC_RUN:
            if (timer.checkTimer(nextPrint)) {
                timer.printTimeRemaining();
                nextPrint += 1000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("[timer] target reached");
                state = STATE_10SEC_DONE;
            }
            break;

        case STATE_10SEC_DONE:
            timer.clear();
            timer.setTargetHours(2);
            timer.start();
            nextPrint = 60000;
            state = STATE_2HR_RUN;
            break;

        case STATE_2HR_RUN:
            if (timer.checkTimer(nextPrint)) {
                timer.printTimeRemaining();
                nextPrint += 60000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("[timer] target reached");
                state = STATE_2HR_DONE;
            }
            break;

        case STATE_2HR_DONE:
            timer.clear();
            state = STATE_IDLE;
            break;

        case STATE_IDLE:
            break;
    }
#endif
}
