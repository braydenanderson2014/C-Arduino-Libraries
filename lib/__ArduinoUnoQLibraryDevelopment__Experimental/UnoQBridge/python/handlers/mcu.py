#!/usr/bin/env python3
"""
handlers/mcu.py — Thread-safe Python → MCU call dispatcher.

Single point of contact for all Python → MCU Bridge.call() invocations.
All other modules (ledmatrix, webserver, etc.) must use this module instead of
calling Bridge.call() directly to guarantee thread safety.

Thread safety model
───────────────────
  • _bridge_lock (RLock) — serialises every Bridge.call(); RLock allows the same
    thread to re-enter (e.g. loop() calling call_safe() which calls Bridge.call()).
  • Bridge.provide() callbacks registered here NEVER call Bridge.call() — they
    only enqueue work or set flags that loop() acts on.
  • call_async() queues a (method, args, callback) tuple; loop() drains it serially
    while holding the lock, so async callers cannot deadlock or race.
  • Blocking callers that must await a result use call_safe() directly from their
    own thread (safe as long as they are not inside a Bridge.provide() callback).

Setup
─────
  import handlers.mcu as mcu
  mcu.setup(bridge, backends)   # call before App.run()

Sketch side (MatrixService.ino or equivalent)
──────────────────────────────────────────────
  Bridge.provide_safe("mcu_pin_mode",        mcu_pin_mode);
  Bridge.provide_safe("mcu_digital_write",   mcu_digital_write);
  Bridge.provide_safe("mcu_digital_read",    mcu_digital_read);
  Bridge.provide_safe("mcu_analog_read",     mcu_analog_read);
  Bridge.provide_safe("mcu_matrix_*",        ...);   // MatrixService.ino
"""

from __future__ import annotations

import queue
import threading
from typing import Any, Callable, Dict, List, Optional, Tuple

# ─── Module state ─────────────────────────────────────────────────────────────

_bridge: Optional[Any] = None
_bridge_lock = threading.RLock()   # RLock: same thread may re-enter safely

# Async call queue: each item is (method, args, callback_or_None)
_call_queue: queue.Queue = queue.Queue()

# Registered MCU → Python push callbacks (set via register_push_callback)
_push_callbacks: Dict[str, Callable] = {}

# Last known error per method, for diagnostics
_last_errors: Dict[str, str] = {}

PLUGIN_LOOP_INTERVAL = 0.05  # 20 Hz — drain async queue promptly

# ─── Core ─────────────────────────────────────────────────────────────────────

def setup(bridge: Any, backends: Dict[str, Any]) -> None:
    """Called by PluginLoader (or main.py) at startup."""
    global _bridge
    _bridge = bridge
    print("[mcu] thread-safe MCU dispatcher ready")


def call_safe(method: str, *args) -> Any:
    """
    Blocking, thread-safe Bridge.call().

    Acquires _bridge_lock then calls Bridge.call(method, *args).
    Returns the result value or None on error.

    MUST NOT be called from inside a Bridge.provide() callback — deadlock.
    Call from loop(), background threads, or direct user code only.
    """
    if _bridge is None:
        return None
    with _bridge_lock:
        try:
            result = _bridge.call(method, *args)
            _last_errors.pop(method, None)
            return result
        except Exception as exc:
            err = str(exc)
            _last_errors[method] = err
            silent = "not available" in err or "(2)" in err
            if not silent:
                print(f"[mcu] {method}({', '.join(str(a) for a in args)}) ERROR: {exc}")
            return None


def call_async(method: str, *args, callback: Optional[Callable[[Any], None]] = None) -> None:
    """
    Non-blocking enqueue of a Bridge.call().

    The call runs in loop() under the lock.  If callback is provided it is
    invoked with the result value after the call completes.

    Safe to call from anywhere — including Bridge.provide() callbacks.
    """
    _call_queue.put_nowait((method, args, callback))


def notify(method: str, *args) -> None:
    """Fire-and-forget Bridge.notify() — does not wait for a response."""
    if _bridge is None:
        return
    with _bridge_lock:
        try:
            _bridge.notify(method, *args)
        except Exception as exc:
            print(f"[mcu] notify({method!r}) ERROR: {exc}")


def register_push_callback(name: str, fn: Callable) -> None:
    """
    Register a Python function that the MCU can trigger via Bridge.call(name, ...).

    The function runs in the Bridge callback thread — it MUST NOT call call_safe()
    or Bridge.call() directly.  Queue work via call_async() instead.
    """
    _push_callbacks[name] = fn
    if _bridge is not None:
        _bridge.provide(name, fn)


def last_error(method: str) -> Optional[str]:
    """Return the last error string for a method, or None if the last call succeeded."""
    return _last_errors.get(method)


def loop() -> None:
    """Drain the async call queue. Must be called regularly (from plugin or main loop)."""
    while True:
        try:
            method, args, cb = _call_queue.get_nowait()
        except queue.Empty:
            break
        result = call_safe(method, *args)
        if cb is not None:
            try:
                cb(result)
            except Exception as exc:
                print(f"[mcu] async callback for {method!r} raised: {exc}")


# ─── GPIO wrappers ────────────────────────────────────────────────────────────
# Sketch must register: Bridge.provide_safe("mcu_pin_mode", mcu_pin_mode) etc.

def pin_mode(pin: int, mode: int) -> bool:
    """Set pin mode. mode: 0=INPUT 1=OUTPUT 2=INPUT_PULLUP."""
    result = call_safe("mcu_pin_mode", int(pin), int(mode))
    return bool(result) if result is not None else False

def digital_write(pin: int, value: bool) -> bool:
    """Set a digital pin HIGH (True) or LOW (False)."""
    result = call_safe("mcu_digital_write", int(pin), bool(value))
    return bool(result) if result is not None else False

def digital_read(pin: int) -> int:
    """Read a digital pin. Returns 0 or 1; -1 on error."""
    result = call_safe("mcu_digital_read", int(pin))
    return int(result) if result is not None else -1

def analog_read(pin: int) -> int:
    """Read an analog pin (0–1023 / 0–4095 depending on board). -1 on error."""
    result = call_safe("mcu_analog_read", int(pin))
    return int(result) if result is not None else -1

def pwm_write(pin: int, value: int) -> bool:
    """analogWrite — PWM duty 0-255."""
    result = call_safe("mcu_pwm_write", int(pin), max(0, min(255, int(value))))
    return bool(result) if result is not None else False


# ─── Async GPIO wrappers ──────────────────────────────────────────────────────
# Non-blocking variants — safe inside Bridge.provide() callbacks.

def digital_write_async(pin: int, value: bool,
                        callback: Optional[Callable[[bool], None]] = None) -> None:
    call_async("mcu_digital_write", int(pin), bool(value),
               callback=callback)

def pin_mode_async(pin: int, mode: int,
                   callback: Optional[Callable[[bool], None]] = None) -> None:
    call_async("mcu_pin_mode", int(pin), int(mode), callback=callback)

def pwm_write_async(pin: int, value: int,
                    callback: Optional[Callable[[bool], None]] = None) -> None:
    call_async("mcu_pwm_write", int(pin), max(0, min(255, int(value))),
               callback=callback)


# ─── LED matrix wrappers ──────────────────────────────────────────────────────
# Matches MatrixService.ino Bridge.provide_safe("mcu_matrix_*", ...) registrations.

def matrix_load_scene(csv: str, index: int) -> bool:
    result = call_safe("mcu_matrix_load_scene", str(csv), int(index))
    return bool(result) if result is not None else False

def matrix_preview(csv: str) -> bool:
    result = call_safe("mcu_matrix_preview", str(csv))
    return bool(result) if result is not None else False

def matrix_play(delay_ms: int = 200, loop: bool = True) -> bool:
    result = call_safe("mcu_matrix_play", int(delay_ms), bool(loop))
    return bool(result) if result is not None else False

def matrix_pause() -> bool:
    result = call_safe("mcu_matrix_pause")
    return bool(result) if result is not None else False

def matrix_stop() -> bool:
    result = call_safe("mcu_matrix_stop")
    return bool(result) if result is not None else False

def matrix_next() -> bool:
    result = call_safe("mcu_matrix_next")
    return bool(result) if result is not None else False

def matrix_prev() -> bool:
    result = call_safe("mcu_matrix_prev")
    return bool(result) if result is not None else False

def matrix_goto(index: int) -> bool:
    result = call_safe("mcu_matrix_goto", int(index))
    return bool(result) if result is not None else False

def matrix_clear() -> bool:
    result = call_safe("mcu_matrix_clear")
    return bool(result) if result is not None else False

def matrix_set_delay(ms: int) -> bool:
    result = call_safe("mcu_matrix_set_delay", int(ms))
    return bool(result) if result is not None else False

def matrix_get_info() -> str:
    """Returns 'sceneCount,currentFrame,playing' from the MCU."""
    result = call_safe("mcu_matrix_get_info")
    return str(result) if result is not None else "0,0,0"


# ─── Async matrix wrappers ────────────────────────────────────────────────────

def matrix_preview_async(csv: str,
                         callback: Optional[Callable[[bool], None]] = None) -> None:
    call_async("mcu_matrix_preview", str(csv), callback=callback)

def matrix_play_async(delay_ms: int = 200, loop: bool = True,
                      callback: Optional[Callable[[bool], None]] = None) -> None:
    call_async("mcu_matrix_play", int(delay_ms), bool(loop), callback=callback)

def matrix_load_scene_async(csv: str, index: int,
                            callback: Optional[Callable[[bool], None]] = None) -> None:
    call_async("mcu_matrix_load_scene", str(csv), int(index), callback=callback)


# ─── MCU → Python push callbacks ─────────────────────────────────────────────
# Register these before App.run(). The MCU calls Bridge.call(name, ...) to push data.
# Callbacks run in Bridge's receive thread — queue work via call_async(), never call_safe().

_sensor_data: Dict[int, int] = {}     # {pin: last_value}
_gpio_events: List[Tuple[int, int]] = []   # [(pin, state), ...]

def _on_sensor_data(pin: int, value: int) -> None:
    _sensor_data[int(pin)] = int(value)

def _on_gpio_event(pin: int, state: int) -> None:
    _gpio_events.append((int(pin), int(state)))
    if len(_gpio_events) > 64:  # cap ring buffer
        _gpio_events.pop(0)

def get_sensor_data() -> Dict[int, int]:
    """Returns the last ADC/digital value pushed by the MCU for each pin."""
    return dict(_sensor_data)

def get_gpio_events() -> List[Tuple[int, int]]:
    """Returns recent (pin, state) edge events pushed by the MCU."""
    return list(_gpio_events)

def clear_gpio_events() -> None:
    _gpio_events.clear()


# ─── Plugin registration table ────────────────────────────────────────────────
# PluginLoader reads BRIDGE_FUNCTIONS and calls Bridge.provide(name, fn) for each.
# These are Python functions callable BY THE MCU (MCU → Python direction).

BRIDGE_FUNCTIONS: Dict[str, Any] = {
    "on_sensor_data": _on_sensor_data,
    "on_gpio_event":  _on_gpio_event,
}
