#ifndef FENWICKTREE_H
#define FENWICKTREE_H

#include <SimpleVector.h>   // SimpleVector class
#include <TypeTraits.h>     // is_arithmetic_v, enable_if_t, is_integral_v, is_floating_point_v
#include <Arduino.h>        // Arduino framework
template <typename T>
class FenwickTree {
private:
    static_assert(is_arithmetic<T>::value, "FenwickTree requires a numerical type");
    SimpleVector<T> BIT;
    int n;

public:
    FenwickTree(int n) : BIT(n), n(n) {}
        
    void add(int idx, T delta) {
        for (; idx < n; idx = idx | (idx + 1)){
            BIT[idx] += delta;
        }
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




};
#endif // FENWICKTREE_H