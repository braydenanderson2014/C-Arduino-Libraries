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

- Library health summary: [library-health-report.md](library-health-report.md)
- PlatformIO metrics report: [PLATFORMIO_METRICS.md](PLATFORMIO_METRICS.md)
- Host simulation and compile workflows: `.github/workflows/`

## Feature Snapshot

Expect practical building blocks rather than one giant framework:

- Storage and file friendly patterns: SD workflows (`SDList`) and filesystem-oriented integration points (including `LittleFS` wrapper paths in this repo).
- Modern utility helpers: `Optional`, `Variant`, `TypeTraits`, `Predicates`, and iterator/style support.
- Data structures: maps, trees, lists, queues, stacks, vectors, and hash containers.
- Math and utility helpers: `MathLib`, `MatrixMath`, timers, and lightweight support modules.

For deep details, check each library README and examples under `lib/`.

## Library Health Badges

<!-- HEALTH_BADGES_START -->
Auto-generated health badges will be inserted by workflow.
<!-- HEALTH_BADGES_END -->

<!-- PLATFORMIO_BADGES_START -->
## PlatformIO Registry Badges

Auto-generated from PlatformIO registry package data (2026-07-30 04:28 UTC).

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
- Aggregated health output: [library-health-report.md](library-health-report.md)
- PlatformIO metrics and registry overview: [PLATFORMIO_METRICS.md](PLATFORMIO_METRICS.md)

Notes:
- Compile/testing pass-fail labels are synchronized to SystemCommands from the host simulation workflow jobs.
- Workflow badges reflect the latest run outcome, including `cancelled` when a run is interrupted.
- For debugging failures, open the workflow run and inspect step logs directly.

## Core Links

- 📦 PlatformIO owner page: https://registry.platformio.org/search?q=owner%3Abraydenanderson2014
- 💬 Discussions: https://github.com/braydenanderson2014/C-Arduino-Libraries/discussions
- 🐛 Issues: https://github.com/braydenanderson2014/C-Arduino-Libraries/issues

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

