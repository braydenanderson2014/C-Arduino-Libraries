# Contributing to C-Arduino-Libraries

First off, thank you for taking the time to contribute! 🎉 This repository houses a diverse ecosystem of C++ Arduino libraries, experimental modules, and architecture-specific Python libraries targeting the Arduino UnoQ / dual-core bridge workflow. Every contribution — big or small — helps make this project better for the entire embedded community.

To maintain code quality and ensure compatibility across multiple hardware architectures, please review the following guidelines before submitting your work.

---

## 📂 Repository Structure

Before diving in, please familiarise yourself with how the codebase is organised:

| Path | Purpose |
|------|---------|
| `lib/[LibraryName]/` | Core, stable C++ Arduino libraries |
| `lib/[LibraryName]/src/` | Library source files (`.h` / `.cpp`) |
| `lib/[LibraryName]/README.md` | Per-library API documentation |
| `lib/__ArduinoUnoQLibraryDevelopment__Experimental/` | Experimental libraries for the Arduino UnoQ dual-core architecture (STM32 MCU + Qualcomm MPU) |
| `lib/__ArduinoUnoQLibraryDevelopment__Experimental/[LibraryName]/` | Experimental library source (e.g. `SimpleChannel`, `SimpleMutex`, `UnoQBridge`) |
| `test/test_[LibraryName]/` | Unity-based unit tests (run via PlatformIO) |
| `test/host_arduino_sim/` | Host-side simulation / stress tests (compiled with GCC, no hardware required) |
| `platformio.ini` | PlatformIO project configuration — board environments, lib dependencies, test settings |

### The Experimental Folder

`lib/__ArduinoUnoQLibraryDevelopment__Experimental/` is the incubation area for the **Arduino UnoQ** platform — a dual-core workflow where an Arduino sketch runs on an STM32 MCU while Python runs in a Linux container on the Qualcomm MPU. Libraries here are **not yet stable** and may change or be removed without notice. If you want to contribute to this area:

- Read [`lib/__ArduinoUnoQLibraryDevelopment__Experimental/README.md`](lib/__ArduinoUnoQLibraryDevelopment__Experimental/README.md) first.
- Coordinate via an issue before making large changes — architecture decisions here have cross-boundary implications.
- Python-side handlers live in `lib/__ArduinoUnoQLibraryDevelopment__Experimental/handlers/` and must stay isolated from non-UnoQ code.

---

## 🐛 Reporting Bugs & Feature Requests

1. Check the [Issues](../../issues) tab to see if your bug or feature has already been reported.
2. If not, open a new issue and select the appropriate template:
   - **Bug Report** — include the specific hardware architecture, board model, Arduino core / PlatformIO platform version, and steps to reproduce.
   - **Feature Request** — describe the problem you are trying to solve and why the existing API does not cover it.
   - **Documentation Request** — for missing or incorrect docs on a library.
   - **General Question** — for usage questions.
3. Labels are applied automatically by our triage workflows. You do not need to add them manually.

> **New contributors:** Look for issues labelled [**good first issue**](../../issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) — these are beginner-friendly tasks that are a great way to get started! Our automation will also label your first issue so maintainers know to give you extra guidance.

---

## 🚀 How to Contribute Code

### 1. Prerequisites

- [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation/index.html) (or PlatformIO IDE extension in VS Code)
- GCC / G++ (for host simulation tests — no hardware required)
- Git

### 2. Fork & Clone

```bash
# Fork on GitHub, then clone your fork
git clone https://github.com/<your-username>/C-Arduino-Libraries.git
cd C-Arduino-Libraries
```

### 3. Install PlatformIO Dependencies

PlatformIO resolves all `lib_deps` automatically when you build or test, but you can pre-fetch them with:

```bash
pio pkg install
```

### 4. Create a Branch

Use a short, descriptive name that reflects your change:

```bash
git checkout -b fix/hashtable-resize-overflow
git checkout -b feat/avltree-iterator
git checkout -b experimental/uniq-simplechannel-buffering
```

### 5. Make Your Changes

Follow the [Coding Standards](#-coding-standards--guidelines) below.

Determine where your change belongs:

| Type of change | Location |
|----------------|---------|
| Stable library bug fix or enhancement | `lib/[LibraryName]/` |
| New stable library | `lib/[NewLibraryName]/` (with `src/` and `README.md`) |
| UnoQ / experimental feature | `lib/__ArduinoUnoQLibraryDevelopment__Experimental/[LibraryName]/` |
| Unit test | `test/test_[LibraryName]/` |
| Host simulation test | `test/host_arduino_sim/` |

### 6. Build Locally

```bash
# Build the default environment (Arduino Mega 2560)
pio run

# Build a specific environment
pio run -e arduino_mega2560
```

### 7. Test Locally

```bash
# Run Unity unit tests (requires a connected Arduino Mega 2560 on COM5 by default)
pio test -e arduino_mega2560_test

# Run OrderedMap tests (separate environment — Json is not ignored here)
pio test -e arduino_mega2560_test_orderedmap

# Build and run host simulation tests (no hardware needed)
cd test/host_arduino_sim
# See test/host_arduino_sim/README.md for full build instructions
```

> **Tip:** If you do not have hardware available, the host simulation tests in `test/host_arduino_sim/` cover many libraries without requiring a board.

When adding a new library, register it in `test/host_arduino_sim/host_test_matrix.json` so it is picked up by the automated compile-smoke matrix.

### 8. Commit

Write clear, imperative commit messages:

```
feat(SimpleVector): add shrink_to_fit() method
fix(Hashtable): prevent rehash loop on zero capacity
docs(Json): document tryGetString optional API
experimental(SimpleChannel): add bounded buffer mode
```

### 9. Open a Pull Request

- Push your branch to your fork and open a PR against `main`.
- Fill in the PR template completely.
- Link any related issues (`Closes #123`).
- Ensure all CI checks pass before requesting a review.
- Our automation will label the PR and add it to the triage queue.

---

## 🛠 Coding Standards & Guidelines

### C++ / Arduino Libraries

- **Naming Conventions**
  - Class names: `PascalCase` (e.g., `SimpleVector`, `Hashtable`)
  - Methods and variables: `camelCase` (e.g., `getSize()`, `loadFactor`)
  - Constants / macros: `UPPER_SNAKE_CASE`
- **Portability** — Libraries in `lib/` must compile cleanly across different microcontrollers (AVR, ARM, RP2040, ESP32, etc.) unless explicitly restricted. Use `#ifdef ARDUINO` guards where needed (see `lib/AVLTree/src/AVLTree.h` for a reference implementation) and avoid platform-specific headers in public API files.
- **Memory** — Prefer stack allocation; document any dynamic allocations clearly. Avoid `std::` heap containers in code that targets bare-metal AVR.
- **Header Guards** — Use `#pragma once` or traditional `#ifndef` include guards, consistently with the surrounding file.
- **PlatformIO `lib_ignore`** — If your library must be excluded from certain build environments, update `platformio.ini` accordingly and document why.
- **Build flags** — The default build uses `-std=gnu++11`. Do not require C++14 or later in stable libraries without discussion.

### Experimental (UnoQ) Libraries

- Experimental code lives exclusively in `lib/__ArduinoUnoQLibraryDevelopment__Experimental/`.
- Arduino-side code (STM32 MCU) and Python-side code (Qualcomm MPU) must remain clearly separated. Never import hardware-specific C++ headers from Python handlers or vice versa.
- Concurrency primitives (`SimpleMutex`, `SimpleRWLock`, `SimpleSemaphore`) must be tested against the UnoQ runtime before promotion to stable.

### Python Libraries / Scripts

- **Code Style**: Follow [PEP 8](https://peps.python.org/pep-0008/).
- **Type Hints**: Add type annotations to all public functions and methods.
- **Architecture Isolation**: Keep architecture-specific scripts (UnoQ handlers, bridge utilities) within their dedicated subdirectory. Never mix cross-platform scripts with localised hardware bindings.

---

## 🧪 Testing Your Changes

We use automated workflows to validate every PR. Please ensure the following before submitting:

| Change type | Expectation |
|-------------|-------------|
| C++ stable library | Compiles cleanly against Arduino core; existing Unity tests pass |
| New C++ stable library | Add `test/test_[LibraryName]/` Unity test suite; add entry to `test/host_arduino_sim/host_test_matrix.json` |
| Experimental library | At minimum, add a host-sim smoke test; document hardware requirements in the library README |
| Python script | No syntax or type errors; compatible with targeted Linux / MPU host |
| Documentation | Markdown renders correctly; links are not broken |

CI automatically runs:
- **compile-examples** — compiles representative sketches
- **arduino-host-sim-tests** — builds and runs host simulation correctness and stress tests
- **host-sim-memory-profiles** — memory profiling on the host sim suite
- **check-arduino** — verifies Arduino compatibility
- **spell-check** — catches typos in documentation

---

## 📝 Pull Request Checklist

Before marking your PR ready for review, confirm:

- [ ] Branch is up to date with `main`
- [ ] Code follows the style guidelines above
- [ ] `pio run` builds without errors
- [ ] Relevant `pio test` environments pass (or hardware unavailability is noted)
- [ ] All existing CI checks pass
- [ ] New or changed public API is documented in the library's `README.md`
- [ ] If adding a new library, `test/host_arduino_sim/host_test_matrix.json` is updated
- [ ] Commit messages are clear and follow the convention above

---

## 🤝 Code of Conduct

This project follows our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you agree to uphold it. Please report unacceptable behaviour to the project maintainers via a private message or by opening a confidential issue.

---

Thank you again for helping make C-Arduino-Libraries better for everyone! 🚀
