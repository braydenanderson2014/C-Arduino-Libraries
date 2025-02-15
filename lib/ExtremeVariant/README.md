# ExtremeVariant

## Overview
**ExtremeVariant** is a versatile container that can store **single elements, lists, or key-value pairs** dynamically. It provides a simple and efficient way to handle different types of data structures using a **unified interface**.

### Key Features:
✅ Supports **single values, lists, and key-value pairs**
✅ Uses **SimpleVector** for lists and **Hashtable** for key-value storage
✅ **Lightweight and optimized** for embedded systems like Arduino & ESP32
✅ Provides **Optional** wrappers to prevent errors when retrieving data

---

## Installation
To use **ExtremeVariant**, include the header file:
```cpp
#include <ExtremeVariant.h>
```

### Dependencies:
Make sure you have the following libraries installed:
- **SimpleVector** (for dynamic arrays)
- **Hashtable** (for key-value storage)
- **Optional** (for safe data handling)

---

## Sponsor This Project ❤️
If you find this project useful, consider supporting its development:
- **Patreon:** [Support on Patreon](https://www.patreon.com/posts/122298248)
- **GoFundMe:** [Donate via GoFundMe](https://gofund.me/923e5f10)
- **GitHub Sponsors:** Click the **"Sponsor"** button on the repository

Your support helps maintain and improve this project! 🚀

---

## API Reference
| Function | Description |
|----------|------------|
| **Constructors & Destructor** | |
| `ExtremeVariant()` | Initializes an empty variant. |
| **Setters** | |
| `void setSingle(const V& value)` | Stores a single value. |
| `void setList(const SimpleVector<V>& values)` | Stores a list of values. |
| `void setKeyValuePairs(const Hashtable<K, V>& pairs)` | Stores key-value pairs. |
| **Getters (Returns Optional Wrappers)** | |
| `Optional<V> getSingle() const` | Retrieves the stored single value (if present). |
| `Optional<SimpleVector<V>> getList() const` | Retrieves the stored list (if present). |
| `Optional<Hashtable<K, V>> getKeyValuePairs() const` | Retrieves key-value pairs (if present). |
| **Utility Functions** | |
| `void clear()` | Clears the stored data. |
| `VariantType getType() const` | Returns the current type of stored data. |

---

## Usage Examples

### **Storing and Retrieving a Single Value**
```cpp
ExtremeVariant<int, String> variant;
variant.setSingle("Hello, World!");
Optional<String> value = variant.getSingle();
if (value.hasValue()) {
    Serial.println(value.get()); // Output: Hello, World!
}
```

### **Storing and Retrieving a List**
```cpp
SimpleVector<int> numbers;
numbers.put(10);
numbers.put(20);
numbers.put(30);

ExtremeVariant<int, int> variant;
variant.setList(numbers);
Optional<SimpleVector<int>> list = variant.getList();
if (list.hasValue()) {
    for (int i = 0; i < list.get().elements(); i++) {
        Serial.println(list.get().get(i)); // Output: 10, 20, 30
    }
}
```

### **Storing and Retrieving Key-Value Pairs**
```cpp
Hashtable<String, int> ageTable;
ageTable.put("Alice", 25);
ageTable.put("Bob", 30);

ExtremeVariant<String, int> variant;
variant.setKeyValuePairs(ageTable);
Optional<Hashtable<String, int>> table = variant.getKeyValuePairs();
if (table.hasValue()) {
    Serial.println(table.get().get("Alice")); // Output: 25
}
```

---

## Change Log

### **Arduino Library Manager**
| Version | Changes |
|---------|---------|
| **v1.0.0** | Initial Release |

### **PlatformIO Registry**
| Version | Changes |
|---------|---------|
| **v1.0.0** | Initial Release |

---

## License
This project is licensed under the **Apache License**. Feel free to use, modify, and contribute! 🚀

## Contributing
Pull requests and feature suggestions are welcome! Open an issue or submit improvements to help enhance the library.

**Happy coding! 🎯**

