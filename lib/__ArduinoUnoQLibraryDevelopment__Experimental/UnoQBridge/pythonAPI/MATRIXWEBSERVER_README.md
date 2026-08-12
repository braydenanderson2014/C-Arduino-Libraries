# LED Matrix Web Controller

A Flask-based web interface for creating and managing LED matrix animations on Arduino Uno Q.

## Features

- **Interactive LED Grid** — Click to toggle individual pixels on/off
- **Brightness Levels** — 0–7 brightness control (grayscale)
- **Scene Management** — Create, save, load, and delete animation frames
- **Real-time Preview** — See changes instantly on hardware
- **Animation Playback** — Play, pause, and control animation speed
- **Persistence** — Save animations to `matrix_animations.json`
- **Hardware Integration** — Works with UnoQBridge and ledmatrix.py plugin

## Installation

### Requirements

```bash
pip install flask
```

## File Placement (MCU vs Python)

### MCU sketch (runs on board)

- Use `CPP/examples/MatrixService/MatrixService.ino` as your sketch.
- This sketch registers the `mcu_matrix_*` methods that Python calls.

### Python files (run in Uno Q container)

Copy these from `pythonAPI/` into your project `python/` folder:

- `main.py`
- `plugin_loader.py`
- `matrixwebserver.py`
- `handlers/ledmatrix.py`

Keep `handlers/` as a folder under `python/`.

### Library/header dependencies

- `LedMatrixStory.h` must be available to the sketch include path.
- `Arduino_RouterBridge.h` must be available in your App Lab/board environment.

### Startup order

1. Start Python (`main.py`) so bridge handlers/plugins are available.
2. Run/upload `MatrixService.ino` on the MCU.
3. Open the web UI on port 5000.

---

### Setup

1. Ensure `handlers/ledmatrix.py` is in your handlers folder (auto-discovered by PluginLoader)
2. Ensure `main.py` and bridge system are running
3. Run the webserver:

```bash
python matrixwebserver.py
```

4. Open in browser: **http://localhost:5000**

## Usage

### Creating Animations

1. **Click LEDs** to toggle them on/off in the matrix grid
2. Use the **Brightness slider** to set pixel brightness (0–7)
3. **Fill Frame** or **Clear Frame** for quick operations
4. **Save as Scene** to store the frame
5. Repeat to create more frames

### Managing Scenes

- **Save as Scene** — Store current frame with a name
- **Load Scene** — Retrieve a saved frame for editing
- **Scene List** — Right panel shows all saved scenes; click to load, delete button to remove

### Playing Animations

1. Create multiple scenes (frames)
2. Adjust **Delay** (ms between frames)
3. Click **Play** — animation loops through all scenes
4. Click **Pause** to stop playback

### Controls

| Action | Method |
|--------|--------|
| Toggle LED | Click |
| Delete LED (clear pixel) | Right-click |
| Adjust Brightness | Slider (0–7) |
| Fill all pixels | Fill Frame button |
| Clear all pixels | Clear Frame button |
| Save frame | Save as Scene |
| Load frame | Click scene name or Load Scene |
| Play animation | Play button |
| Pause animation | Pause button |
| Set frame delay | Delay slider |

## API Endpoints

### GET `/api/scenes`
Get all saved scenes.

**Response:**
```json
{
  "success": true,
  "scenes": [
    {"name": "All On", "frame": [1, 1, 1, ...]},
    {"name": "Checker", "frame": [1, 0, 1, ...]}
  ]
}
```

### POST `/api/scene`
Save a new scene.

**Request:**
```json
{
  "name": "My Scene",
  "frame": [0, 1, 0, 1, ...]  // 104 values for Uno Q
}
```

### GET `/api/scene/<idx>`
Get a specific scene.

**Response:**
```json
{
  "success": true,
  "name": "My Scene",
  "frame": [0, 1, 0, ...]
}
```

### DELETE `/api/scene/<idx>`
Delete a scene.

### POST `/api/preview`
Preview a frame on hardware (no save).

**Request:**
```json
{
  "frame": [0, 1, 0, 1, ...]
}
```

### POST `/api/play`
Start animation playback.

**Request:**
```json
{
  "delay": 300,
  "wrap": true,
  "count": 2
}
```

`wrap/count` are optional. When enabled, playback repeats scenes for `count` loops.

### POST `/api/wrap`
Save playback wrap settings.

**Request:**
```json
{
  "enabled": true,
  "count": 3
}
```

### POST `/api/pause`
Pause animation playback.

### GET `/api/status`
Get current playback status.

**Response:**
```json
{
  "success": true,
  "scenes": 5,
  "current_frame": 2,
  "playing": true
}
```

## File Format

Animations are stored in `matrix_animations.json`:

```json
{
  "scenes": [
    {
      "name": "All On",
      "frame": [1, 1, 1, 1, ..., 1]  // 104 pixels
    },
    {
      "name": "Checkerboard",
      "frame": [1, 0, 1, 0, ..., 0]
    }
  ],
  "wrap": {
    "enabled": true,
    "count": 2
  }
}
```

When wrap is enabled, export may include a loop marker scene:

```json
{ "kind": "loop", "count": 2 }
```

## Pixel Values

Each pixel is 0–7 (grayscale brightness):
- **0** = Off
- **1–7** = Increasing brightness (7 = full on)

Uno Q: 8×13 grid = 104 pixels  
Uno R4 WiFi: 8×12 grid = 96 pixels (adjust `MATRIX_PIXELS` in code)

## Troubleshooting

### "No hardware" message
- Ensure `handlers/ledmatrix.py` is in the handlers folder
- Check that the sketch (BridgeConnectivityTest.ino or MatrixStoryDemo.ino) is loaded with matrix support
- Verify Bridge connection is active

### Changes not syncing to hardware
- Check that `main.py` is running and Python is connected to the MCU
- Verify the ledmatrix plugin is loaded (check console output)
- Try resetting the Bridge connection

### Animation not playing
- Ensure at least 2 scenes are created
- Check animation delay is within 100–1000 ms
- Verify MCU has `mcu_matrix_*` functions registered

## Development

To extend the webserver:

1. **Add new UI controls** — Edit `HTML_TEMPLATE` in matrixwebserver.py
2. **Add API endpoints** — Create new Flask routes
3. **Modify storage** — Edit `save_animations()` and `load_animations()`
4. **Support new hardware** — Change `MATRIX_ROWS`, `MATRIX_COLS`, `MATRIX_PIXELS`

## License

Part of the Arduino_RouterBridge ecosystem.
