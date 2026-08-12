#!/usr/bin/env python3
"""
matrixwebserver.py — Web UI for LED Matrix control, runs inside the Uno Q container.

Started as a background thread by main.py before App.run() blocks:

    import matrixwebserver
    matrixwebserver.start()          # non-blocking — starts Flask in a daemon thread
    App.run(user_loop=loop)          # blocks as normal

Access from any machine on the same network:
    http://<UnoQ-IP>:5000

The webserver shares the process with main.py and imports handlers.ledmatrix
directly — no HTTP bridging needed, no extra IP configuration.

Flask must be available in the container:
    echo 'flask' >> requirements.txt
"""

from __future__ import annotations

import json
import os
import threading
from typing import Any, Dict, List

try:
    from flask import Flask, render_template_string, request, jsonify
    HAS_FLASK = True
except ImportError:
    HAS_FLASK = False
    print("[webserver] Flask not installed — add 'flask' to requirements.txt")

try:
    from handlers import ledmatrix
    HAS_LEDMATRIX = True
except ImportError:
    HAS_LEDMATRIX = False
    print("[webserver] handlers/ledmatrix.py not found — hardware preview unavailable")

# ─── Configuration ────────────────────────────────────────────────────────────

MATRIX_ROWS   = 8
MATRIX_COLS   = 13
MATRIX_PIXELS = MATRIX_ROWS * MATRIX_COLS  # 104 on Uno Q
SERVER_PORT   = 5000
SERVER_HOST   = "0.0.0.0"   # accessible from any machine on the network
ANIMATIONS_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "matrix_animations.json")

# ─── Flask app ────────────────────────────────────────────────────────────────

if HAS_FLASK:
    app = Flask(__name__)
    app.config['JSON_SORT_KEYS'] = False

# In-memory scene store
_scenes: List[Dict[str, Any]] = []


def _normalize_scenes(raw: Any) -> List[Dict[str, Any]]:
    """Coerce legacy or malformed persisted scene data into a scene list."""
    if isinstance(raw, list):
        return [s for s in raw if isinstance(s, dict)]
    if isinstance(raw, dict):
        if isinstance(raw.get("scenes"), list):
            return [s for s in raw["scenes"] if isinstance(s, dict)]
        if all(isinstance(v, dict) for v in raw.values()):
            return [v for v in raw.values() if isinstance(v, dict)]
    return []

def _load_scenes() -> None:
    global _scenes
    if os.path.exists(ANIMATIONS_FILE):
        try:
            with open(ANIMATIONS_FILE, "r") as f:
                _scenes = _normalize_scenes(json.load(f))
            print(f"[webserver] Loaded {len(_scenes)} scenes")
            _sync_ledmatrix_scenes()
        except Exception as e:
            print(f"[webserver] Error loading scenes: {e}")

def _save_scenes() -> None:
    try:
        with open(ANIMATIONS_FILE, "w") as f:
            json.dump(_scenes, f, indent=2)
    except Exception as e:
        print(f"[webserver] Error saving scenes: {e}")


def _sync_ledmatrix_scenes() -> None:
    """Push the webserver's scene list into the ledmatrix plugin cache."""
    if not HAS_LEDMATRIX:
        return
    ledmatrix.matrix_clear()
    for i, scene in enumerate(_scenes):
        frame = scene.get("frame", [])
        if isinstance(frame, list) and _valid_frame(frame):
            ledmatrix.matrix_store_scene(_list_to_csv(frame), i)

# ─── Helpers ──────────────────────────────────────────────────────────────────

def _list_to_csv(pixels: List[int]) -> str:
    return ",".join(str(int(p)) for p in pixels)

def _valid_frame(pixels: List[int]) -> bool:
    return len(pixels) == MATRIX_PIXELS

# ─── Web UI ────────────────────────────────────────────────────────────────────

HTML_TEMPLATE = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED Matrix Controller</title>
    <style>
        .shift-pad {
            display: grid;
            grid-template-columns: repeat(3, 36px);
            grid-template-rows: repeat(3, 36px);
            gap: 3px;
        }
        .shift-btn {
            padding: 0;
            font-size: 1.1em;
            display: flex;
            align-items: center;
            justify-content: center;
            background: #333;
            border-radius: 4px;
            cursor: pointer;
            border: none;
            color: white;
        }
        .shift-btn:hover { background: #667eea; }
        .shift-center { background: transparent !important; cursor: default; }
        .file-input { display: none; }
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: #0a0e27;
            color: #e0e0e0;
            padding: 20px;
        }
        
        .container {
            max-width: 1400px;
            margin: 0 auto;
        }
        
        h1 {
            text-align: center;
            margin-bottom: 30px;
            font-size: 2.5em;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
        }
        
        .main-grid {
            display: grid;
            grid-template-columns: 1fr 350px;
            gap: 20px;
        }
        
        .matrix-panel {
            background: #1a1f3a;
            border: 2px solid #667eea;
            border-radius: 8px;
            padding: 20px;
        }
        
        .matrix-editor {
            display: grid;
            grid-template-columns: repeat(13, 1fr);
            gap: 4px;
            background: #0f1419;
            padding: 15px;
            border-radius: 6px;
        }
        
        .led {
            aspect-ratio: 1;
            border-radius: 4px;
            border: 2px solid #333;
            background: #1a1f3a;
            cursor: pointer;
            transition: all 0.1s ease;
            font-size: 0.7em;
            display: flex;
            align-items: center;
            justify-content: center;
            color: #555;
            font-weight: bold;
        }
        
        .led:hover {
            border-color: #667eea;
            transform: scale(1.05);
        }
        
        .led.active {
            background: #00d4ff;
            border-color: #00d4ff;
            box-shadow: 0 0 10px rgba(0, 212, 255, 0.5);
            color: #000;
        }
        
        .led.active[data-brightness="1"] { background: #1a4d66; }
        .led.active[data-brightness="2"] { background: #2d7a99; }
        .led.active[data-brightness="3"] { background: #40a7cc; }
        .led.active[data-brightness="4"] { background: #52d4ff; box-shadow: 0 0 8px rgba(82, 212, 255, 0.6); }
        .led.active[data-brightness="5"] { background: #5edaff; box-shadow: 0 0 10px rgba(94, 218, 255, 0.7); }
        .led.active[data-brightness="6"] { background: #6ae0ff; box-shadow: 0 0 12px rgba(106, 224, 255, 0.8); }
        .led.active[data-brightness="7"] { background: #7ce6ff; box-shadow: 0 0 15px rgba(124, 230, 255, 0.9); }
        
        .controls {
            background: #1a1f3a;
            border: 2px solid #667eea;
            border-radius: 8px;
            padding: 20px;
            display: flex;
            flex-direction: column;
            gap: 15px;
        }
        
        .control-section {
            display: flex;
            flex-direction: column;
            gap: 8px;
        }
        
        .control-section label {
            font-size: 0.9em;
            color: #aaa;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        
        .brightness-slider {
            width: 100%;
            height: 6px;
            border-radius: 3px;
            background: #333;
            outline: none;
            -webkit-appearance: none;
        }
        
        .brightness-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 18px;
            height: 18px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
            transition: background 0.2s;
        }
        
        .brightness-slider::-webkit-slider-thumb:hover {
            background: #764ba2;
        }
        
        .brightness-slider::-moz-range-thumb {
            width: 18px;
            height: 18px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
            border: none;
        }
        
        .brightness-value {
            text-align: center;
            font-size: 1.2em;
            color: #667eea;
            font-weight: bold;
        }
        
        button {
            background: #667eea;
            color: white;
            border: none;
            padding: 10px 15px;
            border-radius: 4px;
            cursor: pointer;
            font-weight: 600;
            transition: all 0.2s;
        }
        
        button:hover {
            background: #764ba2;
            transform: translateY(-2px);
        }
        
        button:active {
            transform: translateY(0);
        }
        
        .btn-secondary {
            background: #444;
        }
        
        .btn-secondary:hover {
            background: #555;
        }
        
        .scenes-list {
            display: flex;
            flex-direction: column;
            gap: 8px;
            max-height: 300px;
            overflow-y: auto;
        }
        
        .scene-item {
            background: #2a2f4a;
            border: 2px solid #444;
            padding: 8px;
            border-radius: 4px;
            cursor: pointer;
            transition: all 0.2s;
            font-size: 0.9em;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        
        .scene-item:hover {
            border-color: #667eea;
            background: #323950;
        }
        
        .scene-item.active {
            border-color: #667eea;
            background: #3a4060;
            font-weight: bold;
        }
        
        .scene-name {
            flex: 1;
        }
        
        .scene-delete {
            background: #ee6352;
            color: white;
            border: none;
            padding: 4px 8px;
            border-radius: 3px;
            cursor: pointer;
            font-size: 0.8em;
        }
        
        .scene-delete:hover {
            background: #ff4444;
        }
        
        .status {
            background: #2a2f4a;
            padding: 10px;
            border-radius: 4px;
            font-size: 0.9em;
            text-align: center;
        }
        
        .status.playing {
            color: #7ce6ff;
            font-weight: bold;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🎨 LED Matrix Controller</h1>
        
        <div class="main-grid">
            <!-- Matrix Editor -->
            <div class="matrix-panel">
                <div class="matrix-editor" id="matrixEditor"></div>
            </div>
            
            <!-- Controls -->
            <div class="controls">
                <!-- Brightness -->
                <div class="control-section">
                    <label>Brightness</label>
                    <input type="range" id="brightnessSlider" min="0" max="7" value="7" class="brightness-slider">
                    <div class="brightness-value" id="brightnessValue">7</div>
                </div>
                
                <!-- Frame Actions -->
                <div class="control-section">
                    <label>Frame Actions</label>
                    <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 8px;">
                        <button onclick="clearFrame()">Clear</button>
                        <button onclick="fillFrame()" class="btn-secondary">Fill</button>
                    </div>
                </div>

                <!-- Shift Pad -->
                <div class="control-section">
                    <label>Shift (wrap)</label>
                    <div style="display:flex; align-items:center; gap:12px;">
                        <div class="shift-pad">
                            <button class="shift-btn" onclick="shiftFrame(-1,-1)" title="Shift ↖">↖</button>
                            <button class="shift-btn" onclick="shiftFrame(-1, 0)" title="Shift ↑">↑</button>
                            <button class="shift-btn" onclick="shiftFrame(-1, 1)" title="Shift ↗">↗</button>
                            <button class="shift-btn" onclick="shiftFrame( 0,-1)" title="Shift ←">←</button>
                            <div class="shift-btn shift-center"></div>
                            <button class="shift-btn" onclick="shiftFrame( 0, 1)" title="Shift →">→</button>
                            <button class="shift-btn" onclick="shiftFrame( 1,-1)" title="Shift ↙">↙</button>
                            <button class="shift-btn" onclick="shiftFrame( 1, 0)" title="Shift ↓">↓</button>
                            <button class="shift-btn" onclick="shiftFrame( 1, 1)" title="Shift ↘">↘</button>
                        </div>
                        <label style="font-size:0.8em; color:#888;">Arrows wrap around edges</label>
                    </div>
                </div>

                <!-- Scene Management -->
                <div class="control-section">
                    <label>Scenes</label>
                    <button onclick="saveScene()">Save as Scene</button>
                    <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 8px;">
                        <button onclick="exportScenes()" class="btn-secondary">Export JSON</button>
                        <button onclick="document.getElementById('importFile').click()" class="btn-secondary">Import JSON</button>
                    </div>
                    <input type="file" id="importFile" class="file-input" accept=".json" onchange="importScenes(event)">
                    <div class="scenes-list" id="scenesList"></div>
                </div>
                
                <!-- Playback -->
                <div class="control-section">
                    <label>Playback</label>
                    <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 8px;">
                        <button onclick="playAnimation()">Play</button>
                        <button onclick="pauseAnimation()" class="btn-secondary">Pause</button>
                    </div>
                    <input type="range" id="delaySlider" min="100" max="1000" value="300" step="50" class="brightness-slider">
                    <div style="text-align: center; font-size: 0.9em; color: #aaa;">
                        Delay: <span id="delayValue">300</span>ms
                    </div>
                </div>
                
                <!-- Status -->
                <div class="status" id="status">Ready</div>
            </div>
        </div>
    </div>
    
    <script>
        const ROWS = 8;
        const COLS = 13;
        const PIXELS = ROWS * COLS;
        
        let currentBrightness = 7;
        let currentFrame = new Array(PIXELS).fill(0);
        let scenes = [];
        
        // Initialize matrix
        function initMatrix() {
            const editor = document.getElementById('matrixEditor');
            editor.innerHTML = '';
            for (let i = 0; i < PIXELS; ++i) {
                const led = document.createElement('div');
                led.className = 'led';
                led.dataset.index = i;
                led.textContent = i;
                led.addEventListener('click', () => toggleLED(i));
                led.addEventListener('contextmenu', (e) => {
                    e.preventDefault();
                    deleteLED(i);
                });
                editor.appendChild(led);
            }
        }
        
        function toggleLED(index) {
            if (currentFrame[index] === 0) {
                currentFrame[index] = currentBrightness;
            } else {
                currentFrame[index] = 0;
            }
            updateDisplay();
            previewFrame();
        }
        
        function deleteLED(index) {
            currentFrame[index] = 0;
            updateDisplay();
            previewFrame();
        }
        
        function updateDisplay() {
            for (let i = 0; i < PIXELS; ++i) {
                const led = document.querySelector(`.led[data-index="${i}"]`);
                if (currentFrame[i] > 0) {
                    led.classList.add('active');
                    led.dataset.brightness = currentFrame[i];
                } else {
                    led.classList.remove('active');
                }
            }
        }
        
        function clearFrame() {
            currentFrame = new Array(PIXELS).fill(0);
            updateDisplay();
            previewFrame();
        }
        
        function fillFrame() {
            currentFrame = new Array(PIXELS).fill(currentBrightness);
            updateDisplay();
            previewFrame();
        }

        // Shift the frame by (dRow, dCol), wrapping around edges
        function shiftFrame(dRow, dCol) {
            const next = new Array(PIXELS).fill(0);
            for (let r = 0; r < ROWS; r++) {
                for (let c = 0; c < COLS; c++) {
                    const nr = (r + dRow + ROWS) % ROWS;
                    const nc = (c + dCol + COLS) % COLS;
                    next[nr * COLS + nc] = currentFrame[r * COLS + c];
                }
            }
            currentFrame = next;
            updateDisplay();
            previewFrame();
        }
        
        function previewFrame() {
            fetch('/api/preview', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ frame: currentFrame })
            })
            .catch(e => console.error('[preview]', e));
        }
        
        function saveScene() {
            const name = prompt('Scene name:', `Scene ${scenes.length + 1}`);
            if (!name) return;
            fetch('/api/scene', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ name, frame: currentFrame })
            })
            .then(r => r.json())
            .then(data => { if (data.success) { loadScenes(); updateStatus(`Saved: ${name}`); } })
            .catch(e => console.error('[saveScene]', e));
        }

        function exportScenes() {
            fetch('/api/scenes')
            .then(r => r.json())
            .then(data => {
                const blob = new Blob([JSON.stringify({scenes: data.scenes}, null, 2)], {type: 'application/json'});
                const a = document.createElement('a');
                a.href = URL.createObjectURL(blob);
                a.download = 'matrix_animations.json';
                a.click();
                updateStatus(`Exported ${data.scenes.length} scenes`);
            })
            .catch(e => console.error('[exportScenes]', e));
        }

        function importScenes(event) {
            const file = event.target.files[0];
            if (!file) return;
            const reader = new FileReader();
            reader.onload = e => {
                try {
                    const data = JSON.parse(e.target.result);
                    // Accept either {scenes:[...]} or raw array
                    const incoming = Array.isArray(data) ? data : (data.scenes || []);
                    if (!incoming.length) { updateStatus('No scenes found in file'); return; }
                    fetch('/api/import', {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ scenes: incoming })
                    })
                    .then(r => r.json())
                    .then(res => {
                        if (res.success) { loadScenes(); updateStatus(`Imported ${res.count} scenes`); }
                    });
                } catch(err) { updateStatus('Invalid JSON file'); }
            };
            reader.readAsText(file);
            event.target.value = '';
        }
        
        function loadScenes() {
            fetch('/api/scenes')
            .then(r => r.json())
            .then(data => {
                scenes = data.scenes;
                const list = document.getElementById('scenesList');
                list.innerHTML = '';
                scenes.forEach((scene, idx) => {
                    const item = document.createElement('div');
                    item.className = 'scene-item';
                    item.innerHTML = `
                        <span class="scene-name" onclick="loadSceneByIndex(${idx})">${scene.name || 'Scene ' + idx}</span>
                        <button class="scene-delete" onclick="deleteScene(${idx})">Del</button>
                    `;
                    list.appendChild(item);
                });
            })
            .catch(e => console.error('[loadScenes]', e));
        }
        
        function loadSceneByIndex(idx) {
            fetch(`/api/scene/${idx}`)
            .then(r => r.json())
            .then(data => {
                if (data.frame) {
                    currentFrame = data.frame;
                    updateDisplay();
                    previewFrame();
                    updateStatus(`Loaded: ${data.name}`);
                }
            })
            .catch(e => console.error('[loadSceneByIndex]', e));
        }
        
        function deleteScene(idx) {
            if (!confirm('Delete this scene?')) return;
            fetch(`/api/scene/${idx}`, { method: 'DELETE' })
            .then(r => r.json())
            .then(data => {
                if (data.success) {
                    loadScenes();
                    updateStatus('Scene deleted');
                }
            })
            .catch(e => console.error('[deleteScene]', e));
        }
        
        function playAnimation() {
            fetch('/api/play', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ delay: parseInt(document.getElementById('delaySlider').value) })
            })
            .then(r => r.json())
            .then(data => {
                if (data.success) {
                    updateStatus(`Playing ${scenes.length} scenes...`);
                }
            })
            .catch(e => console.error('[playAnimation]', e));
        }
        
        function pauseAnimation() {
            fetch('/api/pause', { method: 'POST' })
            .then(r => r.json())
            .then(data => {
                if (data.success) {
                    updateStatus('Paused');
                }
            })
            .catch(e => console.error('[pauseAnimation]', e));
        }
        
        function updateStatus(msg) {
            document.getElementById('status').textContent = msg;
        }
        
        // Event listeners
        document.getElementById('brightnessSlider').addEventListener('change', (e) => {
            currentBrightness = parseInt(e.target.value);
            document.getElementById('brightnessValue').textContent = currentBrightness;
        });
        
        document.getElementById('delaySlider').addEventListener('change', (e) => {
            document.getElementById('delayValue').textContent = e.target.value;
        });
        
        // Init — auto-load scenes from server on startup
        initMatrix();
        loadScenes();
    </script>
</body>
</html>
"""

# ─── API Routes ────────────────────────────────────────────────────────────────

@app.route('/')
def index():
    """Serve the web UI."""
    return render_template_string(HTML_TEMPLATE)

@app.route('/api/preview', methods=['POST'])
def api_preview():
    data = request.get_json()
    frame = data.get('frame', [])
    if not _valid_frame(frame):
        return jsonify({"success": False, "error": "Invalid frame"}), 400
    if HAS_LEDMATRIX:
        return jsonify({"success": ledmatrix.matrix_preview(_list_to_csv(frame))})
    return jsonify({"success": True})


@app.route('/api/scene', methods=['POST'])
def api_save_scene():
    data = request.get_json()
    frame = data.get('frame', [])
    if not _valid_frame(frame):
        return jsonify({"success": False, "error": "Invalid frame"}), 400
    name = data.get('name', f'Scene {len(_scenes)}')
    _scenes.append({"name": name, "frame": frame})
    _save_scenes()
    idx = len(_scenes) - 1
    if HAS_LEDMATRIX:
        ledmatrix.matrix_store_scene(_list_to_csv(frame), idx)
    return jsonify({"success": True, "index": idx})


@app.route('/api/scene/<int:idx>', methods=['GET'])
def api_get_scene(idx):
    if idx < 0 or idx >= len(_scenes):
        return jsonify({"success": False, "error": "Scene not found"}), 404
    s = _scenes[idx]
    return jsonify({"success": True, "name": s.get("name"), "frame": s.get("frame", [])})


@app.route('/api/scene/<int:idx>', methods=['DELETE'])
def api_delete_scene(idx):
    if idx < 0 or idx >= len(_scenes):
        return jsonify({"success": False, "error": "Scene not found"}), 404
    _scenes.pop(idx)
    _save_scenes()
    _sync_ledmatrix_scenes()
    return jsonify({"success": True})


@app.route('/api/import', methods=['POST'])
def api_import_scenes():
    """Replace all scenes with an imported set."""
    data = request.get_json()
    incoming = data.get('scenes', [])
    # Accept frames as lists of ints; skip malformed entries
    valid = [s for s in incoming if isinstance(s.get('frame', None), list) and _valid_frame(s['frame'])]
    if not valid:
        return jsonify({"success": False, "error": "No valid scenes in payload"}), 400
    _scenes[:] = valid
    _save_scenes()
    _sync_ledmatrix_scenes()
    return jsonify({"success": True, "count": len(_scenes)})


@app.route('/api/scenes', methods=['GET'])
def api_get_scenes():
    return jsonify({"success": True, "scenes": _scenes})


@app.route('/api/play', methods=['POST'])
def api_play():
    data = request.get_json() or {}
    delay = int(data.get('delay', 300))
    if HAS_LEDMATRIX:
        return jsonify({"success": ledmatrix.matrix_play(delay, loop=True)})
    return jsonify({"success": True})


@app.route('/api/pause', methods=['POST'])
def api_pause():
    if HAS_LEDMATRIX:
        return jsonify({"success": ledmatrix.matrix_pause()})
    return jsonify({"success": True})


@app.route('/api/status', methods=['GET'])
def api_status():
    if HAS_LEDMATRIX:
        parts = ledmatrix.matrix_status().split(',')
        return jsonify({
            "success": True,
            "scenes":        int(parts[0]) if len(parts) > 0 else 0,
            "current_frame": int(parts[1]) if len(parts) > 1 else 0,
            "playing":       bool(int(parts[2])) if len(parts) > 2 else False,
        })
    return jsonify({"success": True, "scenes": len(_scenes), "current_frame": 0, "playing": False})


# ─── Entry points ─────────────────────────────────────────────────────────────

def start(port: int = SERVER_PORT) -> None:
    """Start Flask in a daemon thread. Call this from main.py before App.run()."""
    if not HAS_FLASK:
        print("[webserver] Cannot start — Flask not installed (add 'flask' to requirements.txt)")
        return
    _load_scenes()
    t = threading.Thread(
        target=lambda: app.run(host=SERVER_HOST, port=port, debug=False, use_reloader=False),
        daemon=True,
        name="matrixwebserver",
    )
    t.start()
    print(f"[webserver] LED Matrix Controller running on http://0.0.0.0:{port}")
    print(f"[webserver] Access from any device: http://<UnoQ-IP>:{port}")


if __name__ == '__main__':
    # Standalone mode for testing outside of App Lab
    _load_scenes()
    print(f"[webserver] Standalone mode — http://localhost:{SERVER_PORT}")
    app.run(host=SERVER_HOST, port=SERVER_PORT, debug=False, use_reloader=False)
