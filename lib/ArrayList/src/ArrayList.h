#ifndef ARRAYLIST_H
#define ARRAYLIST_H
/**
 * @author Brayden Anderson
 * @date 01/29/2025
 */
#include <Arduino.h>
#include "../../TypeTraits/src/TypeTraits.h"

//#define IKnowWhatIAmDoing   // Uncomment to unlock advanced memory-control functions; USE AT YOUR OWN RISK
//#define SkinnyArray         // Uncomment to remove memory-intensive features and save flash/RAM on constrained devices
//#define AL_NO_SERIAL        // Uncomment to suppress all Serial output from this library (useful when Serial is not initialized)

// PlatformIO users can also set any of the above via build_flags in platformio.ini, e.g.:
//   build_flags = -DSkinnyArray -DAL_NO_SERIAL
//   build_flags = -DSkinnyArray -DOverrideSort

/**
 * @attention When SkinnyArray is defined, the directives below can selectively restore
 *            individual feature groups.  They can be defined in your code or passed as
 *            -D flags via your build system (e.g. PlatformIO build_flags).
 */
#ifdef SkinnyArray
//#define OverrideCopyConstructor    // Restore copy constructor
//#define OverrideAssignmentOperator // Restore assignment operator
//#define OverrideSort               // Restore all sorting functions
//#define OverrideAddAll             // Restore addAll() functions
//#define OverrideInsertAll          // Restore insertAll() functions
//#define OverrideSpecialtyRemove    // Restore removeIf / removeRange / retainAll
//#define OverrideUtilityFunctions   // Restore forEach / toArray / clone / sublist / ensureCapacity / trimToSize
#endif

#define AL_RESIZE_SUCCESS 0
#define AL_RESIZE_ERROR 1
#define AL_RESIZE_ERROR2 2
#define AL_FIXED_SIZE_ERROR 3
#define AL_BUBBLESORT_FAILURE 4
#define AL_QUICKSORT_FAILURE 5
#define AL_MERGESORT_FAILURE 6
#define AL_SORT_SUCCESS 7
#define AL_ADD_ERROR 8
#define AL_ELEMENT_NOT_FOUND 9
#define AL_OUT_OF_BOUNDS 10
#define AL_INSERT_ERROR 11
#define AL_REMOVE_ERROR 12
#define AL_MAX_CAPACITY_ERROR 13


template <typename T>
class ArrayList {
public:
    enum SizeType { FIXED, DYNAMIC, DYNAMIC2 };

#if !defined(SkinnyArray) || defined(OverrideSort)
    enum SortAlgorithm { BUBBLE_SORT, QUICK_SORT, MERGE_SORT };
#endif

    // ─── Constructors / Destructor ──────────────────────────────────────────

    /**
     * @brief Constructs a new ArrayList.
     *
     * @param type        FIXED (no resize), DYNAMIC (2x resize), or DYNAMIC2 (1.5x resize, default).
     * @param initialSize Initial capacity (default 8).
     */
    ArrayList(SizeType type = DYNAMIC2, size_t initialSize = 8)
                : arrayCapacity(initialSize), count(0), initialSize(initialSize), sizeType(type) {
        array = new T[arrayCapacity];
        if (!array) {
#ifndef AL_NO_SERIAL
            Serial.println("[ArrayList] Memory allocation failed");
#endif
        }
#if !defined(SkinnyArray) || defined(OverrideSort)
        sortAlgorithm = MERGE_SORT;
#endif
    }

#if !defined(SkinnyArray) || defined(OverrideCopyConstructor)
    /**
     * @brief Copy constructor — performs a deep copy of another ArrayList.
     */
    ArrayList(const ArrayList<T>& list) {
        this->sizeType       = list.sizeType;
        this->arrayCapacity  = list.arrayCapacity;
        this->count          = list.count;
        this->initialSize    = list.initialSize;
        this->array          = new T[this->arrayCapacity];
        for (size_t i = 0; i < list.count; ++i) {
            this->array[i] = list.array[i];
        }
#if !defined(SkinnyArray) || defined(OverrideSort)
        this->sortAlgorithm = list.sortAlgorithm;
#endif
    }
#endif

    /**
     * @brief Destructor — frees the internal array.
     */
    ~ArrayList() {
        delete[] array;
    }

    // ─── Add ────────────────────────────────────────────────────────────────

    /**
     * @brief Adds an item to the end of the ArrayList.
     *
     * If the list is full and the size type is DYNAMIC or DYNAMIC2 it resizes automatically.
     * If the size type is FIXED and the list is full, the item is silently dropped.
     *
     * @param item The item to add.
     */
    void add(T item) {
        if (sizeType == FIXED && count >= arrayCapacity) {
            return;
        }
        if (verifyResizeNeeded(1)) {
            resize();
        }
        if (count < arrayCapacity) {
            array[count++] = item;
        }
    }

#if !defined(SkinnyArray) || defined(OverrideAddAll)
    /**
     * @brief Adds all items from another ArrayList to this ArrayList.
     *
     * Uses element-by-element assignment, which correctly handles types with
     * non-trivial copy semantics (e.g. Arduino String).
     *
     * @param other The source ArrayList.
     * @return true if all items were added, false if there was insufficient capacity.
     */
    bool addAll(const ArrayList<T>& other) {
        if (verifyResizeNeeded(other.count)) {
            resize();
        }
        if (count + other.count <= arrayCapacity) {
            for (size_t i = 0; i < other.count; i++) {
                array[count + i] = other.array[i];
            }
            count += other.count;
            return true;
        }
        return false;
    }

    /**
     * @brief Adds all items from a raw array to this ArrayList.
     *
     * Uses element-by-element assignment, which correctly handles types with
     * non-trivial copy semantics (e.g. Arduino String).
     *
     * @param other  Pointer to the source array.
     * @param length Number of elements to add.
     * @return true if all items were added, false if there was insufficient capacity.
     */
    bool addAll(const T* other, size_t length) {
        if (verifyResizeNeeded(length)) {
            resize();
        }
        if (count + length <= arrayCapacity) {
            for (size_t i = 0; i < length; i++) {
                array[count + i] = other[i];
            }
            count += length;
            return true;
        }
        return false;
    }
#endif

    // ─── Insert ─────────────────────────────────────────────────────────────

    /**
     * @brief Inserts an item at a specific index.
     *
     * @param index Target index (must be <= count).
     * @param item  The item to insert.
     * @return true on success, false if the index is out of range.
     */
    bool insert(size_t index, T item) {
        if (index > count) {
            return false;
        }
        if (verifyResizeNeeded(1)) {
            resize();
        }
        for (size_t i = count; i > index; --i) {
            array[i] = array[i - 1];
        }
        array[index] = item;
        ++count;
        return true;
    }

#if !defined(SkinnyArray) || defined(OverrideAssignmentOperator)
    /**
     * @brief Assignment operator — deep-copies another ArrayList into this one.
     */
    ArrayList<T>& operator=(const ArrayList<T>& list) {
        if (this == &list) {
            return *this;
        }
        delete[] this->array;
        this->arrayCapacity = list.arrayCapacity;
        this->array         = new T[this->arrayCapacity];
        this->sizeType      = list.sizeType;
        this->initialSize   = list.initialSize;
        this->count         = list.count;
        for (size_t i = 0; i < list.count; i++) {
            this->array[i] = list.array[i];
        }
        return *this;
    }
#endif

    /**
     * @brief Index operator — returns a reference to the element at the given index.
     *
     * @warning If the index is out of bounds, a reference to an internal static default value
     *          is returned.  The default is reset to T() on each out-of-bounds call to prevent
     *          corruption across calls.  Do not store this reference beyond the current statement.
     *
     * @param index The index of the element.
     * @return Reference to the element, or to an internal reset default if out of bounds.
     */
    T& operator[](size_t index) {
        if (index >= count) {
            static T defaultValue;
            defaultValue = T();
            return defaultValue;
        }
        return array[index];
    }

#if !defined(SkinnyArray) || defined(OverrideInsertAll)
    /**
     * @brief Inserts all items from another ArrayList at a specific index.
     *
     * Uses element-by-element assignment, which correctly handles types with
     * non-trivial copy semantics (e.g. Arduino String).
     *
     * @param index The insertion point (must be <= count).
     * @param other The source ArrayList.
     * @return true on success, false if the index is out of range.
     */
    bool insertAll(size_t index, const ArrayList<T>& other) {
        if (index > count) {
            return false;
        }
        if (verifyResizeNeeded(other.count)) {
            resize();
        }
        for (size_t i = count + other.count; i > index + other.count; --i) {
            array[i - 1] = array[i - other.count - 1];
        }
        for (size_t i = 0; i < other.count; i++) {
            array[index + i] = other.array[i];
        }
        count += other.count;
        return true;
    }

    /**
     * @brief Inserts all items from a raw array at a specific index.
     *
     * Uses element-by-element assignment, which correctly handles types with
     * non-trivial copy semantics (e.g. Arduino String).
     *
     * @param index  The insertion point (must be <= count).
     * @param other  Pointer to the source array.
     * @param length Number of elements to insert.
     * @return true on success, false if the index is out of range.
     */
    bool insertAll(size_t index, const T* other, size_t length) {
        if (index > count) {
            return false;
        }
        if (verifyResizeNeeded(length)) {
            resize();
        }
        for (size_t i = count + length - 1; i >= index + length; --i) {
            array[i] = array[i - length];
        }
        for (size_t i = 0; i < length; i++) {
            array[index + i] = other[i];
        }
        count += length;
        return true;
    }
#endif

    // ─── Remove ─────────────────────────────────────────────────────────────

    /**
     * @brief Removes the first occurrence of a value from the ArrayList.
     *
     * @param item The value to remove.
     * @return true if the item was found and removed, false otherwise.
     */
    bool removeItem(T item) {
        size_t index = indexOf(item);
        if (index != static_cast<size_t>(-1)) {
            remove(index);
            return true;
        }
        return false;
    }

    /**
     * @brief Removes the element at a specific index.
     *
     * @param index The index of the element to remove.
     */
    void remove(size_t index) {
        if (index < count) {
            for (size_t i = index; i < count - 1; ++i) {
                array[i] = array[i + 1];
            }
            --count;
        }
    }

#if !defined(SkinnyArray) || defined(OverrideSpecialtyRemove)
    /**
     * @brief Removes all elements for which a predicate returns true.
     *
     * @param predicate A function taking T and returning bool.
     * @return true if at least one element was removed, false otherwise.
     */
    bool removeIf(bool (*predicate)(T)) {
        size_t removed = 0;
        for (size_t i = 0; i < count; ++i) {
            if (predicate(array[i])) {
                ++removed;
            } else {
                array[i - removed] = array[i];
            }
        }
        if (removed > 0) {
            count -= removed;
            return true;
        }
        return false;
    }

    /**
     * @brief Removes all elements in the range [fromIndex, toIndex).
     *
     * @param fromIndex Start index (inclusive).
     * @param toIndex   End index (exclusive).
     */
    void removeRange(size_t fromIndex, size_t toIndex) {
        if (fromIndex > toIndex || toIndex > count) {
            return;
        }
        size_t removed = toIndex - fromIndex;
        for (size_t i = fromIndex; i < count - removed; ++i) {
            array[i] = array[i + removed];
        }
        count -= removed;
    }

    /**
     * @brief Retains only the elements that are also present in another ArrayList.
     *
     * Elements not found in @p other are removed from this list.
     *
     * @param other The ArrayList of values to retain.
     * @return true if any elements were removed, false otherwise.
     */
    bool retainAll(const ArrayList<T>& other) {
        size_t removed = 0;
        for (size_t i = 0; i < count; ++i) {
            if (!other.contains(array[i])) {
                ++removed;
            } else {
                array[i - removed] = array[i];
            }
        }
        if (removed > 0) {
            count -= removed;
            return true;
        }
        return false;
    }
#endif

    // ─── Modify ─────────────────────────────────────────────────────────────

    /**
     * @brief Removes all elements from the ArrayList.
     *
     * Resets the element count to zero without reallocating the backing array,
     * preserving the current capacity.
     */
    void clear() {
        count = 0;
    }

    // ─── Query & Access ──────────────────────────────────────────────────────

    /**
     * @brief Returns the element at the given index by value.
     *
     * @param index The index.
     * @return The element, or a default-constructed T if the index is out of bounds.
     */
    T get(size_t index) const {
        if (index < count) {
            return array[index];
        }
        return T();
    }

    /**
     * @brief Returns a mutable reference to the element at the given index.
     *
     * @warning If the index is out of bounds, a reference to an internal static default value
     *          is returned.  The default is reset to T() on each out-of-bounds call to prevent
     *          corruption across calls.  Do not store this reference beyond the current statement.
     *
     * @param index The index.
     * @return Mutable reference to the element, or to an internal reset default if out of bounds.
     */
    T& getReference(size_t index) {
        if (index < count) {
            return array[index];
        }
        static T defaultValue;
        defaultValue = T();
        return defaultValue;
    }

    /**
     * @brief Returns a const reference to the element at the given index.
     *
     * @param index The index.
     * @return Const reference to the element, or to an internal static default if out of bounds.
     */
    const T& getReference(size_t index) const {
        if (index < count) {
            return array[index];
        }
        static const T defaultValue = T();
        return defaultValue;
    }

    /**
     * @brief Returns the element at the given index as an Arduino String.
     *
     * @param index The index.
     * @return String representation of the element.
     */
    String getAsString(size_t index) const {
        if (index < count) {
            return toString(array[index]);
        }
        return toString(T());
    }

    /**
     * @brief Converts a value of type T to an Arduino String.
     *
     * @param value The value to convert.
     * @return String representation of the value.
     */
    String toString(const T& value) {
        return String(value);
    }

    /**
     * @brief Checks whether the ArrayList contains a given item.
     *
     * @param item The value to search for.
     * @return true if found, false otherwise.
     */
    bool contains(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns the index of the first occurrence of an item.
     *
     * @param item The value to find.
     * @return The zero-based index, or (size_t)-1 if not found.
     */
    size_t indexOf(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return i;
            }
        }
        return static_cast<size_t>(-1);
    }

    /**
     * @brief Returns the current capacity (maximum elements before a resize is needed).
     */
    size_t capacity() const {
        return arrayCapacity;
    }

    /**
     * @brief Returns the number of elements currently stored.
     */
    size_t size() const {
        return count;
    }

    /**
     * @brief Returns true if the ArrayList contains no elements.
     */
    bool isEmpty() const {
        return count == 0;
    }

    /**
     * @brief Replaces the element at the given index.
     *
     * @param index The index.
     * @param item  The new value.
     * @return true if the index was in bounds and the element was set, false otherwise.
     */
    bool set(size_t index, T item) {
        if (index < count) {
            array[index] = item;
            return array[index] == item;
        }
        return false;
    }

#if !defined(SkinnyArray) || defined(OverrideUtilityFunctions)
    /**
     * @brief Applies an operator function to every element, replacing each with the result.
     *
     * @param operatorFunction A function taking T and returning a new T.
     */
    void replaceAll(T (*operatorFunction)(T)) {
        for (size_t i = 0; i < count; ++i) {
            array[i] = operatorFunction(array[i]);
        }
    }

    /**
     * @brief Calls a consumer function for every element in order.
     *
     * @param consumer A void function taking T.
     */
    void forEach(void (*consumer)(T)) const {
        for (size_t i = 0; i < count; ++i) {
            consumer(array[i]);
        }
    }

    /**
     * @brief Copies all elements into a caller-supplied array.
     *
     * Uses element-by-element assignment to correctly handle types with non-trivial
     * copy semantics (e.g. Arduino String).
     *
     * @param outputArray Destination array — must have room for at least size() elements.
     * @return The destination array pointer.
     */
    T* toArray(T* outputArray) const {
        for (size_t i = 0; i < count; i++) {
            outputArray[i] = array[i];
        }
        return outputArray;
    }

    /**
     * @brief Returns a new heap-allocated ArrayList containing elements in [fromIndex, toIndex).
     *
     * @param fromIndex Start index (inclusive).
     * @param toIndex   End index (exclusive).
     * @return Pointer to the new ArrayList, or nullptr if the range is invalid.
     * @warning The caller is responsible for deleting the returned object.
     */
    ArrayList<T>* sublist(size_t fromIndex, size_t toIndex) const {
        if (fromIndex > toIndex || toIndex > count) {
            return nullptr;
        }
        ArrayList<T>* newList = new ArrayList<T>(sizeType, toIndex - fromIndex);
        for (size_t i = fromIndex; i < toIndex; ++i) {
            newList->add(array[i]);
        }
        return newList;
    }

    /**
     * @brief Returns a heap-allocated deep copy of this ArrayList.
     *
     * @return Pointer to the cloned ArrayList.
     * @warning The caller is responsible for deleting the returned object.
     */
    ArrayList<T>* clone() const {
        ArrayList<T>* cloned = new ArrayList<T>(sizeType, arrayCapacity);
        cloned->addAll(*this);
        return cloned;
    }

    /**
     * @brief Returns a heap-allocated deep copy of this ArrayList with a specific capacity.
     *
     * @param InitialCapacity The capacity of the new clone.
     * @return Pointer to the cloned ArrayList.
     * @warning The caller is responsible for deleting the returned object.
     */
    ArrayList<T>* clone(int InitialCapacity) const {
        ArrayList<T>* cloned = new ArrayList<T>(sizeType, InitialCapacity);
        cloned->addAll(*this);
        return cloned;
    }

    /**
     * @brief Ensures the ArrayList can hold at least @p minCapacity elements without resizing.
     *
     * Uses element-by-element assignment to correctly handle types with non-trivial
     * copy semantics (e.g. Arduino String).  If allocation fails, the list is unchanged.
     *
     * @param minCapacity The required minimum capacity.
     */
    void ensureCapacity(size_t minCapacity) {
        if (minCapacity > arrayCapacity) {
            T* newArray = new T[minCapacity];
            if (!newArray) return;
            for (size_t i = 0; i < count; i++) {
                newArray[i] = array[i];
            }
            delete[] array;
            array        = newArray;
            arrayCapacity = minCapacity;
        }
    }

    /**
     * @brief Shrinks the capacity of the ArrayList to exactly its current element count.
     *
     * Only applies to DYNAMIC and DYNAMIC2 lists.  Has no effect when already at minimum
     * capacity or when the list is FIXED.
     * Uses element-by-element assignment to correctly handle types with non-trivial
     * copy semantics (e.g. Arduino String).  If allocation fails, the list is unchanged.
     */
    void trimToSize() {
        if ((sizeType == DYNAMIC || sizeType == DYNAMIC2) && count < arrayCapacity) {
            T* newArray = new T[count];
            if (!newArray) return;
            for (size_t i = 0; i < count; i++) {
                newArray[i] = array[i];
            }
            delete[] array;
            array        = newArray;
            arrayCapacity = count;
        }
    }
#endif

#if !defined(SkinnyArray) || defined(OverrideSort)
    /**
     * @brief Sets the sorting algorithm used by the single-argument sort() overload.
     *
     * @param algorithm BUBBLE_SORT, QUICK_SORT, or MERGE_SORT (default: MERGE_SORT).
     */
    void setSortAlgorithm(SortAlgorithm algorithm = MERGE_SORT) {
        sortAlgorithm = algorithm;
    }

    /**
     * @brief Returns the currently selected sorting algorithm.
     */
    SortAlgorithm getSortAlgorithm() {
        return sortAlgorithm;
    }

    /**
     * @brief Sorts the ArrayList using the currently selected algorithm.
     *
     * @param comparator Returns true when its first argument should come after the second
     *                   in the sorted order.
     */
    void sort(bool (*comparator)(T, T)) {
        switch (sortAlgorithm) {
            case BUBBLE_SORT:
                bubbleSort(comparator);
                break;
            case QUICK_SORT:
                quickSort(comparator);
                break;
            default:
                mergeSort(*this, 0, count - 1);
                break;
        }
    }

    /**
     * @brief Sorts the ArrayList using a specified algorithm.
     *
     * @param comparator Returns true when its first argument should come after the second.
     * @param algorithm  The sorting algorithm to use.
     */
    void sort(bool (*comparator)(T, T), SortAlgorithm algorithm) {
        switch (algorithm) {
            case BUBBLE_SORT:
                bubbleSort(comparator);
                break;
            case QUICK_SORT:
                quickSort(comparator);
                break;
            default:
                mergeSort(*this, 0, count - 1);
                break;
        }
    }

    /**
     * @brief Sorts using Bubble Sort.
     *
     * @param comparator Returns true when the first arg should come after the second.
     */
    void bubbleSort(bool (*comparator)(T, T)) {
        for (size_t i = 0; i < count - 1; ++i) {
            for (size_t j = 0; j < count - i - 1; ++j) {
                if (comparator(array[j], array[j + 1])) {
                    T temp       = array[j];
                    array[j]     = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }

    /**
     * @brief Sorts using Quick Sort.
     *
     * @param comparator Returns true when the first arg should come after the second.
     */
    void quickSort(bool (*comparator)(T, T)) {
        quickSortHelper(comparator, 0, count - 1);
    }

    /**
     * @brief Recursively sorts a subrange using Merge Sort.
     *
     * @note Each merge step allocates temporary heap arrays.  On very memory-constrained
     *       devices prefer BUBBLE_SORT or QUICK_SORT to avoid this overhead.
     *
     * @param list Reference to the ArrayList being sorted.
     * @param l    Left boundary index.
     * @param r    Right boundary index.
     */
    void mergeSort(ArrayList<T>& list, int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(list, l, m);
            mergeSort(list, m + 1, r);
            merge(list, l, m, r);
        }
    }
#endif

#if !defined(SkinnyArray) || defined(OverrideUtilityFunctions)
    /**
     * @brief Sets the resize mode of this ArrayList.
     *
     * @param type FIXED, DYNAMIC, or DYNAMIC2.
     */
    void setSizeType(SizeType type) {
        sizeType = type;
    }

    /**
     * @brief Returns the current resize mode.
     */
    SizeType getSizeType() {
        return sizeType;
    }
#endif

#if !defined(SkinnyArray) || defined(OverrideSort) || defined(OverrideUtilityFunctions)
    /**
     * @brief Returns the initial capacity this ArrayList was constructed with.
     */
    size_t getInitialSize() {
        return initialSize;
    }

    /**
     * @brief Updates the stored initial-size value.
     *
     * @param size The new initial-size value to store.
     */
    void setInitialSize(size_t size) {
        initialSize = size;
    }
#endif

    // ─── Iterators ───────────────────────────────────────────────────────────

    /** @brief Returns a pointer to the first element (enables range-based for loops). */
    T* begin() const {
        return &array[0];
    }

    /** @brief Returns a pointer one past the last element (enables range-based for loops). */
    T* end() const {
        return &array[count];
    }

#ifdef IKnowWhatIAmDoing
    /**
     * @brief Directly overwrites the internal capacity field.
     *
     * @warning This does NOT reallocate the backing array.  Only use this when you have
     *          already replaced the array pointer via other means and know exactly what
     *          you are doing.  Misuse will corrupt memory.
     */
    void setCapacity(size_t capacity) {
        arrayCapacity = capacity;
    }

    /**
     * @brief Directly overwrites the element count.
     *
     * @warning Setting count above the actual number of initialised elements exposes
     *          uninitialised memory.  Use with extreme caution.
     */
    void setCount(size_t count) {
        this->count = count;
    }
#endif

private:
    T*       array;
    size_t   arrayCapacity;
    size_t   count;
    size_t   initialSize;
    SizeType sizeType;
#if !defined(SkinnyArray) || defined(OverrideSort)
    SortAlgorithm sortAlgorithm;
#endif

    // ─── Private: Resize ─────────────────────────────────────────────────────

    /**
     * @brief Returns true when the backing array needs to grow to fit @p spacesNeeded
     *        additional elements.
     */
    bool verifyResizeNeeded(size_t spacesNeeded) {
        return sizeType != FIXED && (count + spacesNeeded > arrayCapacity);
    }

    /**
     * @brief Dispatches to the appropriate resize implementation based on SizeType.
     */
    void resize() {
        if (sizeType == DYNAMIC) {
            resize1();
        } else if (sizeType == DYNAMIC2) {
            resize2();
        }
    }

    /**
     * @brief Grows the backing array to 2x its current capacity (DYNAMIC mode).
     *
     * Uses element-by-element assignment to correctly handle types with non-trivial
     * copy semantics (e.g. Arduino String).  If the allocation fails the list is
     * left unchanged.
     */
    void resize1() {
        size_t newCapacity = arrayCapacity * 2;
        T* newArray = new T[newCapacity];
        if (!newArray) return;
        for (size_t i = 0; i < count; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array        = newArray;
        arrayCapacity = newCapacity;
    }

    /**
     * @brief Grows the backing array to 1.5x its current capacity (DYNAMIC2 mode).
     *
     * Allocates less excess memory than resize1() at the cost of more frequent
     * resizes.  Uses element-by-element assignment.  If the allocation fails the
     * list is left unchanged.
     */
    void resize2() {
        size_t newCapacity = arrayCapacity + (arrayCapacity / 2);
        T* newArray = new T[newCapacity];
        if (!newArray) return;
        for (size_t i = 0; i < count; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array        = newArray;
        arrayCapacity = newCapacity;
    }

#if !defined(SkinnyArray) || defined(OverrideSort)
    // ─── Private: Sort helpers ───────────────────────────────────────────────

    /**
     * @brief Partitions a subarray for Quick Sort and returns the pivot index.
     */
    int partition(bool (*comparator)(T, T), int low, int high) {
        T pivot = array[high];
        int i   = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (comparator(array[j], pivot)) {
                i++;
                swapElements(&array[i], &array[j]);
            }
        }
        swapElements(&array[i + 1], &array[high]);
        return (i + 1);
    }

    /**
     * @brief Recursive Quick Sort driver.
     */
    void quickSortHelper(bool (*comparator)(T, T), int low, int high) {
        if (low < high) {
            int pi = partition(comparator, low, high);
            quickSortHelper(comparator, low, pi - 1);
            quickSortHelper(comparator, pi + 1, high);
        }
    }

    /**
     * @brief Merges two sorted halves of the ArrayList during Merge Sort.
     *
     * Allocates two temporary heap arrays during the merge; they are freed before
     * returning.  If either allocation fails the merge is skipped and the subrange
     * is left in its pre-merge (partially sorted) state.
     */
    void merge(ArrayList<T>& list, int l, int m, int r) {
        int n1 = m - l + 1;
        int n2 = r - m;
        T* L   = new T[n1];
        T* R   = new T[n2];
        if (!L || !R) {
            delete[] L;
            delete[] R;
            return;
        }
        for (int i = 0; i < n1; i++) L[i] = list[l + i];
        for (int j = 0; j < n2; j++) R[j] = list[m + 1 + j];
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                list[k++] = L[i++];
            } else {
                list[k++] = R[j++];
            }
        }
        while (i < n1) list[k++] = L[i++];
        while (j < n2) list[k++] = R[j++];
        delete[] L;
        delete[] R;
    }

    /**
     * @brief Swaps two elements.
     */
    void swapElements(T* a, T* b) {
        T t = *a;
        *a  = *b;
        *b  = t;
    }
#endif
};

#endif // ARRAYLIST_H
