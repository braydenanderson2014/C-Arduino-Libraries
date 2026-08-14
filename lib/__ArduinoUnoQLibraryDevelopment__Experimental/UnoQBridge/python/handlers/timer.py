#!/usr/bin/env python3
"""
handlers/timer.py — Timer status bridge for the UnoQ App Lab environment.

This plugin is intentionally lightweight. The sketch uses Bridge.notify() to push
periodic snapshots of a Timer instance into Python, while Python can also call
MCU timer functions via the normal Bridge.call("timer_*", ...) route.

Sketch-side wiring (example):
    Bridge.provide_safe("timer_start", timer_start);
    Bridge.provide_safe("timer_set_target_seconds", timer_set_target_seconds);
    Bridge.provide_safe("timer_elapsed_ms", timer_elapsed_ms);

Python-side usage:
    Bridge.call("timer_set_target_seconds", 5)
    Bridge.call("timer_start")
    elapsed = Bridge.call("timer_elapsed_ms")
"""

from __future__ import annotations

import json
import threading
from typing import Any, Dict

_timer_lock = threading.Lock()
_timer_state: Dict[str, Any] = {
    "elapsed_ms": 0,
    "target_ms": 0,
    "running": False,
    "paused": False,
    "repeating": False,
    "reached": False,
}


def timer_snapshot(
    elapsed_ms: int = 0,
    target_ms: int = 0,
    running: bool = False,
    paused: bool = False,
    repeating: bool = False,
    reached: bool = False,
) -> str:
    """Receive a timer snapshot pushed by the sketch via Bridge.notify()."""
    global _timer_state
    with _timer_lock:
        _timer_state = {
            "elapsed_ms": int(elapsed_ms),
            "target_ms": int(target_ms),
            "running": bool(running),
            "paused": bool(paused),
            "repeating": bool(repeating),
            "reached": bool(reached),
        }
    print(
        "[timer] elapsed_ms=%s target_ms=%s running=%s paused=%s repeating=%s reached=%s"
        % (
            _timer_state["elapsed_ms"],
            _timer_state["target_ms"],
            _timer_state["running"],
            _timer_state["paused"],
            _timer_state["repeating"],
            _timer_state["reached"],
        )
    )
    return json.dumps(_timer_state, separators=(",", ":"))


def timer_status() -> str:
    """Return the latest timer snapshot as compact JSON."""
    with _timer_lock:
        return json.dumps(_timer_state, separators=(",", ":"))


def timer_log(message: str = "") -> bool:
    """Optional log sink so the sketch can send note strings to Python."""
    print(f"[timer] {message}")
    return True


BRIDGE_FUNCTIONS = {
    "timer_snapshot": timer_snapshot,
    "timer_status": timer_status,
    "timer_log": timer_log,
}
