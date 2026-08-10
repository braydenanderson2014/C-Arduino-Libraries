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

from typing import Any, Dict, List, Optional

MATRIX_ROWS   = 8
MATRIX_COLS   = 13
MATRIX_PIXELS = MATRIX_ROWS * MATRIX_COLS  # 104

# ─── Plugin state ─────────────────────────────────────────────────────────────

_bridge: Optional[Any] = None
_scenes: List[str] = []        # CSV strings, one per scene
_current_frame: int  = 0
_playing: bool       = False
_frame_delay: int    = 200


def setup(bridge: Any, backends: Dict[str, Any]) -> None:
    """Called by PluginLoader at startup — receives the Bridge object."""
    global _bridge
    _bridge = bridge
    print("[ledmatrix] plugin ready — sketch must Bridge.provide_safe() its side")


# ─── Helpers ──────────────────────────────────────────────────────────────────

def _mcu(method: str, *args) -> Any:
    """Call an MCU-side Bridge.call() function. NOT safe inside provide() callbacks."""
    if _bridge is None:
        return None
    try:
        return _bridge.call(method, *args)
    except Exception as exc:
        print(f"[ledmatrix] Bridge.call({method!r}) ERROR: {exc}")
        return None


def _valid_csv(csv: str) -> bool:
    """Return True if csv looks like a valid 104-value pixel string."""
    parts = csv.split(",")
    return len(parts) == MATRIX_PIXELS and all(p.strip().isdigit() for p in parts)


# ─── Bridge functions (ARDUINO calls these → Python handles) ──────────────────

def matrix_status() -> str:
    """Return 'sceneCount,currentFrame,isPlaying' — safe short string."""
    return f"{len(_scenes)},{_current_frame},{1 if _playing else 0}"


def matrix_store_scene(csv: str, index: int) -> bool:
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
    """Render a frame immediately on the MCU without storing it."""
    if not _valid_csv(csv):
        return False
    result = _mcu("matrix_preview", csv)
    return result is not False


def matrix_play(delay: int = 200, loop: bool = True) -> bool:
    """Start animation on the MCU. Syncs all stored scenes first."""
    global _playing, _frame_delay
    _frame_delay = int(delay)
    _playing = True
    # Sync all scenes to MCU before playing
    _mcu("matrix_clear", "")
    for i, csv in enumerate(_scenes):
        _mcu("matrix_load_scene", csv, i)
    result = _mcu("matrix_play", int(delay), bool(loop))
    print(f"[ledmatrix] play: {len(_scenes)} scenes @ {delay}ms, loop={loop}")
    return result is not False


def matrix_pause() -> bool:
    global _playing
    _playing = False
    return _mcu("matrix_pause", "") is not False


def matrix_stop() -> bool:
    global _playing, _current_frame
    _playing = False
    _current_frame = 0
    return _mcu("matrix_stop", "") is not False


def matrix_next() -> bool:
    global _current_frame
    if _scenes:
        _current_frame = (_current_frame + 1) % len(_scenes)
    return _mcu("matrix_next", "") is not False


def matrix_prev() -> bool:
    global _current_frame
    if _scenes:
        _current_frame = (_current_frame - 1) % len(_scenes)
    return _mcu("matrix_prev", "") is not False


def matrix_goto(index: int) -> bool:
    global _current_frame
    _current_frame = int(index)
    return _mcu("matrix_goto", int(index)) is not False


def matrix_clear() -> bool:
    global _scenes, _current_frame, _playing
    _scenes = []
    _current_frame = 0
    _playing = False
    return _mcu("matrix_clear", "") is not False


def matrix_set_delay(ms: int) -> bool:
    global _frame_delay
    _frame_delay = int(ms)
    return _mcu("matrix_set_delay", int(ms)) is not False


def matrix_scene_count() -> int:
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
