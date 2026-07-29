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

## Notes
- The tree stores unique keys (duplicate inserts are ignored).
- `find`, `findMin`, and `findMax` return `T()` when no value is available.

## Arduino Library Manager
## ChangeLog:
### Version 1.0.1-ALPHA
- Reworked AVL tree internals to eliminate memory safety bugs and duplicate deletion paths.
- Added compatibility header support (`AVLTree.h` + `avlTree.h`).
- Added/updated host simulation and stress coverage for AVL behavior.

### Version 1.0.0-ALPHA
- Initial untested release.

## PlatformIO Registry
## ChangeLog:
### Version 1.0.1-ALPHA
- Reworked AVL tree internals to eliminate memory safety bugs and duplicate deletion paths.
- Added compatibility header support (`AVLTree.h` + `avlTree.h`).
- Added/updated host simulation and stress coverage for AVL behavior.

### Version 1.0.0-ALPHA
- Initial untested release.
