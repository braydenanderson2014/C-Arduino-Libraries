#!/usr/bin/env python3
"""
handlers/ledmatrix.py — LED Matrix Story plugin for UnoQBridge.

Drop this file into handlers/ — it is auto-discovered and registered
by plugin_loader.py. No changes to main.py required.

Functions the ARDUINO SKETCH can call via Bridge.call("name", ...):
  matrix_status()              → str   "scenes,frame,playing"
  matrix_store_scene(csv, idx) → bool  store 104-value CSV into slot idx
  matrix_preview(csv)          → bool  render a frame without storing
  matrix_play(delay, loop)     → bool  start animation
  matrix_pause()               → bool  pause playback
  matrix_stop()                → bool  stop, return to frame 0
  matrix_next()                → bool  advance one frame
  matrix_prev()                → bool  go back one frame
  matrix_goto(index)           → bool  jump to frame N
  matrix_clear()               → bool  erase all scenes
  matrix_set_delay(ms)         → bool  change frame delay
  matrix_scene_count()         → int   number of stored scenes

Functions Python code calls to reach the ARDUINO (Bridge.call → MCU):
  Use the mcu_matrix_* helpers below inside loop() or threads.
  Do NOT call them from inside a Bridge.provide() callback — deadlock.

The sketch must register its side with Bridge.provide_safe():
  See MatrixStoryDemo.ino for the full sketch template.
"""

from __future__ import annotations

import queue
from typing import Any, Dict, List, Optional

MATRIX_ROWS   = 8
MATRIX_COLS   = 13
MATRIX_PIXELS = MATRIX_ROWS * MATRIX_COLS  # 104

# ─── Plugin state ─────────────────────────────────────────────────────────────

_bridge: Optional[Any] = None
_scenes: List[str] = []
_current_frame: int  = 0
_playing: bool       = False
_frame_delay: int    = 200

# Frames queued for MCU rendering — populated in Bridge callbacks, drained in loop()
# Unbounded so large scene sets cannot drop the final __play__ command.
_preview_queue: queue.Queue = queue.Queue()
_mcu_available: bool = False  # set True after first successful MCU call


def _queue(item: str) -> bool:
    """Put an item in the command queue; returns False silently if full."""
    try:
        _preview_queue.put_nowait(item)
        return True
    except queue.Full:
        print(f"[ledmatrix] queue full, dropped: {item[:40]}")
        return False


def setup(bridge: Any, backends: Dict[str, Any]) -> None:
    """Called by PluginLoader at startup — receives the Bridge object."""
    global _bridge
    _bridge = bridge
    print("[ledmatrix] plugin ready — sketch must Bridge.provide_safe() its side")


# Plugin loop — runs in a background thread via PluginLoader.
# This is the ONLY place that calls Bridge.call() to the MCU from this plugin;
# calling Bridge.call() inside a Bridge.provide() callback causes a deadlock.
PLUGIN_LOOP_INTERVAL = 0.1  # seconds — drain preview queue promptly

def loop() -> None:
    """Drain the command queue and relay to MCU — safe, runs outside any Bridge callback."""
    if _bridge is None:
        return
    while True:
        try:
            item = _preview_queue.get_nowait()
        except queue.Empty:
            return

        if item == "__clear__":
            _mcu("mcu_matrix_clear")
        elif item == "__pause__":
            _mcu("mcu_matrix_pause")
        elif item == "__stop__":
            _mcu("mcu_matrix_stop")
        elif item == "__next__":
            _mcu("mcu_matrix_next")
        elif item == "__prev__":
            _mcu("mcu_matrix_prev")
        elif item.startswith("__goto__"):
            _mcu("mcu_matrix_goto", int(item[8:]))
        elif item.startswith("__delay__"):
            _mcu("mcu_matrix_set_delay", int(item[9:]))
        elif item.startswith("__load__"):
            parts = item[8:].split("__", 1)
            if len(parts) == 2:
                _mcu("mcu_matrix_load_scene", parts[1], int(parts[0]))
        elif item.startswith("__play__"):
            parts = item[8:].split("__", 1)
            if len(parts) == 2:
                _mcu("mcu_matrix_play", int(parts[0]), parts[1] == "1")
        else:
            _mcu("mcu_matrix_preview", item)


# ─── Helpers ──────────────────────────────────────────────────────────────────

def _mcu(method: str, *args) -> Any:
    """Call an MCU-side Bridge.call() function. NOT safe inside provide() callbacks."""
    global _mcu_available
    if _bridge is None:
        return None
    try:
        result = _bridge.call(method, *args)
        _mcu_available = True
        return result
    except Exception as exc:
        err = str(exc)
        # 'method not available' (error 2) means sketch didn't register it — stop retrying
        if "not available" in err or "(2)" in err:
            pass  # silent: MCU simply doesn't have this sketch loaded
        else:
            print(f"[ledmatrix] Bridge.call({method!r}) ERROR: {exc}")
        return None


def _valid_csv(csv: str) -> bool:
    """Return True if csv looks like a valid 104-value pixel string."""
    parts = csv.split(",")
    return len(parts) == MATRIX_PIXELS and all(p.strip().isdigit() for p in parts)


# ─── Bridge functions (ARDUINO calls these → Python handles) ──────────────────

def matrix_status(*args) -> str:
    """Return 'sceneCount,currentFrame,isPlaying' — safe short string."""
    return f"{len(_scenes)},{_current_frame},{1 if _playing else 0}"


def matrix_store_scene(csv: str, index: int, *args) -> bool:
    """Store a 104-value CSV pixel frame into slot `index`."""
    global _scenes
    if not _valid_csv(csv):
        print(f"[ledmatrix] matrix_store_scene: invalid CSV ({len(csv.split(','))} values)")
        return False
    idx = int(index)
    while len(_scenes) <= idx:
        _scenes.append(",".join(["0"] * MATRIX_PIXELS))
    _scenes[idx] = csv
    print(f"[ledmatrix] stored scene {idx} ({len(_scenes)} total)")
    return True


def matrix_preview(csv: str) -> bool:
    """Queue a frame for MCU rendering. Returns immediately — no MCU call here.

    Calling Bridge.call() (via _mcu) from inside a Bridge.provide() callback
    deadlocks the router. The plugin loop() drains this queue from its own thread.
    """
    if not _valid_csv(csv):
        print(f"[ledmatrix] matrix_preview: invalid CSV ({len(csv.split(','))} values, expected {MATRIX_PIXELS})")
        return False
    try:
        _preview_queue.put_nowait(csv)
        print(f"[ledmatrix] matrix_preview: queued for MCU render")
        return True
    except queue.Full:
        print("[ledmatrix] matrix_preview: queue full, dropped frame")
        return False


def matrix_play(delay: int = 200, loop: bool = True, *args) -> bool:
    """Sync all stored scenes to MCU and start playback."""
    global _playing, _frame_delay
    _frame_delay = int(delay)
    _playing = True
    _queue("__clear__")
    for i, csv in enumerate(_scenes):
        _queue(f"__load__{i}__{csv}")
    _queue(f"__play__{delay}__{1 if loop else 0}")
    print(f"[ledmatrix] matrix_play: queued {len(_scenes)} scene(s) @ {delay}ms, loop={loop}")
    return True


def matrix_pause(*args) -> bool:
    global _playing
    _playing = False
    _queue("__pause__")
    return True


def matrix_stop(*args) -> bool:
    global _playing, _current_frame
    _playing = False
    _current_frame = 0
    _queue("__stop__")
    return True


def matrix_next(*args) -> bool:
    global _current_frame
    if _scenes:
        _current_frame = (_current_frame + 1) % len(_scenes)
    _queue("__next__")
    return True


def matrix_prev(*args) -> bool:
    global _current_frame
    if _scenes:
        _current_frame = (_current_frame - 1) % len(_scenes)
    _queue("__prev__")
    return True


def matrix_goto(index: int, *args) -> bool:
    global _current_frame
    _current_frame = int(index)
    _queue(f"__goto__{int(index)}")
    return True


def matrix_clear(*args) -> bool:
    global _scenes, _current_frame, _playing
    _scenes = []
    _current_frame = 0
    _playing = False
    _queue("__clear__")
    return True


def matrix_set_delay(ms: int, *args) -> bool:
    global _frame_delay
    _frame_delay = int(ms)
    _queue(f"__delay__{int(ms)}")
    return True


def matrix_scene_count(*args) -> int:
    return len(_scenes)


# ─── Python-side helpers (call MCU directly) ──────────────────────────────────
# Use these from loop() or thread manager tasks, NOT from Bridge.provide() callbacks.

def mcu_matrix_preview(csv: str) -> bool:
    """Python → MCU: render a frame immediately."""
    return _mcu("matrix_preview", csv) is not False


def mcu_matrix_play(delay: int = 200, loop: bool = True) -> bool:
    """Python → MCU: start playing the story."""
    return matrix_play(delay, loop)


def mcu_matrix_stop() -> bool:
    """Python → MCU: stop playback."""
    return matrix_stop()


def mcu_matrix_led_set(row: int, col: int, brightness: int) -> bool:
    """
    Python → MCU: set a single pixel by building a scene and previewing it.
    NOTE: This creates a full 104-pixel frame with only one pixel lit.
    For efficient single-pixel updates, send the full frame CSV instead.
    """
    pixels = ["0"] * MATRIX_PIXELS
    idx = int(row) * MATRIX_COLS + int(col)
    if 0 <= idx < MATRIX_PIXELS:
        pixels[idx] = str(int(brightness))
    return mcu_matrix_preview(",".join(pixels))


# ─── Plugin registration ──────────────────────────────────────────────────────
# PluginLoader reads this dict and calls Bridge.provide(name, fn) for each entry.

BRIDGE_FUNCTIONS: Dict[str, Any] = {
    "matrix_status":      matrix_status,
    "matrix_store_scene": matrix_store_scene,
    "matrix_preview":     matrix_preview,
    "matrix_play":        matrix_play,
    "matrix_pause":       matrix_pause,
    "matrix_stop":        matrix_stop,
    "matrix_next":        matrix_next,
    "matrix_prev":        matrix_prev,
    "matrix_goto":        matrix_goto,
    "matrix_clear":       matrix_clear,
    "matrix_set_delay":   matrix_set_delay,
    "matrix_scene_count": matrix_scene_count,
}
