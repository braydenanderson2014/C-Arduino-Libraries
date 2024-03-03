#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <Arduino.h>
#include <TypeTraits.h>
#include <Math.h>
#include <SimpleVector.h>
#include <Numeric_Limits.h>

template <typename T>
class SegmentTree {
    static_assert(is_arithmetic<T>::value, "T must be numeric type");

public:
    SegmentTree(SimpleVector<T>& arr) {
        n = arr.size();
        int x = (int)(ceil(log2(n)));
        int max_size = 2 * (int)pow(2, x) - 1;
        tree = new T[max_size] {};
        lazy = new T[max_size] {};
        build(arr, 0, n - 1, 0);
    }

    ~SegmentTree() {
        delete[] tree;
        delete[] lazy;
    }

    T getSum(int qs, int qe) {
        return getSumUtil(0, n - 1, qs, qe, 0);
    }

    void updateValue(SimpleVector<T>& arr, int i, T new_val) {
        T diff = new_val - arr[i];
        arr[i] = new_val;
        updateValueUtil(0, n - 1, i, diff, 0);
    }

    void updateRange(int us, int ue, T diff) {
        updateRangeUtil(0, n - 1, us, ue, diff, 0);
    }

    T getMin(int qs, int qe) {
      return getMinUtil(0, n - 1, qs, qe, 0);
    }

    T getMax(int qs, int qe) {
      return getMaxUtil(0, n - 1, qs, qe, 0);
    }

private:
    T* tree;
    T* lazy;
    int n;

    T getMinUtil(int ss, int se, int qs, int qe, int si) {
    // If segment of this node is a part of given range, then return the min of the segment
    if (qs <= ss && qe >= se) {
        return tree[si];
    }

    // If segment of this node is outside the given range
    if (se < qs || ss > qe) {
        return numeric_limits<T>::Max();
    }

    // If a part of this segment overlaps with the given range
    int mid = ss + (se - ss) / 2;
    return min(getMinUtil(ss, mid, qs, qe, 2 * si + 1),
               getMinUtil(mid + 1, se, qs, qe, 2 * si + 2));
    }

    T getMaxUtil(int ss, int se, int qs, int qe, int si) {
    // If segment of this node is a part of given range, then return the max of the segment
    if (qs <= ss && qe >= se) {
        return tree[si];
    }

    // If segment of this node is outside the given range
    if (se < qs || ss > qe) {
        return numeric_limits<T>::Min();
      }

      // If a part of this segment overlaps with the given range
      int mid = ss + (se - ss) / 2;
      return max(getMaxUtil(ss, mid, qs, qe, 2 * si + 1),
               getMaxUtil(mid + 1, se, qs, qe, 2 * si + 2));
    }

    void build(SimpleVector<T>& arr, int ss, int se, int si) {
        if (ss == se) {
            tree[si] = arr[ss];
            return;
        }
        int mid = ss + (se - ss) / 2;
        build(arr, ss, mid, si * 2 + 1);
        build(arr, mid + 1, se, si * 2 + 2);
        tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
    }

    T getSumUtil(int ss, int se, int qs, int qe, int si) {
        // If lazy value is present at node, update it before making any Sum query
        if (lazy[si] != 0) {
            tree[si] += (se - ss + 1) * lazy[si];
            if (ss != se) { // Not a leaf node
                lazy[si * 2 + 1] += lazy[si];
                lazy[si * 2 + 2] += lazy[si];
            }
            lazy[si] = 0;
        }

        // Total overlap
        if (qs <= ss && qe >= se) {
            return tree[si];
        }
        // No overlap
        if (se < qs || ss > qe) {
            return 0;
        }
        // Partial overlap
        int mid = ss + (se - ss) / 2;
        return getSumUtil(ss, mid, qs, qe, 2 * si + 1) +
               getSumUtil(mid + 1, se, qs, qe, 2 * si + 2);
    }

    void updateRangeUtil(int ss, int se, int us, int ue, T diff, int si) {
        // Update current node if it has a lazy value
        if (lazy[si] != 0) {
            tree[si] += (se - ss + 1) * lazy[si];
            if (ss != se) { // Not a leaf node, propagate the lazy value to children
                lazy[si * 2 + 1] += lazy[si];
                lazy[si * 2 + 2] += lazy[si];
            }
            lazy[si] = 0;
        }

        // No overlap
        if (ss > se || ss > ue || se < us) {
            return;
        }

        // Total overlap
        if (ss >= us && se <= ue) {
            tree[si] += (se - ss + 1) * diff;
            if (ss != se) { // Not a leaf node, propagate the diff to children
                lazy[si * 2 + 1] += diff;
                lazy[si * 2 + 2] += diff;
            }
            return;
        }

        // Partial overlap
        int mid = ss + (se - ss) / 2;
        updateRangeUtil(ss, mid, us, ue, diff, 2 * si + 1);
        updateRangeUtil(mid + 1, se, us, ue, diff, 2 * si + 2);
        tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
    }

    void updateValueUtil(int ss, int se, int i, T diff, int si) {
        // Base Case: If the input index lies outside the range of this segment
        if (i < ss || i > se) {
            return;
        }

        // If the input index is in range of this node, then update the value of the node and its children
        tree[si] += diff;
        if (se != ss) {
            int mid = ss + (se - ss) / 2;
            updateValueUtil(ss, mid, i, diff, 2 * si + 1);
            updateValueUtil(mid + 1, se, i, diff, 2 * si + 2);
        }        
    }
};

#endif // SEGMENT_TREE_H
