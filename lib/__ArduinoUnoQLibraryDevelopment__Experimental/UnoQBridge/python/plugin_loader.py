#!/usr/bin/env python3
"""
plugin_loader.py — Auto-discovers and registers handler plugins.

Drop any .py file into handlers/ that follows the plugin convention
and it will be loaded automatically when main.py starts.

═══════════════════════════════════════════════════════════════════
PLUGIN CONVENTION
═══════════════════════════════════════════════════════════════════

A handler plugin is any file in handlers/ that exposes one or more
of the following module-level attributes:

  BRIDGE_FUNCTIONS: dict[str, callable]
      Functions the Arduino SKETCH can call via Bridge.call("name").
      Each entry becomes a Bridge.provide("name", fn) registration.

      Example:
          def matrix_status() -> str:
              return "ok"

          BRIDGE_FUNCTIONS = {
              "matrix_status": matrix_status,
          }

  PLUGIN_LOOP_INTERVAL: float   (seconds, optional)
      If present alongside a loop() function, the loader schedules
      loop() to be called repeatedly at this interval in a background
      thread. Use for plugins that need periodic work.

  def setup(bridge, backends: dict) -> None:
      Called once at startup. Use this when you need the Bridge object
      or access to other backends (filesystem, threadmemory, etc.).
      Can call bridge.provide() directly for complex registrations.

  def loop() -> None:
      Called periodically if PLUGIN_LOOP_INTERVAL is set.

═══════════════════════════════════════════════════════════════════
SKIP LIST
═══════════════════════════════════════════════════════════════════
Modules in SKIP_MODULES are ignored by the loader (they are
managed explicitly by main.py — they have their own backend classes
and dispatch functions that require special initialisation).

═══════════════════════════════════════════════════════════════════
USAGE IN main.py
═══════════════════════════════════════════════════════════════════

  from plugin_loader import PluginLoader

  # After Bridge backends are initialised and before App.run():
  _loader = PluginLoader(Bridge, backends={
      "file": _fs, "threadmemory": _tm,
      "networking": _net, "threadmanager": _tmgr,
  })
  _loader.load_all()
"""

from __future__ import annotations

import importlib
import pkgutil
import threading
import time
from typing import Any, Dict, Optional

# These modules are managed explicitly in main.py — skip auto-registration
SKIP_MODULES = frozenset({
    "filesystem",
    "threadmemory",
    "networking",
    "threadmanager",
    "__init__",
})


class PluginLoader:
    def __init__(self, bridge: Any, backends: Optional[Dict[str, Any]] = None) -> None:
        self._bridge   = bridge
        self._backends = backends or {}
        self._plugins: Dict[str, Any] = {}  # name → module

    def load_all(self) -> None:
        """Scan handlers/ and load every eligible plugin module."""
        import handlers as _pkg
        for finder, name, _ in pkgutil.iter_modules(_pkg.__path__):
            if name in SKIP_MODULES:
                continue
            self._load(name)

    def _load(self, name: str) -> None:
        try:
            mod = importlib.import_module(f"handlers.{name}")
        except Exception as exc:
            print(f"[plugin] ERROR importing handlers/{name}.py: {exc}")
            return

        registered = []

        # 1. setup(bridge, backends) — full control initialisation
        setup_fn = getattr(mod, "setup", None)
        if callable(setup_fn):
            try:
                setup_fn(self._bridge, self._backends)
                registered.append("setup()")
            except Exception as exc:
                print(f"[plugin] handlers/{name}.py setup() ERROR: {exc}")

        # 2. BRIDGE_FUNCTIONS — auto-register with Bridge.provide()
        bridge_fns: Dict[str, Any] = getattr(mod, "BRIDGE_FUNCTIONS", {})
        for fn_name, fn in bridge_fns.items():
            try:
                self._bridge.provide(fn_name, fn)
                registered.append(fn_name)
            except Exception as exc:
                print(f"[plugin] handlers/{name}.py Bridge.provide({fn_name!r}) ERROR: {exc}")

        # 3. Periodic loop() — run in a background daemon thread
        loop_fn = getattr(mod, "loop", None)
        interval = getattr(mod, "PLUGIN_LOOP_INTERVAL", None)
        if callable(loop_fn) and interval is not None:
            self._start_loop(name, loop_fn, float(interval))
            registered.append(f"loop()@{interval}s")

        if registered:
            print(f"[plugin] handlers/{name}.py loaded: {', '.join(registered)}")
        else:
            print(f"[plugin] handlers/{name}.py skipped (no BRIDGE_FUNCTIONS/setup/loop)")

        self._plugins[name] = mod

    def _start_loop(self, name: str, fn: Any, interval: float) -> None:
        def _run() -> None:
            while True:
                try:
                    fn()
                except Exception as exc:
                    print(f"[plugin] handlers/{name}.py loop() ERROR: {exc}")
                time.sleep(interval)

        t = threading.Thread(target=_run, name=f"plugin-{name}", daemon=True)
        t.start()

    @property
    def loaded(self) -> Dict[str, Any]:
        return dict(self._plugins)
