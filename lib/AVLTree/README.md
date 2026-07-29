# AVL Tree Library for Arduino

## Overview
This library provides a templated AVL Tree implementation for Arduino. The tree self-balances after inserts/removals to keep operations efficient.

## Features
- Insert and remove values with automatic AVL rebalancing
- Search and membership checks (`find`, `contains`)
- Safe empty-tree handling for `findMin`, `findMax`, and `find`
- In-order, pre-order, and post-order printing via `Serial`
- Utility APIs: `size`, `height`, `getBalance`, `clear`, `isEmpty`
- Compatibility include for both `AVLTree.h` and legacy `avlTree.h`
- Optional error-code tracking via `AVL_TREE_ENABLE_ERROR_CODES` (off by default)

## Usage
```cpp
#include <AVLTree.h>

AVLTree<int> tree;

void setup() {
  Serial.begin(9600);

  tree.insert(10);
  tree.insert(20);
  tree.insert(15);

  if (tree.contains(15)) {
    Serial.println(tree.find(15));
  }

  Serial.println(tree.findMin());
  Serial.println(tree.findMax());

  tree.remove(10);
  tree.inOrder();
}

void loop() {}
```

## Error Code Tracking

Not all microcontrollers have enough resources (or C++ runtime support) for
exception handling, so error reporting is provided as an opt-in feature that
compiles to zero overhead when disabled.

Enable it by defining `AVL_TREE_ENABLE_ERROR_CODES=1` before including the
header or by passing `-DAVL_TREE_ENABLE_ERROR_CODES=1` to the compiler:

```cpp
#define AVL_TREE_ENABLE_ERROR_CODES 1
#include <AVLTree.h>
```

When enabled the following integer error constants are defined:

| Constant           | Value | Meaning                                    |
|--------------------|-------|--------------------------------------------|
| `AVL_ERR_NONE`     | 0     | Operation succeeded                        |
| `AVL_ERR_DUPLICATE`| 1     | `insert` ignored because key already exists|
| `AVL_ERR_NOT_FOUND`| 2     | `remove` / `find` key not in tree          |
| `AVL_ERR_EMPTY`    | 3     | `findMin` / `findMax` on an empty tree     |
| `AVL_ERR_ALLOC`    | 4     | Memory allocation failed                   |

Two additional methods become available:

- `int  getLastError() const` — returns the code set by the most recent operation, or `AVL_ERR_NONE`.
- `void clearLastError()`    — resets the stored code to `AVL_ERR_NONE`.

Every mutating or querying method clears the error to `AVL_ERR_NONE` before it
runs, so you only need to check after the call that matters.

```cpp
#define AVL_TREE_ENABLE_ERROR_CODES 1
#include <AVLTree.h>

AVLTree<int> tree;

void setup() {
  Serial.begin(9600);

  tree.insert(5);
  tree.insert(5); // duplicate
  if (tree.getLastError() == AVL_ERR_DUPLICATE) {
    Serial.println("duplicate ignored");
  }

  tree.remove(99); // not present
  if (tree.getLastError() == AVL_ERR_NOT_FOUND) {
    Serial.println("key not found");
  }

  tree.clear();
  tree.findMin();
  if (tree.getLastError() == AVL_ERR_EMPTY) {
    Serial.println("tree is empty");
  }
}

void loop() {}
```

## Notes
- The tree stores unique keys (duplicate inserts are ignored).
- `find`, `findMin`, and `findMax` return `T()` when no value is available.
- When `AVL_TREE_ENABLE_ERROR_CODES` is 0 (default) the feature compiles away
  completely, adding no flash or RAM overhead.

## Arduino Library Manager
Release notes for the Arduino Library Manager package.
## ChangeLog:
### Version 1.0.2-ALPHA
- Added optional error-code tracking via `AVL_TREE_ENABLE_ERROR_CODES`.
  Disabled by default (zero overhead). When enabled, `getLastError()` and
  `clearLastError()` are available and all mutating/querying operations set a
  lightweight `int` status code.

### Version 1.0.1-ALPHA
- Reworked AVL tree internals to eliminate memory safety bugs and duplicate deletion paths.
- Added compatibility header support (`AVLTree.h` + `avlTree.h`).
- Added/updated host simulation and stress coverage for AVL behavior.

### Version 1.0.0-ALPHA
- Initial untested release.

## PlatformIO Registry
Release notes for the PlatformIO package metadata.
## ChangeLog:
### Version 1.0.2-ALPHA
- Added optional error-code tracking via `AVL_TREE_ENABLE_ERROR_CODES`.
  Disabled by default (zero overhead). When enabled, `getLastError()` and
  `clearLastError()` are available and all mutating/querying operations set a
  lightweight `int` status code.

### Version 1.0.1-ALPHA
- Reworked AVL tree internals to eliminate memory safety bugs and duplicate deletion paths.
- Added compatibility header support (`AVLTree.h` + `avlTree.h`).
- Added/updated host simulation and stress coverage for AVL behavior.

### Version 1.0.0-ALPHA
- Initial untested release.
