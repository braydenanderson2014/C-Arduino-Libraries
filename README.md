# **C-Arduino-Libraries**  
🚀 **A collection of optimized C++ libraries for Arduino and embedded systems**  

## 📁 **Project Structure**  
All libraries are located under the `lib` folder. For best results, open the **parent folder** containing this `README.md` to access the full project.  

- 📌 The **`src`** folder contains the main project file (`main.cpp`) for testing libraries.  
- 🧪 The **`Test`** folder allows unit testing using the **Unity** framework. Enable it in `platformio.ini` when needed.  
- 📖 Each library has its own README file with setup instructions.  
- 🔍 Some libraries are also available on **Arduino Library Manager**. See the list below for version details.  

---
## 💖 **Support My Work**  
If you find these libraries useful, consider supporting me:  

🔗 **[Donate via GoFundMe](https://gofund.me/923e5f10)**  
💰 **Click the Sponsor Button** at the top of this repository!  

---
## ⚠ **Library Testing & Stability**  
Most libraries are **[PARTIALLY-TESTED]** or **[MOSTLY-TESTED]**. Even **[STABLE]** libraries may contain unknown bugs. If you encounter issues, **please report them** on this repository.  

⚠ **Note:** Some libraries have different published names. For example:  
- **Properties** → Published as **SimpleProperties**  
- **iostream** → Published as **BasicIOStream**  

📌 **[Development Branch]:** Libraries here are in **active development** and may not compile. Use **main** branch versions for stable releases.

---
## 📚 **Available Libraries**  
* [Dictionary](#dictionary-class) [DEVELOPMENT-COMPLETE] (PURE VIRTUAL : No Concrete Implementation in this Library), [TESTING-NOT-NECESSARY],[IMPLEMENTED IN ANOTHER LIBRARY]
* [Hashtable](#hashtable-class) [PASSIVE-DEVELOPMENT], [TESTED], [STABLE]
* [iostream](#iostream-class) [PASSIVE-DEVELOPMENT] (Published as : BasicIOStream), [PARTIALLY-TESTED], [STABLE]
* [iterator](#iterator-class) [DEVELOPMENT-COMPLETE], (PURE VIRTUAL : No Concrete Implementation in this Library)  [TESTING-NOT-NECESSARY] 
* [Properties](#properties-class) [PASSIVE-DEVELOPMENT] (Published as : SimpleProperties), [MOSTLY-TESTED], [STABLE]
* [SimpleVector](#simplevector-class) [PASSIVE-DEVELOPMENT] , [TESTED], [STABLE]
* [Timer](#timer-class) [PASSIVE_DEVELOPMENT] (Published as : SimpleArduinoTimer), [MOSTLY-TESTED], [SEMI-STABLE]
* [UnorderedMap](#unorderedmap-class) [PASSIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [LinkedList](#linkedlist-class) [PASSIVE-DEVELOPMENT] (Published as : BasicLinkedList), [PARTIALLY-TESTED], [STABLE]
* [DoubleLinkedList](#doublelinkedlist-class) [PASSIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [MyDictionary](#mydictionary-class) [ACTIVE-DEVELOPMENT],(implemented Version of the Dictionary Class), [UNTESTED]
* [SDList](#sdlist-class) [PASSIVE-DEVELOPMENT], [REIMPLEMENTED], [MOSTLY-TESTED], [UNSTABLE]
* [ArrayList](#arraylist-class) [PASSIVE-DEVELOPMENT], [TESTED], [STABLE]
* [SimpleMutex](#simplemutex-class) [ACTIVE-DEVELOPMENT]
* [SimpleThreadManager](#simplethreadmanager-class) [ACTIVE-DEVELOPMENT]
* [Operators](#operators-class) [PASSIVE-DEVELOPMENT], [STABLE]
* [Predicates](#predicates-class) [PASSIVE-DEVELOPMENT], [STABLE]
* [TypeTraits](#typetraits-library) [PASSIVE-DEVELOPMENT], [STABLE]
* [Numeric_Limits](#numeric-limits-library-for-arduino) [PASSIVE-DEVELOPMENT], [STABLE]
* [Map](#map-library-for-arduino) [ACTIVE-DEVELOPMENT], [STABLE], [PARTIALLY-TESTED]
* [Queue](#queue-library-for-arduino) [PASSIVE-DEVELOPMENT], [STABLE]
* [Stack](#stack-library-for-arduino) [PASSIVE-DEVELOPMENT], [STABLE]
* [Predicates](#predicates-class) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [Operators](#operators-class) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [MatrixMath](#matrixmath-library-for-arduino) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [MathLib](#mathlib-for-arduino) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [ColorManager](#colormapper-library) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [InitializerList](#initializer-list-library-for-arduino) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [Optional]() [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED]
* [Variant]() [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED]
* [ExtremeVariant]() [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED]


Total Available Libraries: (45)
Total Libraries: (51)

### 🏆 **Stable & Tested Libraries** ✅  
| Library | Status | Notes |
|---------|--------|-------|
| [Dictionary](#dictionary-class) | ✅ DEVELOPMENT-COMPLETE | Pure Virtual (No Concrete Implementation) |
| [Hashtable](#hashtable-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [SimpleVector](#simplevector-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [ArrayList](#arraylist-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, ⚠ SEMI-STABLE |
| [Timer](#timer-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ MOSTLY-TESTED, ⚠ SEMI-STABLE |
| [LinkedList](#linkedlist-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ PARTIALLY-TESTED, 🏆 STABLE |
| [UnorderedMap](#unorderedmap-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ PARTIALLY-TESTED, 🏆 STABLE |
| [Stack](#stack-library-for-arduino) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [Queue](#queue-library-for-arduino) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [Operators](#operators-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [Predicates](#predicates-class) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |

---

### 🔬 **Active Development Libraries** ⚡  
| Library | Status | Notes |
|---------|--------|-------|
| [MyDictionary](#mydictionary-class) | 🔥 ACTIVE-DEVELOPMENT | ⚠ UNTESTED |
| [SimpleMutex](#simplemutex-class) | 🔥 ACTIVE-DEVELOPMENT | 🚧 IN PROGRESS |
| [SimpleThreadManager](#simplethreadmanager-class) | 🔥 ACTIVE-DEVELOPMENT | 🚧 IN PROGRESS |
| [JSON](#json-library) | 🔥 ACTIVE-DEVELOPMENT | 🚧 UNPUBLISHED |
| [MemoryManager](#memory-manager) | 🔥 ACTIVE-DEVELOPMENT | 🚧 UNPUBLISHED |
| [Optional](#optional-library) | 🔥 ACTIVE-DEVELOPMENT | ✅ MOSTLY-TESTED |
| [Variant](#variant-library) | 🔥 ACTIVE-DEVELOPMENT | ✅ MOSTLY-TESTED |

---
## 🧪 **Currently Testing Libraries** 🔍  
🚧 These libraries are under active testing:  
1. [MatrixMath](#matrixmath-library-for-arduino)  
2. [MathLib](#mathlib-for-arduino)  
3. [JSON]
4. [Optional]
5. [Variant]
6. [ExtremeVariant]
7. [ArrayList]
8. [SimpleVector]
---

## 🚧 **Unpublished Libraries (In Development)**  
📢 These libraries are **not yet available** but are under **active development**:  

| Library | Status |
|---------|--------|
| SimpleMutex | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| SimpleThreadManager | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| OrderedMap | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| JSON | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| STRING | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| MemoryManager | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |

---

## 📜 **Libraries Published on Arduino Library Manager**  

- **ArrayList** `[ON-PAR]`  
- **SimpleVector** `[ON-PAR]`  
- **Hashtable** `[ON-PAR]`  
- **Properties** (SimpleProperties)  
- **Timer** (ArduinoTimer)  
- **LinkedList** (ArduinoLinkedList)  
- **UnorderedMap**  
- **DoubleLinkedList** (ArduinoDoubleLinkedList)  
- **SDList** (Arduino SDList) `[ON-PAR]`  
- **Optional** (Will be synced between Versions shortly (made a mistake))  
- **Variant** `[ON-PAR]`  

## 📢 **Contributing & Issues**  
🛠 Found a bug? Have a question? **Open an issue** on GitHub!  

💡 Want to contribute? Fork the repository and submit a **pull request** with your changes.  

---

## 📜 **Final Notes**  
- **Libraries marked as `[ON-PAR]`** have the same version in **Arduino Library Manager** and **PlatformIO**.  
- **If a library is not `[ON-PAR]`**, the **Arduino version may be outdated**. Updates on **PlatformIO** happen first due to ease of deployment.  

🚀 **Happy coding & thank you for using my libraries!** 🚀  

---

## 🎯 **Quick Links**  
🔗 **[GoFundMe - Support My Work](https://gofund.me/923e5f10)**  
🔗 **[GitHub Repository](#)**  
🔗 **[Arduino Library Manager](#)**  

## ALL LIBRARIES INCLUDE A README.md ##

## 📊 **PlatformIO Leaderboard for These Libraries**  
📅 **Updated:** *01/29/2025 09:08 PM Mountain Time*  

| Library Name | Position | Downloads MTD | Total Downloads |
|-------------|----------|---------------|----------------|
| ArrayList | 1,100 | 26 | 174 |
| Dictionary | 14,128 | 0 | 16 |
| SimpleArduinoTimer | 3,947 | 2 | 16 |
| UnorderedMap | 3,952 | 2 | 46 |
| SimpleProperties | 3,948 | 2 | 46 |
| Iterator | 3,949 | 2 | 25 |
| BasicIO | 1,679 | 12 | 360 |
| SDList | 3,951 | 2 | 12 |
| Hashtable | 1,375 | 17 | 183 |
| SimpleVector | 1,259 | 21 | 247 |
| MyDictionary | 14,136 | 0 | 3 |
| Operators | 14,221 | 0 | 15 | 
| Predicates | 14,222 | 0 | 15 |
| BasicLinkedList | 2,818 | 4 | 20 |
| DoubleLinkedList | 3,250 | 3 | 21 |
| TypeTraits | 3,980 | 2 | 28 |
| AVLTree | 14,422 | 0 | 4 |
| BinarySearchTree | 14,423 | 0 | 4 |
| BinaryTree | 14,424 | 0 | 4 |
| B+ Tree | 14,425 | 0 | 4 |
| B Tree | 14,426 | 0 | 4 |
| ColorManager | 14,427 | 0 | 5 |
| FenwickTree | 14,428 | 0 | 4 |
| HeapTree | 14,429 | 0 | 4 |
| InitializerList | 1,379 | 18 | 217 |
| IntervalTree | 14,430 | 0 | 4 |
| KDimensionalTree | 14,431 | 0 | 4 |
| MathLib | 3,982 | 2 | 21 |
| Map | 2,726 | 1 | 38 |
| MatrixMath | 14,444 | 0 | 4 |
| Numeric_Limits | 3,983 | 2 | 25 |
| OCTree | 14,432 | 0 | 4 |
| QuadTree | 14,436 | 0 | 4 |
| Queue | 14,437 | 0 | 5 |
| RedBlackTree | 14,438 | 0 | 4 |
| RTree | 14,439 | 0 | 4 |
| SegmentTree | 14,440 | 0 | 4 |
| Stack | 14,441 | 0 | 6 |
| SuffixTree | 14,442 | 0 | 4 |
| TrieTree | 14,443 | 0 | 4 | 
| Optional | [NEW] | 0 | 0 |
| Variant | [NEW] | 0 | 0 | 
| ExtremeVariant | [NEW] | 0 | 0 |

