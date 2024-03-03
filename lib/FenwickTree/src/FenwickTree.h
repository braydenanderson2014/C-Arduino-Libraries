#ifndef FENWICKTREE_H
#define FENWICKTREE_H

#include <SimpleVector.h>   // SimpleVector class
#include <TypeTraits.h>     // is_arithmetic_v, enable_if_t, is_integral_v, is_floating_point_v
#include <Arduino.h>        // Arduino framework

template <typename T>
class FenwickTree {
private:
    static_assert(is_arithmetic<T>::value, "FenwickTree requires a numerical type");  // Check if T is a numerical type
    SimpleVector<T> BIT;   // Binary Indexed Tree
    int n; // Size of the array

public:
    /**
     * @brief Construct a new Fenwick Tree object
    */
    FenwickTree(int n) : BIT(n), n(n) {}
        
    /**
     * @brief Updates the Fenwick Tree with a new value at a specific index.
     *
     * This function updates the Fenwick Tree by adding a delta value to the 
     * value at the specified index and all the relevant nodes in the tree.
     * The update is propagated to all the nodes that include idx in their range.
     *
     * @param idx The index at which the new value is to be added. Indexing starts from 0.
     * @param delta The value to be added to the current value at the specified index.
    */
    void add(int idx, T delta) {
        for (; idx < n; idx = idx | (idx + 1)){
            BIT[idx] += delta;
        }
    }

    /**
     * @brief Updates the Fenwick Tree with a new value in a range.
     * 
     * This function updates the Fenwick Tree by adding a delta value to the
     * values in the range [l, r]. The update is propagated to all the nodes
     * that include l and r in their range.
     * 
     * @param l The left index of the range. Indexing starts from 0.
     * @param r The right index of the range. Indexing starts from 0.
     * @param delta The value to be added to the current value in the range [l, r].
    */
    void add(int l, int r, T delta) {
        add(l, delta); // l is the index to start the update
        add(r + 1, -delta); // r + 1 is the next index after r
    }

    T sum(int r) {
        T ret = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1){
            ret += BIT[r];
        }
        return ret;
    }

    T sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }

    void scale(T c){
        for(int i = 0; i < n; i++){
            BIT[i] *= c;
        }
    }

   int find(T x) {
        int idx = 0;
        for(int k = n; k > 0; k >>= 1) {
            if(idx + k <= n && BIT[idx + k] <= x) {
                x -= BIT[idx + k];
                idx += k;
            }
        }
        return idx;
    }

    void range_update(int l, int r, T val) {
        add(l, val);
        add(r + 1, -val);
    }

    T range_query(int l, int r) {
        return sum(r) - sum(l - 1); 
    }

    T point_query(int idx) {
        return sum(idx);
    }

    void point_update(int idx, T val) {
        range_update(idx, idx, val - point_query(idx));
    }

    void clear() {
        BIT.clear();
    }

    int size() {
        return n;
    }

    bool empty() {
        return n == 0;
    }

    void remove(int i, T val) {
        add(i, -val);
    }
};
#endif // FENWICKTREE_H