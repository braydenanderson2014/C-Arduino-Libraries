#pragma once
/*
 * LedMatrixStory.h
 *
 * Story/Scene/Animation library for Arduino boards with a built-in LED matrix.
 * Automatically targets the correct hardware based on the board being compiled.
 *
 * ═══════════════════════════════════════════════════════════════════
 * SUPPORTED BOARDS
 * ═══════════════════════════════════════════════════════════════════
 *
 *  Arduino Uno Q   — 8 × 13 blue matrix, grayscale (1/3/8-bit), draw()
 *  Arduino Uno R4 WiFi — 8 × 12 red matrix, binary only, renderBitmap()
 *
 *  Functions locked on Uno R4 (compile-time stub or no-op):
 *    setGrayscaleBits()  — R4 only supports on/off; calling this is a no-op
 *                          and triggers a compile-time warning.
 *
 * ═══════════════════════════════════════════════════════════════════
 * QUICK START
 * ═══════════════════════════════════════════════════════════════════
 *
 *   #include <Arduino_LED_Matrix.h>
 *   #include "LedMatrixStory.h"
 *
 *   Arduino_LED_Matrix hw;
 *   LedMatrixStory story(hw);
 *
 *   void setup() {
 *       hw.begin();
 *   #if LEDMATRIX_HAS_GRAYSCALE
 *       story.setGrayscaleBits(3);   // Uno Q only — 8 brightness levels
 *   #endif
 *
 *       LedMatrixScene s1, s2;
 *       s1.fill(1);
 *       s2.setRow(0, LEDMATRIX_MAX_BRIGHTNESS);
 *
 *       story.addScene(s1);
 *       story.addScene(s2);
 *       story.play(300);
 *   }
 *
 *   void loop() {
 *       story.update();
 *   }
 */

#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

// ─── Board detection ──────────────────────────────────────────────────────────

#if defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_UNOWIFIR4)
    #define LEDMATRIX_TARGET_R4       1
    #define LEDMATRIX_HAS_GRAYSCALE   0   // R4: binary LEDs only
    #define LEDMATRIX_MAX_BRIGHTNESS  1   // 0 = off, 1 = on
#elif defined(ARDUINO_UNO_Q) || defined(ARDUINO_ARCH_STM32)
    #define LEDMATRIX_TARGET_UNO_Q    1
    #define LEDMATRIX_HAS_GRAYSCALE   1   // Uno Q: 1/3/8-bit grayscale
    #define LEDMATRIX_MAX_BRIGHTNESS  7   // default 3-bit max; 255 if using 8-bit
#else
    // Unknown target — assume Uno Q behaviour; override by defining your own
    #define LEDMATRIX_TARGET_UNKNOWN  1
    #define LEDMATRIX_HAS_GRAYSCALE   0
    #define LEDMATRIX_MAX_BRIGHTNESS  1
    #warning "LedMatrixStory: unknown board — defaulting to binary (on/off) mode."
#endif

// ─── Matrix dimensions ────────────────────────────────────────────────────────

#if LEDMATRIX_TARGET_R4
    static constexpr uint8_t MATRIX_ROWS   = 8;
    static constexpr uint8_t MATRIX_COLS   = 12;
#else
    static constexpr uint8_t MATRIX_ROWS   = 8;
    static constexpr uint8_t MATRIX_COLS   = 13;
#endif

static constexpr uint8_t MATRIX_PIXELS = MATRIX_ROWS * MATRIX_COLS;
// R4: 96 pixels   Uno Q: 104 pixels

// ─── LedMatrixScene ──────────────────────────────────────────────────────────

class LedMatrixScene {
public:
    uint8_t pixels[MATRIX_PIXELS] = {};
    char    name[32]              = "untitled";

    LedMatrixScene() = default;

    // ── Constructors / factories ────────────────────────────────────────────

    static LedMatrixScene fromArray(const uint8_t* data, uint8_t len) {
        LedMatrixScene s;
        uint8_t n = (len < MATRIX_PIXELS) ? len : MATRIX_PIXELS;
        for (uint8_t i = 0; i < n; ++i) s.pixels[i] = data[i];
        return s;
    }

    // Parse "v0,v1,...,vN" (N = MATRIX_PIXELS-1) for Bridge string payloads.
    // Values are clamped to LEDMATRIX_MAX_BRIGHTNESS on R4.
    static LedMatrixScene fromString(const char* csv) {
        LedMatrixScene s;
        uint8_t idx = 0;
        const char* p = csv;
        while (*p && idx < MATRIX_PIXELS) {
            uint8_t v = 0;
            while (*p >= '0' && *p <= '9') {
                v = static_cast<uint8_t>(v * 10 + (*p - '0'));
                ++p;
            }
#if LEDMATRIX_TARGET_R4
            v = (v > 0) ? 1 : 0;   // clamp to binary on R4
#endif
            s.pixels[idx++] = v;
            if (*p == ',') ++p;
        }
        return s;
    }

    static LedMatrixScene fromString(const String& csv) {
        return fromString(csv.c_str());
    }

    // ── Pixel access ────────────────────────────────────────────────────────

    void setPixel(uint8_t row, uint8_t col, uint8_t brightness) {
        if (row < MATRIX_ROWS && col < MATRIX_COLS) {
#if LEDMATRIX_TARGET_R4
            brightness = (brightness > 0) ? 1 : 0;
#endif
            pixels[row * MATRIX_COLS + col] = brightness;
        }
    }

    uint8_t getPixel(uint8_t row, uint8_t col) const {
        if (row < MATRIX_ROWS && col < MATRIX_COLS)
            return pixels[row * MATRIX_COLS + col];
        return 0;
    }

    // ── Bulk drawing helpers ────────────────────────────────────────────────

    void fill(uint8_t brightness = 0) {
#if LEDMATRIX_TARGET_R4
        brightness = (brightness > 0) ? 1 : 0;
#endif
        for (uint8_t i = 0; i < MATRIX_PIXELS; ++i) pixels[i] = brightness;
    }

    void clear() { fill(0); }

    void setRow(uint8_t row, uint8_t brightness) {
        if (row >= MATRIX_ROWS) return;
#if LEDMATRIX_TARGET_R4
        brightness = (brightness > 0) ? 1 : 0;
#endif
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            pixels[row * MATRIX_COLS + c] = brightness;
    }

    void setCol(uint8_t col, uint8_t brightness) {
        if (col >= MATRIX_COLS) return;
#if LEDMATRIX_TARGET_R4
        brightness = (brightness > 0) ? 1 : 0;
#endif
        for (uint8_t r = 0; r < MATRIX_ROWS; ++r)
            pixels[r * MATRIX_COLS + col] = brightness;
    }

    void setRect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1, uint8_t brightness) {
#if LEDMATRIX_TARGET_R4
        brightness = (brightness > 0) ? 1 : 0;
#endif
        for (uint8_t r = r0; r <= r1 && r < MATRIX_ROWS; ++r)
            for (uint8_t c = c0; c <= c1 && c < MATRIX_COLS; ++c)
                pixels[r * MATRIX_COLS + c] = brightness;
    }

    void invert(uint8_t maxBrightness = LEDMATRIX_MAX_BRIGHTNESS) {
        for (uint8_t i = 0; i < MATRIX_PIXELS; ++i)
            pixels[i] = (pixels[i] > 0) ? 0 : maxBrightness;
    }

    void shiftLeft(bool wrap = false) {
        for (uint8_t r = 0; r < MATRIX_ROWS; ++r) {
            uint8_t first = pixels[r * MATRIX_COLS];
            for (uint8_t c = 0; c < MATRIX_COLS - 1; ++c)
                pixels[r * MATRIX_COLS + c] = pixels[r * MATRIX_COLS + c + 1];
            pixels[r * MATRIX_COLS + MATRIX_COLS - 1] = wrap ? first : 0;
        }
    }

    void shiftRight(bool wrap = false) {
        for (uint8_t r = 0; r < MATRIX_ROWS; ++r) {
            uint8_t last = pixels[r * MATRIX_COLS + MATRIX_COLS - 1];
            for (uint8_t c = MATRIX_COLS - 1; c > 0; --c)
                pixels[r * MATRIX_COLS + c] = pixels[r * MATRIX_COLS + c - 1];
            pixels[r * MATRIX_COLS] = wrap ? last : 0;
        }
    }

    void shiftUp(bool wrap = false) {
        uint8_t tmp[MATRIX_COLS];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c) tmp[c] = pixels[c];
        for (uint8_t r = 0; r < MATRIX_ROWS - 1; ++r)
            for (uint8_t c = 0; c < MATRIX_COLS; ++c)
                pixels[r * MATRIX_COLS + c] = pixels[(r + 1) * MATRIX_COLS + c];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            pixels[(MATRIX_ROWS - 1) * MATRIX_COLS + c] = wrap ? tmp[c] : 0;
    }

    void shiftDown(bool wrap = false) {
        uint8_t tmp[MATRIX_COLS];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            tmp[c] = pixels[(MATRIX_ROWS - 1) * MATRIX_COLS + c];
        for (uint8_t r = MATRIX_ROWS - 1; r > 0; --r)
            for (uint8_t c = 0; c < MATRIX_COLS; ++c)
                pixels[r * MATRIX_COLS + c] = pixels[(r - 1) * MATRIX_COLS + c];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            pixels[c] = wrap ? tmp[c] : 0;
    }

    String toString() const {
        String s;
        s.reserve(MATRIX_PIXELS * 3);
        for (uint8_t i = 0; i < MATRIX_PIXELS; ++i) {
            s += String(pixels[i]);
            if (i < MATRIX_PIXELS - 1) s += ',';
        }
        return s;
    }
};

// ─── LedMatrixStory ──────────────────────────────────────────────────────────

class LedMatrixStory {
public:
    static constexpr uint8_t MAX_SCENES = 64;

    explicit LedMatrixStory(Arduino_LED_Matrix& matrix)
        : _matrix(matrix) {}

    // ── Scene management ────────────────────────────────────────────────────

    bool addScene(const LedMatrixScene& scene) {
        if (_count >= MAX_SCENES) return false;
        _scenes[_count++] = scene;
        return true;
    }

    bool insertScene(uint8_t index, const LedMatrixScene& scene) {
        if (_count >= MAX_SCENES || index > _count) return false;
        for (uint8_t i = _count; i > index; --i) _scenes[i] = _scenes[i - 1];
        _scenes[index] = scene;
        ++_count;
        if (_currentFrame >= index) ++_currentFrame;
        return true;
    }

    bool removeScene(uint8_t index) {
        if (index >= _count) return false;
        for (uint8_t i = index; i < _count - 1; ++i) _scenes[i] = _scenes[i + 1];
        --_count;
        if (_currentFrame >= _count && _count > 0) _currentFrame = _count - 1;
        return true;
    }

    bool setScene(uint8_t index, const LedMatrixScene& scene) {
        if (index == _count) return addScene(scene);
        if (index >= _count) return false;
        _scenes[index] = scene;
        return true;
    }

    LedMatrixScene* getScene(uint8_t index) {
        return (index < _count) ? &_scenes[index] : nullptr;
    }

    uint8_t sceneCount() const { return _count; }
    void    clearAll()         { _count = 0; _currentFrame = 0; _playing = false; }

    // ── Playback ────────────────────────────────────────────────────────────

    void play(unsigned long frameDelayMs = 200, bool loop = true) {
        if (_count == 0) return;
        _frameDelay    = frameDelayMs;
        _loop          = loop;
        _playing       = true;
        _lastFrameTime = millis();
        _render(_scenes[_currentFrame]);
    }

    void pause() { _playing = false; }

    void stop() {
        _playing      = false;
        _currentFrame = 0;
        if (_count > 0) _render(_scenes[0]);
    }

    void nextFrame() {
        if (_count == 0) return;
        _currentFrame = (_currentFrame + 1) % _count;
        _render(_scenes[_currentFrame]);
        _lastFrameTime = millis();
    }

    void prevFrame() {
        if (_count == 0) return;
        _currentFrame = (_currentFrame == 0) ? _count - 1 : _currentFrame - 1;
        _render(_scenes[_currentFrame]);
        _lastFrameTime = millis();
    }

    void gotoFrame(uint8_t index) {
        if (index >= _count) return;
        _currentFrame = index;
        _render(_scenes[_currentFrame]);
        _lastFrameTime = millis();
    }

    void previewScene(uint8_t index) {
        if (index < _count) _render(_scenes[index]);
    }

    void previewScene(const LedMatrixScene& scene) { _render(scene); }

    void blank() {
        LedMatrixScene empty;
        _render(empty);
    }

    // ── Must be called in loop() ─────────────────────────────────────────────

    void update() {
        if (!_playing || _count == 0) return;
        if (millis() - _lastFrameTime >= _frameDelay) {
            _lastFrameTime = millis();
            if (++_currentFrame >= _count) {
                if (_loop) _currentFrame = 0;
                else       { _currentFrame = _count - 1; _playing = false; return; }
            }
            _render(_scenes[_currentFrame]);
        }
    }

    // ── Settings ─────────────────────────────────────────────────────────────

    void setFrameDelay(unsigned long ms) { _frameDelay = ms; }
    void setLoop(bool loop)              { _loop = loop; }

    // Grayscale — Uno Q only. Calling on R4 triggers a compile-time warning
    // and is silently ignored at runtime so cross-platform code still compiles.
#if LEDMATRIX_HAS_GRAYSCALE
    void setGrayscaleBits(uint8_t bits) { _matrix.setGrayscaleBits(bits); }
#else
    // Suppress: "unused parameter 'bits'" warning with (void)bits
    [[deprecated("setGrayscaleBits() is not supported on this board (binary LEDs only).")]]
    void setGrayscaleBits(uint8_t bits) { (void)bits; }
#endif

    // ── State ─────────────────────────────────────────────────────────────────

    uint8_t       currentFrame() const { return _currentFrame; }
    bool          isPlaying()    const { return _playing; }
    unsigned long frameDelay()   const { return _frameDelay; }

    // Board capability query — use instead of #ifdef in sketch code
    static constexpr bool hasGrayscale()   { return LEDMATRIX_HAS_GRAYSCALE; }
    static constexpr uint8_t maxBrightness() { return LEDMATRIX_MAX_BRIGHTNESS; }
    static constexpr uint8_t rows()        { return MATRIX_ROWS; }
    static constexpr uint8_t cols()        { return MATRIX_COLS; }
    static constexpr uint8_t pixels()      { return MATRIX_PIXELS; }

private:
    Arduino_LED_Matrix& _matrix;
    LedMatrixScene      _scenes[MAX_SCENES];
    uint8_t             _count         = 0;
    uint8_t             _currentFrame  = 0;
    bool                _playing       = false;
    bool                _loop          = true;
    unsigned long       _frameDelay    = 200;
    unsigned long       _lastFrameTime = 0;

    void _render(const LedMatrixScene& scene) {
#if LEDMATRIX_TARGET_R4
        // R4: renderBitmap expects a non-const pointer; local copy is needed
        uint8_t buf[MATRIX_PIXELS];
        memcpy(buf, scene.pixels, MATRIX_PIXELS);
        _matrix.renderBitmap(buf, MATRIX_ROWS, MATRIX_COLS);
#else
        _matrix.draw(scene.pixels);
#endif
    }
};

 *
 * ═══════════════════════════════════════════════════════════════════
 * CONCEPTS
 * ═══════════════════════════════════════════════════════════════════
 *
 * LedMatrixScene  — one frame: 8×13 pixels, each 0-255 brightness.
 *                   Stored as uint8_t[104]; the matrix driver maps
 *                   values to however many grayscale bits you set.
 *
 * LedMatrixStory  — ordered collection of scenes + playback engine.
 *                   Call update() from loop() for timed animation.
 *
 * ═══════════════════════════════════════════════════════════════════
 * QUICK START
 * ═══════════════════════════════════════════════════════════════════
 *
 *   #include <Arduino_LED_Matrix.h>
 *   #include "LedMatrixStory.h"
 *
 *   Arduino_LED_Matrix hw;
 *   LedMatrixStory story(hw);
 *
 *   void setup() {
 *       hw.begin();
 *       hw.setGrayscaleBits(3);   // 8 brightness levels
 *
 *       LedMatrixScene s1, s2;
 *       s1.fill(1);               // dim glow
 *       s2.setRow(0, 7);          // top row full brightness
 *       s2.setRow(7, 7);          // bottom row full brightness
 *
 *       story.addScene(s1);
 *       story.addScene(s2);
 *       story.play(300);          // 300 ms per frame, loops
 *   }
 *
 *   void loop() {
 *       story.update();
 *   }
 *
 * ═══════════════════════════════════════════════════════════════════
 * BRIDGE INTEGRATION (Python → sketch)
 * ═══════════════════════════════════════════════════════════════════
 *
 *   Python sends a scene as a comma-separated string of 104 values
 *   (0-255) and the sketch adds or replaces it:
 *
 *   Bridge.provide_safe("matrix_load_scene",  matrix_load_scene);
 *   Bridge.provide_safe("matrix_play",        matrix_play_cmd);
 *   Bridge.provide_safe("matrix_pause",       matrix_pause_cmd);
 *   Bridge.provide_safe("matrix_next",        matrix_next_cmd);
 *   Bridge.provide_safe("matrix_clear",       matrix_clear_cmd);
 *   Bridge.provide_safe("matrix_set_delay",   matrix_set_delay);
 *
 *   See MatrixStoryDemo.ino for the full sketch integration.
 */

#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

// ─── Matrix dimensions ────────────────────────────────────────────────────────

static constexpr uint8_t  MATRIX_ROWS   = 8;
static constexpr uint8_t  MATRIX_COLS   = 13;
static constexpr uint8_t  MATRIX_PIXELS = MATRIX_ROWS * MATRIX_COLS;  // 104

// ─── LedMatrixScene ──────────────────────────────────────────────────────────

class LedMatrixScene {
public:
    uint8_t pixels[MATRIX_PIXELS] = {};
    char    name[32]              = "untitled";

    LedMatrixScene() = default;

    // ── Constructors / factories ────────────────────────────────────────────

    static LedMatrixScene fromArray(const uint8_t* data, uint8_t len) {
        LedMatrixScene s;
        uint8_t n = (len < MATRIX_PIXELS) ? len : MATRIX_PIXELS;
        for (uint8_t i = 0; i < n; ++i) s.pixels[i] = data[i];
        return s;
    }

    // Parse "v0,v1,...,v103" — used for Bridge string payloads
    static LedMatrixScene fromString(const char* csv) {
        LedMatrixScene s;
        uint8_t idx = 0;
        const char* p = csv;
        while (*p && idx < MATRIX_PIXELS) {
            uint8_t v = 0;
            while (*p >= '0' && *p <= '9') {
                v = static_cast<uint8_t>(v * 10 + (*p - '0'));
                ++p;
            }
            s.pixels[idx++] = v;
            if (*p == ',') ++p;
        }
        return s;
    }

    static LedMatrixScene fromString(const String& csv) {
        return fromString(csv.c_str());
    }

    // ── Pixel access ────────────────────────────────────────────────────────

    void setPixel(uint8_t row, uint8_t col, uint8_t brightness) {
        if (row < MATRIX_ROWS && col < MATRIX_COLS)
            pixels[row * MATRIX_COLS + col] = brightness;
    }

    uint8_t getPixel(uint8_t row, uint8_t col) const {
        if (row < MATRIX_ROWS && col < MATRIX_COLS)
            return pixels[row * MATRIX_COLS + col];
        return 0;
    }

    // ── Bulk drawing helpers ────────────────────────────────────────────────

    void fill(uint8_t brightness = 0) {
        for (uint8_t i = 0; i < MATRIX_PIXELS; ++i) pixels[i] = brightness;
    }

    void clear() { fill(0); }

    void setRow(uint8_t row, uint8_t brightness) {
        if (row >= MATRIX_ROWS) return;
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            pixels[row * MATRIX_COLS + c] = brightness;
    }

    void setCol(uint8_t col, uint8_t brightness) {
        if (col >= MATRIX_COLS) return;
        for (uint8_t r = 0; r < MATRIX_ROWS; ++r)
            pixels[r * MATRIX_COLS + col] = brightness;
    }

    // Draw a rectangle (inclusive bounds)
    void setRect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1, uint8_t brightness) {
        for (uint8_t r = r0; r <= r1 && r < MATRIX_ROWS; ++r)
            for (uint8_t c = c0; c <= c1 && c < MATRIX_COLS; ++c)
                pixels[r * MATRIX_COLS + c] = brightness;
    }

    // Invert: on→off for binary, or (maxBrightness − value) for grayscale
    void invert(uint8_t maxBrightness = 1) {
        for (uint8_t i = 0; i < MATRIX_PIXELS; ++i)
            pixels[i] = (pixels[i] >= maxBrightness) ? 0 : maxBrightness;
    }

    // Shift all pixels left/right/up/down (wraps or fills with 0)
    void shiftLeft(bool wrap = false) {
        for (uint8_t r = 0; r < MATRIX_ROWS; ++r) {
            uint8_t first = pixels[r * MATRIX_COLS];
            for (uint8_t c = 0; c < MATRIX_COLS - 1; ++c)
                pixels[r * MATRIX_COLS + c] = pixels[r * MATRIX_COLS + c + 1];
            pixels[r * MATRIX_COLS + MATRIX_COLS - 1] = wrap ? first : 0;
        }
    }

    void shiftRight(bool wrap = false) {
        for (uint8_t r = 0; r < MATRIX_ROWS; ++r) {
            uint8_t last = pixels[r * MATRIX_COLS + MATRIX_COLS - 1];
            for (uint8_t c = MATRIX_COLS - 1; c > 0; --c)
                pixels[r * MATRIX_COLS + c] = pixels[r * MATRIX_COLS + c - 1];
            pixels[r * MATRIX_COLS] = wrap ? last : 0;
        }
    }

    void shiftUp(bool wrap = false) {
        uint8_t tmp[MATRIX_COLS];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c) tmp[c] = pixels[c];
        for (uint8_t r = 0; r < MATRIX_ROWS - 1; ++r)
            for (uint8_t c = 0; c < MATRIX_COLS; ++c)
                pixels[r * MATRIX_COLS + c] = pixels[(r + 1) * MATRIX_COLS + c];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            pixels[(MATRIX_ROWS - 1) * MATRIX_COLS + c] = wrap ? tmp[c] : 0;
    }

    void shiftDown(bool wrap = false) {
        uint8_t tmp[MATRIX_COLS];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            tmp[c] = pixels[(MATRIX_ROWS - 1) * MATRIX_COLS + c];
        for (uint8_t r = MATRIX_ROWS - 1; r > 0; --r)
            for (uint8_t c = 0; c < MATRIX_COLS; ++c)
                pixels[r * MATRIX_COLS + c] = pixels[(r - 1) * MATRIX_COLS + c];
        for (uint8_t c = 0; c < MATRIX_COLS; ++c)
            pixels[c] = wrap ? tmp[c] : 0;
    }

    // Serialise to "0,1,0,..." for Bridge transfer (stays ≤~450 chars)
    String toString() const {
        String s;
        s.reserve(MATRIX_PIXELS * 3);
        for (uint8_t i = 0; i < MATRIX_PIXELS; ++i) {
            s += String(pixels[i]);
            if (i < MATRIX_PIXELS - 1) s += ',';
        }
        return s;
    }
};

// ─── LedMatrixStory ──────────────────────────────────────────────────────────

class LedMatrixStory {
public:
    static constexpr uint8_t MAX_SCENES = 64;

    explicit LedMatrixStory(Arduino_LED_Matrix& matrix)
        : _matrix(matrix) {}

    // ── Scene management ────────────────────────────────────────────────────

    bool addScene(const LedMatrixScene& scene) {
        if (_count >= MAX_SCENES) return false;
        _scenes[_count++] = scene;
        return true;
    }

    bool insertScene(uint8_t index, const LedMatrixScene& scene) {
        if (_count >= MAX_SCENES || index > _count) return false;
        for (uint8_t i = _count; i > index; --i) _scenes[i] = _scenes[i - 1];
        _scenes[index] = scene;
        ++_count;
        if (_currentFrame >= index) ++_currentFrame;
        return true;
    }

    bool removeScene(uint8_t index) {
        if (index >= _count) return false;
        for (uint8_t i = index; i < _count - 1; ++i) _scenes[i] = _scenes[i + 1];
        --_count;
        if (_currentFrame >= _count && _count > 0) _currentFrame = _count - 1;
        return true;
    }

    // Replace scene at index — or add if index == sceneCount()
    bool setScene(uint8_t index, const LedMatrixScene& scene) {
        if (index == _count) return addScene(scene);
        if (index >= _count) return false;
        _scenes[index] = scene;
        return true;
    }

    LedMatrixScene* getScene(uint8_t index) {
        return (index < _count) ? &_scenes[index] : nullptr;
    }

    uint8_t sceneCount() const { return _count; }
    void    clearAll()         { _count = 0; _currentFrame = 0; _playing = false; }

    // ── Playback ────────────────────────────────────────────────────────────

    // Start playing — call update() from loop()
    void play(unsigned long frameDelayMs = 200, bool loop = true) {
        if (_count == 0) return;
        _frameDelay    = frameDelayMs;
        _loop          = loop;
        _playing       = true;
        _lastFrameTime = millis();
        _render(_scenes[_currentFrame]);
    }

    void pause() { _playing = false; }

    void stop() {
        _playing       = false;
        _currentFrame  = 0;
        if (_count > 0) _render(_scenes[0]);
    }

    void nextFrame() {
        if (_count == 0) return;
        _currentFrame = (_currentFrame + 1) % _count;
        _render(_scenes[_currentFrame]);
        _lastFrameTime = millis();
    }

    void prevFrame() {
        if (_count == 0) return;
        _currentFrame = (_currentFrame == 0) ? _count - 1 : _currentFrame - 1;
        _render(_scenes[_currentFrame]);
        _lastFrameTime = millis();
    }

    void gotoFrame(uint8_t index) {
        if (index >= _count) return;
        _currentFrame = index;
        _render(_scenes[_currentFrame]);
        _lastFrameTime = millis();
    }

    // Render a specific scene without changing the current frame pointer
    void previewScene(uint8_t index) {
        if (index < _count) _render(_scenes[index]);
    }

    // Render any arbitrary scene (e.g. from Bridge before adding it)
    void previewScene(const LedMatrixScene& scene) {
        _render(scene);
    }

    // Clear the display (all pixels off)
    void blank() {
        LedMatrixScene empty;
        _render(empty);
    }

    // ── Must be called in loop() ─────────────────────────────────────────────

    void update() {
        if (!_playing || _count == 0) return;
        if (millis() - _lastFrameTime >= _frameDelay) {
            _lastFrameTime = millis();
            _currentFrame++;
            if (_currentFrame >= _count) {
                if (_loop) {
                    _currentFrame = 0;
                } else {
                    _currentFrame = _count - 1;
                    _playing = false;
                    return;
                }
            }
            _render(_scenes[_currentFrame]);
        }
    }

    // ── Settings ─────────────────────────────────────────────────────────────

    void setFrameDelay(unsigned long ms) { _frameDelay = ms; }
    void setLoop(bool loop)              { _loop = loop; }
    void setGrayscaleBits(uint8_t bits)  { _matrix.setGrayscaleBits(bits); }

    // ── State queries ─────────────────────────────────────────────────────────

    uint8_t       currentFrame() const { return _currentFrame; }
    bool          isPlaying()    const { return _playing; }
    unsigned long frameDelay()   const { return _frameDelay; }

private:
    Arduino_LED_Matrix& _matrix;
    LedMatrixScene      _scenes[MAX_SCENES];
    uint8_t             _count        = 0;
    uint8_t             _currentFrame = 0;
    bool                _playing      = false;
    bool                _loop         = true;
    unsigned long       _frameDelay   = 200;
    unsigned long       _lastFrameTime= 0;

    void _render(const LedMatrixScene& scene) {
        _matrix.draw(scene.pixels);
    }
};
