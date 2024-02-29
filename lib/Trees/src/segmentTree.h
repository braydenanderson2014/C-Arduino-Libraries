#ifndef SEGMENT_TREE_h
#define SEGMENT_TREE_h

#include <Arduino.h>
#include <TypeTraits.h>
#include <Math.h>

template <typename T>
class SegmentTree {
    static_assert(is_arithmetic<T>::value, "T must be numeric type");
public:
  SegmentTree(int arr[], int n) {
    // Allocate memory for segment tree
    int x = (int)(Ceil(Log2(n))); //Height of segment tree
    int max_size = 2*(int)Pow(2, x) - 1; //Maximum size of segment tree
    tree = new int[max_size];
    build(arr, 0, n - 1, 0);
  }

  int getSum(int n, int qs, int qe) {
    // qs -> query start, qe -> query end
    return getSumUtil(0, n - 1, qs, qe, 0);
  }

  void updateValue(int arr[], int n, int i, int new_val) {
    // i -> index of the element to be updated
    int diff = new_val - arr[i];
    arr[i] = new_val;
    updateValueUtil(0, n - 1, i, diff, 0);
  }

private:
  int *tree;


  void build(int arr[], int ss, int se, int si) {
    if (ss == se) {
      tree[si] = arr[ss];
      return;
    }
    int mid = ss + (se - ss) / 2;
    build(arr, ss, mid, si * 2 + 1);
    build(arr, mid + 1, se, si * 2 + 2);
    tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
  }

  int getSumUtil(int ss, int se, int qs, int qe, int si) {
    if (qs <= ss && qe >= se)
      return tree[si];
    if (se < qs || ss > qe)
      return 0;
    int mid = ss + (se - ss) / 2;
    return getSumUtil(ss, mid, qs, qe, 2 * si + 1) +
           getSumUtil(mid + 1, se, qs, qe, 2 * si + 2);
  }

  void updateValueUtil(int ss, int se, int i, int diff, int si) {
    if (i < ss || i > se)
      return;
    tree[si] = tree[si] + diff;
    if (se != ss) {
      int mid = ss + (se - ss) / 2;
      updateValueUtil(ss, mid, i, diff, 2 * si + 1);
      updateValueUtil(mid + 1, se, i, diff, 2 * si + 2);
    }
  }
};

#endif // SEGMENT_TREE_h