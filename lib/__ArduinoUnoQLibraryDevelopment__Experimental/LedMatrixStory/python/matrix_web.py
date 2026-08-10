#!/usr/bin/env python3
"""
matrix_web.py — Self-hosted web UI for LedMatrixStory.

Runs a Flask HTTP server (port 5050) in a background thread alongside
the App Lab Bridge. The browser shows an interactive 8×13 grid editor;
all commands reach the MCU via Bridge.call().

Usage in main.py:
    from matrix_web import MatrixWebUI, register_matrix_bridge
    _mweb = MatrixWebUI()
    register_matrix_bridge(_mweb)   # adds Bridge.provide() callbacks
    _mweb.start()                   # starts Flask thread (non-blocking)

Then in App.run() loop, nothing extra is needed — Flask runs in its own thread.

Open  http://<board-ip>:5050  in a browser on the same network.
"""

from __future__ import annotations

import json
import threading
from typing import Any, List, Optional

# Flask is not pre-installed in the App Lab container.
# Add 'flask' to python/requirements.txt and it will be installed on next Run.
try:
    from flask import Flask, request, jsonify, Response
    _FLASK_AVAILABLE = True
except ImportError:  # pragma: no cover
    _FLASK_AVAILABLE = False

MATRIX_ROWS = 8
MATRIX_COLS = 13
MATRIX_PIXELS = MATRIX_ROWS * MATRIX_COLS

# ─── HTML/JS frontend ────────────────────────────────────────────────────────
# Single-file SPA served from /; no external CDN needed.

_HTML = r"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>LED Matrix Story Editor</title>
<style>
  body{font-family:monospace;background:#111;color:#ccc;margin:0;padding:16px}
  h1{color:#4af;margin:0 0 12px}
  #grid{display:grid;grid-template-columns:repeat(13,28px);gap:3px;margin-bottom:12px}
  .cell{width:28px;height:28px;border-radius:4px;background:#222;cursor:pointer;
        border:1px solid #333;display:flex;align-items:center;justify-content:center;
        font-size:10px;color:#555}
  .cell.on{background:#00f;border-color:#44f;color:#aaf}
  .controls{display:flex;gap:8px;flex-wrap:wrap;margin-bottom:12px}
  button{background:#333;color:#ccc;border:1px solid #555;padding:6px 12px;
         border-radius:4px;cursor:pointer;font-family:monospace}
  button:hover{background:#444;color:#fff}
  button.primary{background:#135;color:#4af;border-color:#4af}
  select,input[type=range],input[type=number]{background:#222;color:#ccc;
         border:1px solid #444;padding:4px;border-radius:4px;font-family:monospace}
  #scenes{display:flex;gap:6px;flex-wrap:wrap;margin-bottom:12px}
  .scene-btn{padding:4px 8px;font-size:11px}
  .scene-btn.active{background:#135;color:#4af;border-color:#4af}
  #status{color:#888;font-size:12px;margin-top:8px}
  #brightness-row{display:flex;align-items:center;gap:8px;margin-bottom:8px}
  label{font-size:12px}
</style>
</head>
<body>
<h1>🔵 LED Matrix Story Editor</h1>
<div id="brightness-row">
  <label>Pixel brightness:</label>
  <input type="range" id="bright" min="0" max="7" value="7" step="1">
  <span id="bright-val">7</span>
  <label style="margin-left:16px">Grayscale bits:</label>
  <select id="gbits">
    <option value="1">1-bit (on/off)</option>
    <option value="3" selected>3-bit (8 levels)</option>
    <option value="8">8-bit (256 levels)</option>
  </select>
</div>
<div id="grid"></div>
<div class="controls">
  <button onclick="clearGrid()">Clear</button>
  <button onclick="fillGrid()">Fill</button>
  <button onclick="invertGrid()">Invert</button>
  <button onclick="previewFrame()">Preview on Matrix</button>
  <button class="primary" onclick="addScene()">+ Add Scene</button>
  <button onclick="replaceScene()">Replace Current</button>
  <button onclick="removeScene()">Remove Current</button>
</div>
<div class="controls">
  <button class="primary" onclick="playStory()">▶ Play</button>
  <button onclick="pauseStory()">⏸ Pause</button>
  <button onclick="stopStory()">⏹ Stop</button>
  <button onclick="nextFrame()">▶| Next</button>
  <button onclick="prevFrame()">|◀ Prev</button>
  <label>Delay (ms):</label>
  <input type="number" id="delay" value="200" min="50" max="5000" style="width:70px">
  <button onclick="setDelay()">Set</button>
</div>
<div>
  <label>Scenes:</label>
  <div id="scenes"></div>
</div>
<div id="status">Ready.</div>
<script>
const ROWS=8, COLS=13, PX=ROWS*COLS;
let pixels = new Array(PX).fill(0);
let scenes = [];      // array of {name, pixels[]}
let currentScene = 0;
let activeCellBrightness = 7;

// Build grid
const grid = document.getElementById('grid');
for(let i=0;i<PX;i++){
  const d=document.createElement('div');
  d.className='cell';
  d.id='c'+i;
  d.textContent=0;
  d.addEventListener('mousedown', ()=>toggleCell(i));
  d.addEventListener('mouseover', e=>{ if(e.buttons) toggleCell(i); });
  grid.appendChild(d);
}

document.getElementById('bright').oninput = e => {
  activeCellBrightness = parseInt(e.target.value);
  document.getElementById('bright-val').textContent = activeCellBrightness;
};

function toggleCell(i){
  pixels[i] = pixels[i] > 0 ? 0 : activeCellBrightness;
  updateCell(i);
}
function updateCell(i){
  const d=document.getElementById('c'+i);
  d.textContent = pixels[i] > 0 ? pixels[i] : '';
  d.className='cell'+(pixels[i]>0?' on':'');
  d.style.background = pixels[i] > 0
    ? `rgb(0,0,${Math.round(pixels[i]/7*255)})` : '#222';
}
function renderGrid(px){ pixels=px.slice(); for(let i=0;i<PX;i++) updateCell(i); }
function clearGrid(){ renderGrid(new Array(PX).fill(0)); }
function fillGrid(){
  renderGrid(new Array(PX).fill(activeCellBrightness||1));
}
function invertGrid(){
  const mx = parseInt(document.getElementById('gbits').value==='1'?1:7);
  renderGrid(pixels.map(v=>v>0?0:mx));
}

function csvOf(px){ return px.join(','); }
function status(msg){ document.getElementById('status').textContent = msg; }

async function api(path, body=null){
  const opts = body ? {method:'POST',headers:{'Content-Type':'application/json'},
                       body:JSON.stringify(body)} : {method:'GET'};
  const r = await fetch(path, opts);
  return r.json();
}

async function previewFrame(){
  const r = await api('/preview', {pixels: csvOf(pixels)});
  status(r.ok ? 'Previewing on matrix.' : 'Error: '+r.error);
}

async function addScene(){
  scenes.push({name:'scene_'+scenes.length, pixels:pixels.slice()});
  currentScene = scenes.length-1;
  await syncScenes();
  renderSceneTabs();
  status('Added scene '+currentScene);
}

async function replaceScene(){
  if(scenes.length===0) return addScene();
  scenes[currentScene].pixels = pixels.slice();
  await syncScenes();
  status('Replaced scene '+currentScene);
}

async function removeScene(){
  if(scenes.length===0) return;
  scenes.splice(currentScene,1);
  currentScene = Math.max(0, currentScene-1);
  await syncScenes();
  renderSceneTabs();
  status('Removed scene');
}

async function syncScenes(){
  // Upload all scenes to the server (which relays to MCU)
  const r = await api('/story', {scenes: scenes.map(s=>({name:s.name,pixels:csvOf(s.pixels)}))});
  status(r.ok ? 'Synced '+scenes.length+' scene(s) to MCU.' : 'Sync error: '+r.error);
}

function renderSceneTabs(){
  const el=document.getElementById('scenes');
  el.innerHTML='';
  scenes.forEach((s,i)=>{
    const b=document.createElement('button');
    b.className='scene-btn'+(i===currentScene?' active':'');
    b.textContent=s.name||('scene_'+i);
    b.onclick=()=>loadScene(i);
    el.appendChild(b);
  });
}

function loadScene(i){
  currentScene=i;
  renderGrid(scenes[i].pixels);
  renderSceneTabs();
  status('Loaded scene '+i);
}

async function playStory(){
  const delay=parseInt(document.getElementById('delay').value)||200;
  const r=await api('/play',{delay,loop:true});
  status(r.ok?'Playing.':'Error: '+r.error);
}
async function pauseStory(){ const r=await api('/pause'); status(r.ok?'Paused.':'Error'); }
async function stopStory(){ const r=await api('/stop'); status(r.ok?'Stopped.':'Error'); }
async function nextFrame(){ const r=await api('/next'); status(r.ok?'Next frame.':'Error'); }
async function prevFrame(){ const r=await api('/prev'); status(r.ok?'Prev frame.':'Error'); }
async function setDelay(){
  const ms=parseInt(document.getElementById('delay').value)||200;
  const r=await api('/delay',{ms});
  status(r.ok?'Delay set to '+ms+'ms':'Error');
}

// Poll MCU info every 2 s
setInterval(async()=>{
  try{
    const r=await api('/info');
    if(r.ok) status('MCU: '+r.scenes+' scene(s), frame='+r.frame+(r.playing?' ▶':' ⏸'));
  }catch(_){}
}, 2000);
</script>
</body>
</html>"""


# ─── MatrixWebUI ──────────────────────────────────────────────────────────────

class MatrixWebUI:
    """Flask web server for the LED Matrix Story Editor."""

    def __init__(self, port: int = 5050) -> None:
        self.port = port
        self._bridge_call = None   # injected by register_matrix_bridge()
        self._bridge_notify = None
        self._app: Optional[Any] = None
        self._thread: Optional[threading.Thread] = None

    def _call(self, method: str, *args) -> Any:
        """Call an MCU Bridge function if available."""
        if self._bridge_call:
            try:
                return self._bridge_call(method, *args)
            except Exception as exc:
                print(f"[matrix_web] Bridge.call({method!r}) ERROR: {exc}")
        return None

    def _build_app(self) -> Any:
        if not _FLASK_AVAILABLE:
            raise RuntimeError(
                "Flask not installed. Add 'flask' to python/requirements.txt and re-run."
            )
        app = Flask(__name__)

        @app.get("/")
        def index():
            return Response(_HTML, mimetype="text/html")

        @app.post("/preview")
        def preview():
            data = request.get_json(force=True)
            csv = data.get("pixels", "")
            result = self._call("matrix_preview", csv)
            return jsonify({"ok": result is not False})

        @app.post("/story")
        def story():
            data = request.get_json(force=True)
            scenes: List[dict] = data.get("scenes", [])
            self._call("matrix_clear", "")
            ok = True
            for i, s in enumerate(scenes):
                r = self._call("matrix_load_scene", s.get("pixels", ""), i)
                if r is False:
                    ok = False
            return jsonify({"ok": ok, "count": len(scenes)})

        @app.post("/play")
        def play():
            data = request.get_json(force=True)
            delay = int(data.get("delay", 200))
            loop = bool(data.get("loop", True))
            r = self._call("matrix_play", delay, loop)
            return jsonify({"ok": r is not False})

        @app.post("/pause")
        def pause():
            r = self._call("matrix_pause", "")
            return jsonify({"ok": r is not False})

        @app.post("/stop")
        def stop():
            r = self._call("matrix_stop", "")
            return jsonify({"ok": r is not False})

        @app.post("/next")
        def next_frame():
            r = self._call("matrix_next", "")
            return jsonify({"ok": r is not False})

        @app.post("/prev")
        def prev_frame():
            r = self._call("matrix_prev", "")
            return jsonify({"ok": r is not False})

        @app.post("/delay")
        def set_delay():
            data = request.get_json(force=True)
            ms = int(data.get("ms", 200))
            r = self._call("matrix_set_delay", ms)
            return jsonify({"ok": r is not False})

        @app.get("/info")
        def info():
            raw = self._call("matrix_get_info", "")
            if raw and "," in str(raw):
                parts = str(raw).split(",")
                return jsonify({
                    "ok": True,
                    "scenes":  int(parts[0]) if parts[0].isdigit() else 0,
                    "frame":   int(parts[1]) if len(parts) > 1 and parts[1].isdigit() else 0,
                    "playing": parts[2] == "1" if len(parts) > 2 else False,
                })
            return jsonify({"ok": False, "scenes": 0, "frame": 0, "playing": False})

        return app

    def start(self) -> None:
        """Start Flask in a background daemon thread."""
        if not _FLASK_AVAILABLE:
            print("[matrix_web] Flask not available — add 'flask' to requirements.txt")
            return
        self._app = self._build_app()

        def _run():
            print(f"[matrix_web] Web UI running on http://0.0.0.0:{self.port}")
            import logging
            log = logging.getLogger("werkzeug")
            log.setLevel(logging.WARNING)
            self._app.run(host="0.0.0.0", port=self.port, threaded=True)

        self._thread = threading.Thread(target=_run, name="matrix-web", daemon=True)
        self._thread.start()


def register_matrix_bridge(web_ui: MatrixWebUI, bridge_obj: Any) -> None:
    """
    Inject Bridge.call into the web UI so HTTP requests reach the MCU.

    Call this in main.py after Bridge.provide() registrations:

        from matrix_web import MatrixWebUI, register_matrix_bridge
        from arduino.app_utils import Bridge

        _mweb = MatrixWebUI(port=5050)
        register_matrix_bridge(_mweb, Bridge)
        _mweb.start()
    """
    web_ui._bridge_call = bridge_obj.call
