#ifndef TREE_h
#define TREE_h

#include <BinaryTree.h>
#include <suffixTree.h>
#include <avlTree.h>
#include <binarySearchTree.h>
#include <redblackTree.h>
#include <heapTree.h>
#include <bTree.h>
#include <bPlusTree.h>
#include <trieTree.h>
#include <segmentTree.h>
#include <kDimensionalTree.h>
#include <quadTree.h>
#include <fenwickTree.h>
//#include <intervalTree.h>
#include <octree.h>
#include <rTree.h>
//#include <quadTree.h>
//#include <fenwickTree.h>    
// ENUMS:

/**
 * @brief Enum for traversal order
 * 
*/

/**
 * @brief Enum for tree type
 * 
*/
enum TreeType {
    BINARY,
    BINARY_SEARCH,
    AVL,
    RED_BLACK,
    HEAP,
    B_TREE,
    B_PLUS_TREE,
    TRIE,
    SUFFIX_TREE,
    SEGMANT_TREE,
    K_DIMENSIONAL_TREE,
    QUAD_TREE,
    FENWICK_TREE,
    INTERVAL_TREE,
    OCTREE,
    R_TREE
};

template <typename T>
class Tree {
private:
    
};

#endif // TREE_h