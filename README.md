# C-Arduino-Libraries

🚀 A collection of Arduino-focused C++ libraries with shared tooling, automated quality checks, and registry tracking.

[![Arduino Host Simulation Tests](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/arduino-host-sim-tests.yml/badge.svg)](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/arduino-host-sim-tests.yml)
[![Compile Examples](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/compile-examples.yml)
[![Cross-Repo Health Report](https://github.com/braydenanderson2014/SystemCommands/actions/workflows/Generate%20Cross-Repo%20Library%20Health%20Report.yml/badge.svg)](https://github.com/braydenanderson2014/SystemCommands/actions/workflows/Generate%20Cross-Repo%20Library%20Health%20Report.yml)
[![PlatformIO Metrics](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/update-platformio-metrics.yml/badge.svg)](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/update-platformio-metrics.yml)

## Quick Start

- Libraries live under `lib/`.
- Most libraries include their own README with API and examples.
- Experimental UnoQ branch work is in `lib/__ArduinoUnoQLibraryDevelopment__Experimental/`.

## Automation-Driven Status

This repo now relies heavily on workflows for health and stability reporting.

- Library health summary: [reports/library-health-report.md](reports/library-health-report.md)
- PlatformIO metrics report: [PLATFORMIO_METRICS.md](PLATFORMIO_METRICS.md)
- Library changelog index: [reports/library-changelogs/README.md](reports/library-changelogs/README.md)
- Host simulation and compile workflows: `.github/workflows/`

## Feature Snapshot

Expect practical building blocks rather than one giant framework:

- Storage and file friendly patterns: SD workflows (`SDList`) and filesystem-oriented integration points (including `LittleFS` wrapper paths in this repo).
- Modern utility helpers: `Optional`, `Variant`, `TypeTraits`, `Predicates`, and iterator/style support.
- Data structures: maps, trees, lists, queues, stacks, vectors, and hash containers.
- Math and utility helpers: `MathLib`, `MatrixMath`, timers, and lightweight support modules.

For deep details, check each library README and examples under `lib/`.


<!-- HEALTH_BADGES_START -->
## Library Health Badges

Auto-generated from workflow status results (2026-09-01T01:55:46.079Z).

![Array List](https://img.shields.io/badge/Array%20List-Healthy-2ea44f) ![AVL Tree](https://img.shields.io/badge/AVL%20Tree-Healthy-2ea44f) ![B Plus Tree](https://img.shields.io/badge/B%20Plus%20Tree-Healthy-2ea44f) ![B Tree](https://img.shields.io/badge/B%20Tree-Healthy-2ea44f) ![Binary Search Tree](https://img.shields.io/badge/Binary%20Search%20Tree-Healthy-2ea44f) ![Binary Tree](https://img.shields.io/badge/Binary%20Tree-Healthy-2ea44f) ![Color Manager](https://img.shields.io/badge/Color%20Manager-Healthy-2ea44f) ![Dictionary](https://img.shields.io/badge/Dictionary-Healthy-2ea44f) ![Double Linked List](https://img.shields.io/badge/Double%20Linked%20List-Healthy-2ea44f) ![Ethernet Connection Checker](https://img.shields.io/badge/Ethernet%20Connection%20Checker-Healthy-2ea44f) ![Ethernet Mach2 Connection Checker](https://img.shields.io/badge/Ethernet%20Mach2%20Connection%20Checker-Healthy-2ea44f) ![Extreme Variant](https://img.shields.io/badge/Extreme%20Variant-Healthy-2ea44f) ![Fenwick Tree](https://img.shields.io/badge/Fenwick%20Tree-Healthy-2ea44f) ![Hashtable](https://img.shields.io/badge/Hashtable-Healthy-2ea44f) ![Heap Tree](https://img.shields.io/badge/Heap%20Tree-Healthy-2ea44f) ![Initializer List](https://img.shields.io/badge/Initializer%20List-Healthy-2ea44f) ![Interval Tree](https://img.shields.io/badge/Interval%20Tree-Healthy-2ea44f) ![Iterator](https://img.shields.io/badge/Iterator-Healthy-2ea44f) ![Map](https://img.shields.io/badge/Map-Healthy-2ea44f) ![Math Lib](https://img.shields.io/badge/Math%20Lib-Healthy-2ea44f) ![Matrix Math](https://img.shields.io/badge/Matrix%20Math-Healthy-2ea44f) ![My Dictionary](https://img.shields.io/badge/My%20Dictionary-Healthy-2ea44f) ![Numeric Limits](https://img.shields.io/badge/Numeric%20Limits-Healthy-2ea44f) ![OC Tree](https://img.shields.io/badge/OC%20Tree-Healthy-2ea44f) ![Operators](https://img.shields.io/badge/Operators-Healthy-2ea44f) ![Optional](https://img.shields.io/badge/Optional-Healthy-2ea44f) ![Predicates](https://img.shields.io/badge/Predicates-Healthy-2ea44f) ![Quad Tree](https://img.shields.io/badge/Quad%20Tree-Healthy-2ea44f) ![R Tree](https://img.shields.io/badge/R%20Tree-Healthy-2ea44f) ![Red Black Tree](https://img.shields.io/badge/Red%20Black%20Tree-Healthy-2ea44f) ![SD List](https://img.shields.io/badge/SD%20List-Healthy-2ea44f) ![Segment Tree](https://img.shields.io/badge/Segment%20Tree-Healthy-2ea44f) ![Simple Vector](https://img.shields.io/badge/Simple%20Vector-Healthy-2ea44f) ![Suffix Tree](https://img.shields.io/badge/Suffix%20Tree-Healthy-2ea44f) ![Trie Tree](https://img.shields.io/badge/Trie%20Tree-Healthy-2ea44f) ![Type Traits](https://img.shields.io/badge/Type%20Traits-Healthy-2ea44f) ![Unordered Map](https://img.shields.io/badge/Unordered%20Map-Healthy-2ea44f) ![Variant](https://img.shields.io/badge/Variant-Healthy-2ea44f)

Lint: ![Passed Lint: 11](https://img.shields.io/badge/Passed%20Lint-11-2ea44f?style=flat-square) ![Failed Lint: 0](https://img.shields.io/badge/Failed%20Lint-0-d73a49?style=flat-square) ![Unknown Lint: 41](https://img.shields.io/badge/Unknown%20Lint-41-9e9e9e?style=flat-square)
Compilation: ![Passed Compilation: 44](https://img.shields.io/badge/Passed%20Compilation-44-2ea44f?style=flat-square) ![Failed Compilation: 0](https://img.shields.io/badge/Failed%20Compilation-0-d73a49?style=flat-square) ![Unknown Compilation: 8](https://img.shields.io/badge/Unknown%20Compilation-8-9e9e9e?style=flat-square)
<!-- HEALTH_BADGES_END -->

<!-- PLATFORMIO_BADGES_START -->
## PlatformIO Registry Badges

Auto-generated from PlatformIO registry package data (2026-09-01 00:20 UTC).

Quick expectation notes: data structures, utility helpers (`Optional`, `Variant`, `TypeTraits`), and storage-facing support (`SD`/`SDList` and repo-level `LittleFS` integration paths).

### Data Structures

- [AVL Tree](lib/AVLTree) [![AVL Tree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/AVL%20Tree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/AVL%20Tree)
- [ArrayList](lib/ArrayList) [![ArrayList](https://badges.registry.platformio.org/packages/braydenanderson2014/library/ArrayList.svg)](https://registry.platformio.org/libraries/braydenanderson2014/ArrayList)
- [BPlusTree](lib/bPlusTree) [![BPlusTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/BPlusTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/BPlusTree)
- [BTree](lib/bTree) [![BTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/BTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/BTree)
- [BasicLinkedList](lib/LinkedList) [![BasicLinkedList](https://badges.registry.platformio.org/packages/braydenanderson2014/library/BasicLinkedList.svg)](https://registry.platformio.org/libraries/braydenanderson2014/BasicLinkedList)
- [BinarySearchTree](lib/BinarySearchTree) [![BinarySearchTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/BinarySearchTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/BinarySearchTree)
- [BinaryTree](lib/BinaryTree) [![BinaryTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/BinaryTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/BinaryTree)
- [Dictionary](lib/Dictionary) [![Dictionary](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Dictionary.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Dictionary)
- [DoubleLinkedList](lib/DoubleLinkedList) [![DoubleLinkedList](https://badges.registry.platformio.org/packages/braydenanderson2014/library/DoubleLinkedList.svg)](https://registry.platformio.org/libraries/braydenanderson2014/DoubleLinkedList)
- [FenwickTree](lib/FenwickTree) [![FenwickTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/FenwickTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/FenwickTree)
- [Hashtable](lib/Hashtable) [![Hashtable](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Hashtable.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Hashtable)
- [HeapTree](lib/HeapTree) [![HeapTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/HeapTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/HeapTree)
- [Initializer_List](lib/Initializer_List) [![Initializer_List](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Initializer_List.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Initializer_List)
- [IntervalTree](lib/IntervalTree) [![IntervalTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/IntervalTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/IntervalTree)
- [KDimensonalTree](lib/kdimensionalTree) [![KDimensonalTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/KDimensonalTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/KDimensonalTree)
- [Map](lib/Map) [![Map](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Map.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Map)
- [MyDictionary](lib/MyDictionary) [![MyDictionary](https://badges.registry.platformio.org/packages/braydenanderson2014/library/MyDictionary.svg)](https://registry.platformio.org/libraries/braydenanderson2014/MyDictionary)
- [OcTree](lib/OCTree) [![OcTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/OcTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/OcTree)
- [QuadTree](lib/QuadTree) [![QuadTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/QuadTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/QuadTree)
- [RTree](lib/RTree) [![RTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/RTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/RTree)
- [RedBlackTree](lib/RedBlackTree) [![RedBlackTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/RedBlackTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/RedBlackTree)
- [SDList](lib/SDList) [![SDList](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SDList.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SDList)
- [SegmentTree](lib/SegmentTree) [![SegmentTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SegmentTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SegmentTree)
- [SimpleQueue](lib/Queue) [![SimpleQueue](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SimpleQueue.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SimpleQueue)
- [SimpleStack](lib/Stack) [![SimpleStack](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SimpleStack.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SimpleStack)
- [SimpleVector](lib/SimpleVector) [![SimpleVector](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SimpleVector.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SimpleVector)
- [SuffixTree](lib/SuffixTree) [![SuffixTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SuffixTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SuffixTree)
- [TrieTree](lib/TrieTree) [![TrieTree](https://badges.registry.platformio.org/packages/braydenanderson2014/library/TrieTree.svg)](https://registry.platformio.org/libraries/braydenanderson2014/TrieTree)
- [UnorderedMap](lib/UnorderedMap) [![UnorderedMap](https://badges.registry.platformio.org/packages/braydenanderson2014/library/UnorderedMap.svg)](https://registry.platformio.org/libraries/braydenanderson2014/UnorderedMap)

### Language and Utility Helpers

- [Iterator](lib/iterator) [![Iterator](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Iterator.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Iterator)
- [Numeric_Limits](lib/Numeric_Limits) [![Numeric_Limits](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Numeric_Limits.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Numeric_Limits)
- [Operators](lib/Operators) [![Operators](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Operators.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Operators)
- [Optional](lib/Optional) [![Optional](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Optional.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Optional)
- [Predicates](lib/Predicates) [![Predicates](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Predicates.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Predicates)
- [TypeTraits](lib/TypeTraits) [![TypeTraits](https://badges.registry.platformio.org/packages/braydenanderson2014/library/TypeTraits.svg)](https://registry.platformio.org/libraries/braydenanderson2014/TypeTraits)
- [Variant](lib/Variant) [![Variant](https://badges.registry.platformio.org/packages/braydenanderson2014/library/Variant.svg)](https://registry.platformio.org/libraries/braydenanderson2014/Variant)

### Storage and IO

- [BasicIOStream](lib/iostream) [![BasicIOStream](https://badges.registry.platformio.org/packages/braydenanderson2014/library/BasicIOStream.svg)](https://registry.platformio.org/libraries/braydenanderson2014/BasicIOStream)
- [SimpleProperties](lib/Properties) [![SimpleProperties](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SimpleProperties.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SimpleProperties)

### Math and Runtime Helpers

- [ColorManager](lib/ColorManager) [![ColorManager](https://badges.registry.platformio.org/packages/braydenanderson2014/library/ColorManager.svg)](https://registry.platformio.org/libraries/braydenanderson2014/ColorManager)
- [MathLib](lib/MathLib) [![MathLib](https://badges.registry.platformio.org/packages/braydenanderson2014/library/MathLib.svg)](https://registry.platformio.org/libraries/braydenanderson2014/MathLib)
- [MatrixMath](lib/MatrixMath) [![MatrixMath](https://badges.registry.platformio.org/packages/braydenanderson2014/library/MatrixMath.svg)](https://registry.platformio.org/libraries/braydenanderson2014/MatrixMath)
- [SimpleArduinoTimer](lib/Timer) [![SimpleArduinoTimer](https://badges.registry.platformio.org/packages/braydenanderson2014/library/SimpleArduinoTimer.svg)](https://registry.platformio.org/libraries/braydenanderson2014/SimpleArduinoTimer)

<!-- PLATFORMIO_BADGES_END -->

## Health and Testing

Track quality and CI status from these workflow/report entry points:

- Cross-repo health status (SystemCommands issue buckets): [![Cross-Repo Health Report](https://github.com/braydenanderson2014/SystemCommands/actions/workflows/Generate%20Cross-Repo%20Library%20Health%20Report.yml/badge.svg)](https://github.com/braydenanderson2014/SystemCommands/actions/workflows/Generate%20Cross-Repo%20Library%20Health%20Report.yml)
- Host-level simulation tests: [![Arduino Host Simulation Tests](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/arduino-host-sim-tests.yml/badge.svg)](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/arduino-host-sim-tests.yml)
- Build and compile coverage: [![Compile Examples](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/compile-examples.yml)
- JSON host simulation tests: [![JSON Host Sim Tests](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/json-host-sim-tests.yml/badge.svg)](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/json-host-sim-tests.yml)
- Aggregated health output: [reports/library-health-report.md](reports/library-health-report.md)
- PlatformIO metrics and registry overview: [PLATFORMIO_METRICS.md](PLATFORMIO_METRICS.md)

Notes:
- Compile/testing pass-fail labels are synchronized to SystemCommands from the host simulation workflow jobs.
- Workflow badges reflect the latest run outcome, including `cancelled` when a run is interrupted.
- For debugging failures, open the workflow run and inspect step logs directly.

## Testing Strategy and Confidence Levels

This repository uses multiple testing layers on purpose. Each layer catches different classes of failures.

### 1) Compile Validation

Primary signal:
- [Compile Examples workflow](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/compile-examples.yml)
- Local workflow file: [.github/workflows/compile-examples.yml](.github/workflows/compile-examples.yml)

What it checks:
- Example sketches and library code still compile.
- Header/API changes did not break common include paths or usage patterns.

What it does not guarantee:
- Runtime behavior on real boards.
- Correct timing, interrupts, or peripheral behavior.

### 2) Host Simulation and Functional Tests

Primary signal:
- [Arduino Host Simulation Tests workflow](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/arduino-host-sim-tests.yml)
- Local workflow file: [.github/workflows/arduino-host-sim-tests.yml](.github/workflows/arduino-host-sim-tests.yml)

Related signal:
- [JSON Host Sim Tests workflow](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/json-host-sim-tests.yml)
- Local workflow file: [.github/workflows/json-host-sim-tests.yml](.github/workflows/json-host-sim-tests.yml)

What they check:
- Data-structure and algorithm correctness under repeatable host-side conditions.
- Serialization and parser behavior for JSON-oriented flows.
- Regressions in logic that do not require physical hardware.

What they do not guarantee:
- Exact SRAM/stack pressure behavior on 8-bit targets.
- Hardware-dependent timing and bus-level edge cases.

### 3) Memory and Resource-Oriented Testing

Primary signal:
- [Host Sim Memory Profiles workflow](https://github.com/braydenanderson2014/C-Arduino-Libraries/actions/workflows/host-sim-memory-profiles.yml)
- Local workflow file: [.github/workflows/host-sim-memory-profiles.yml](.github/workflows/host-sim-memory-profiles.yml)

What it checks:
- Relative allocation trends, leak-like patterns, and growth behavior in host simulation.
- Useful early warnings when a change increases memory pressure.

What it does not guarantee:
- Exact AVR/ARM allocator behavior.
- Real board fragmentation patterns over long uptime.

### Why a Passing CI Run Can Still Fail on Arduino Hardware

A green CI badge means strong baseline quality, but not full hardware certification.

Common reasons for hardware-only failures:
- Smaller SRAM/flash limits than host environment assumptions.
- Different compiler flags, ABI details, or core library implementations.
- Peripheral timing differences (SPI/I2C/UART), ISR interactions, or watchdog behavior.
- Electrical and board-specific factors (clock, wiring quality, power stability).

Practical confidence guide:
- Compile pass only: low-to-medium confidence for runtime on device.
- Compile + host sim pass: medium-to-high confidence for pure logic libraries.
- Compile + host sim + memory profile pass: higher confidence, but still verify on target board before release.

Recommended release gate:
1. Pass compile workflow.
2. Pass host simulation workflows.
3. Review memory profile diffs for unusual growth.
4. Run at least one real-board smoke test for each changed library family.

## Issue Automation and Moderation Tools

This repository includes automated issue triage and duplicate-management workflows. They reduce manual work, but they also use protected labels and project-field updates that contributors should understand.

### What Tools Exist

- Challenge handling: [Challenge Handler](.github/workflows/challenge-handler.yml)
- Duplicate detection and AI training: [Duplicate Detection](.github/workflows/duplicate-detection.yml)
- Restricted label enforcement: [Restricted Label Watchdog](.github/workflows/restricted-label-watchdog.yml)
- Reopen behavior integration: [Issue Reopen Handler](.github/workflows/issue-reopen-handler.yml)

### Challenge System (When You Disagree With a Closure)

Use this when an issue was marked duplicate or closed and you believe it was incorrect.

Contributor path:
1. Add the challenge label to the issue.
2. Or comment with a challenge command such as /challenge, start challenge, or mention-style challenge text.
3. Include specific reasons why your issue is unique.

What automation does:
- Moves issue status to Challenged.
- Temporarily removes duplicate-related labels during review.
- Reopens the issue for review if needed.
- Posts an automated challenge-status comment.

Maintainer resolution:
- Challenge upheld: apply challenge-upheld or use comment command such as /challenge-upheld.
- Challenge failed: apply challenge-failed or use comment command such as /challenge-failed followed by a reason.

Typical result:
- Upheld challenges return the issue to active triage.
- Failed challenges archive the issue in Failed Challenges and keep closure intent.

### Duplicate Detection and Admin Override Labels

Duplicate detection runs on new issues, selected label events, comments, and scheduled retraining.

Auto outcomes:
- Potential duplicate labels may be added for human review.
- Parent-child duplicate relationships can be established.
- Parent issues get an aggregated duplicate list section.

Admin override labels:
- adminduplicate: confirms duplicate, closes issue, links parent-child, updates project status, records training data.
- adminduplicatenegative: confirms not-duplicate, removes auto duplicate labels, records negative training data.

Manual bot commands for maintainers:
- Duplicate check trigger: comment with a bot mention plus wording like test duplicate, check duplicate, or scan duplicate.
- Duplicate list refresh trigger: comment with a bot mention plus wording like refresh duplicates, update duplicates, or rebuild duplicates.

### Restricted Label Watchdog

The label watchdog protects sensitive labels from unauthorized edits.

Restricted labels include:
- adminduplicate
- adminduplicatenegative
- challenge-upheld
- challenge-failed
- challenge-complete
- parent
- child
- ai-training
- ai-data
- ai-verified
- training-data
- auto-processed

Policy behavior:
- Unauthorized add: label is removed automatically.
- Unauthorized removal: label is restored automatically.
- Allowed actors: maintainers, admins, and trusted system actors.

What this means in practice:
- Contributors should not rely on manually toggling restricted labels.
- If you need a restricted-state change, request maintainer review in issue comments.

### Recommended Maintainer Playbook

1. For likely duplicates, let automation mark potential duplicates first.
2. Confirm true duplicates with adminduplicate.
3. Mark false positives with adminduplicatenegative.
4. If challenged, resolve with challenge-upheld or challenge-failed and include rationale.
5. If labels seem to revert unexpectedly, check the restricted label watchdog run before reapplying labels.

## Core Links

- 📦 PlatformIO owner page: https://registry.platformio.org/search?q=owner%3Abraydenanderson2014
- 💬 Discussions: https://github.com/braydenanderson2014/C-Arduino-Libraries/discussions
- 🐛 Issues: https://github.com/braydenanderson2014/C-Arduino-Libraries/issues
- 🤝 Contributing: [CONTRIBUTING.md](CONTRIBUTING.md)
- 📜 Code of Conduct: [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md)

## Support

If these libraries help you, support is appreciated:

- ❤️ GoFundMe: https://gofund.me/923e5f10
- 💰 Patreon: https://www.patreon.com/posts/122298248
- ☕ Ko-fi: https://ko-fi.com/joesupercool15673
- 🤝 GitHub Sponsors button in this repository

## Notes

- Legacy long-form status sections were reduced in favor of workflow-generated sources.
- For detailed per-library state, check each library README plus generated reports.
- Expect active changes, especially in experimental and development branches.

