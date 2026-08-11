# Contributing to C-Arduino-Libraries

First off, thank you for taking the time to contribute! 🎉 This repository houses a diverse ecosystem of C++ Arduino libraries, experimental modules, and architecture-specific Python libraries. Every contribution — big or small — helps make this project better for the entire embedded community.

To maintain code quality and ensure compatibility across multiple hardware architectures, please review the following guidelines before submitting your work.

---

## 📂 Repository Structure

Before diving in, please familiarise yourself with how the codebase is organised:

| Path | Purpose |
|------|---------|
| `lib/[LibraryName]/` | Core, stable C++ Arduino libraries |
| `lib/[LibraryName]/src/` | Library source files (`.h` / `.cpp`) |
| `lib/[LibraryName]/README.md` | Per-library API documentation |
| `test/test_[LibraryName]/` | Unity-based unit tests (PlatformIO) |
| `test/host_arduino_sim/` | Host-side simulation/stress tests |

---

## 🐛 Reporting Bugs & Feature Requests

1. Check the [Issues](../../issues) tab to see if your bug or feature has already been reported.
2. If not, open a new issue and select the appropriate template:
   - **Bug Report** — include the specific hardware architecture, board model, Arduino core version, and steps to reproduce.
   - **Feature Request** — describe the problem you are trying to solve and why the existing API does not cover it.
3. Add relevant labels (e.g., `bug`, `enhancement`, `question`).

> **New contributors:** Look for issues labelled [**good first issue**](../../issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) — these are beginner-friendly tasks that are a great way to get started!

---

## 🚀 How to Contribute Code

### 1. Fork & Clone

```bash
# Fork on GitHub, then clone your fork
git clone https://github.com/<your-username>/C-Arduino-Libraries.git
cd C-Arduino-Libraries
```

### 2. Create a Branch

Use a short, descriptive name that reflects your change:

```bash
git checkout -b fix/hashtable-resize-overflow
git checkout -b feat/avltree-iterator
```

### 3. Make Your Changes

Follow the [Coding Standards](#-coding-standards--guidelines) below.

### 4. Test Locally

```bash
# Run Unity unit tests via PlatformIO (requires PlatformIO CLI)
pio test -e arduino_mega2560_test

# Build and run host simulation tests
cd test/host_arduino_sim
# See test/host_arduino_sim/README.md for full build instructions
```

### 5. Commit

Write clear, imperative commit messages:

```
feat(SimpleVector): add shrink_to_fit() method
fix(Hashtable): prevent rehash loop on zero capacity
docs(Json): document tryGetString optional API
```

### 6. Open a Pull Request

- Push your branch to your fork and open a PR against `main`.
- Fill in the PR template completely.
- Link any related issues (`Closes #123`).
- Ensure all CI checks pass before requesting a review.

---

## 🛠 Coding Standards & Guidelines

### C++ / Arduino Libraries

- **Naming Conventions**
  - Class names: `PascalCase` (e.g., `SimpleVector`, `Hashtable`)
  - Methods and variables: `camelCase` (e.g., `getSize()`, `loadFactor`)
  - Constants / macros: `UPPER_SNAKE_CASE`
- **Portability** — Libraries must compile cleanly across different microcontrollers (AVR, ARM, RP2040, ESP32, etc.) unless explicitly restricted. Use `#ifdef ARDUINO` guards where needed and avoid platform-specific headers in public API files.
- **Memory** — Prefer stack allocation; document any dynamic allocations clearly. Avoid `std::` heap containers in code that targets bare-metal AVR.
- **New / Experimental Code** — Introduce large architectural changes or high-risk new features under community feedback first by opening an issue or discussion before submitting a PR.
- **Header Guards** — Use `#pragma once` or traditional `#ifndef` include guards consistently with the surrounding file.

### Python Libraries

- **Code Style**: Follow [PEP 8](https://peps.python.org/pep-0008/).
- **Type Hints**: Add type annotations to all public functions and methods.
- **Architecture Isolation**: Keep architecture-specific scripts within their dedicated subdirectory. Never mix cross-platform scripts with localised hardware bindings.

---

## 🧪 Testing Your Changes

We use automated workflows to validate every PR. Please ensure the following before submitting:

| Change type | Expectation |
|-------------|-------------|
| C++ library | Compiles cleanly against Arduino core; existing unit tests pass |
| New C++ library | Add at least a smoke-test in `test/host_arduino_sim/` and update `test/host_arduino_sim/host_test_matrix.json` |
| Python script | No syntax or type errors; compatible with targeted Linux / MPU host |
| Documentation | Markdown renders correctly; links are not broken |

---

## 📝 Pull Request Checklist

Before marking your PR ready for review, confirm:

- [ ] Branch is up to date with `main`
- [ ] Code follows the style guidelines above
- [ ] All existing CI checks pass
- [ ] New or changed public API is documented in the library's `README.md`
- [ ] Commit messages are clear and follow the convention above

---

## 🤝 Code of Conduct

This project follows our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you agree to uphold it. Please report unacceptable behaviour to the project maintainers via a private message or by opening a confidential issue.

---

Thank you again for helping make C-Arduino-Libraries better for everyone! 🚀
