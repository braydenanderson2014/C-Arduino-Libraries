#ifndef SUFFIX_TREE_h
#define SUFFIX_TREE_h

#include <Arduino.h>
#include <Map.h>
struct SuffixTreeNode {
    Map <char, SuffixTreeNode*> children; // map (Uses Hashtable class from braydenanderson2014)
    SuffixTreeNode* suffixLink; // pointer to the suffix link
    int start; // start index of the suffix for the path
    int* end; // end index of the suffix for the path
    int suffixIndex; // index of the suffix
};

class SuffixTree {
    String text; // input string
    SuffixTreeNode* root = nullptr; // root of the suffix tree
    SuffixTreeNode* lastNewNode = nullptr; // last new node created
    SuffixTreeNode* activeNode = nullptr; // active node
    int activeEdge = -1; // active edge
    int activeLength = 0; // active length
    int remainingSuffixCount = 0; // remaining suffix count
    int leafEnd = -1; // end of a suffix
    int* rootEnd = nullptr; // end of the root
    int* splitEnd = nullptr; // split end
    int size = -1; // size of the input string

public:
    SuffixTree(String txt){
        text = txt;
        size = text.length();
        rootEnd = new int(-1);
        root = new SuffixTreeNode();
        activeNode = root;
        for(int i = 0; i < size; i++){
            extendSuffixTree(i);
        }
        int labelHeight = 0;
        setSuffixIndexByDFS(root, labelHeight);
    }
    
    void extendSuffixTree(int pos){ 
        leafEnd = pos;
        remainingSuffixCount++;
        lastNewNode = nullptr;
        while(remainingSuffixCount > 0){
            if(activeLength == 0){
                activeEdge = pos;
            }
            if(activeNode->children[text[activeEdge]] == nullptr){
                activeNode->children[text[activeEdge]] = new SuffixTreeNode();
                if(lastNewNode != nullptr){
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
            } else {
                SuffixTreeNode* next = activeNode->children[text[activeEdge]];
                if(walkDown(next)){
                    continue;
                }
                if(text[next->start + activeLength] == text[pos]){
                    if(lastNewNode != nullptr && activeNode != root){
                        lastNewNode->suffixLink = activeNode;
                        lastNewNode = nullptr;
                    }
                    activeLength++;
                    break;
                }
                splitEnd = new int(next->start + activeLength - 1);
                SuffixTreeNode* split = new SuffixTreeNode();
                split->start = next->start;
                split->end = new int(*splitEnd);
                activeNode->children[text[activeEdge]] = split;
                split->children[text[pos]] = new SuffixTreeNode();
                next->start += activeLength;
                split->children[text[next->start]] = next;
                if(lastNewNode != nullptr){
                    lastNewNode->suffixLink = split;
                }
                lastNewNode = split;
            }
            remainingSuffixCount--;
            if(activeNode == root && activeLength > 0){
                activeLength--;
                activeEdge = pos - remainingSuffixCount + 1;
            } else if(activeNode != root){
                activeNode = activeNode->suffixLink;
            }
        }
    }

    int edgeLength(SuffixTreeNode* n){
        if(n == root){
            return 0;
        }
        return *(n->end) - n->start + 1;
    }
    
    int walkDown(SuffixTreeNode* currNode){
        if(activeLength >= edgeLength(currNode)){
            activeEdge += edgeLength(currNode);
            activeLength -= edgeLength(currNode);
            activeNode = currNode;
            return 1;
        }
        return 0;
    }

    void setSuffixIndexByDFS(SuffixTreeNode* n, int labelHeight){
        if(n == nullptr){
            return;
        }
        int leaf = 1;
        for(auto i = n->children.begin(); i != n->children.end(); ++i){
            if(i->second != nullptr){
                leaf = 0; 
                setSuffixIndexByDFS(i->second, labelHeight + edgeLength(i->second));
            }
        }
        if(leaf == 1){
            n->suffixIndex = size - labelHeight;
        }
    }

    void freeSuffixTreeByPostOrder(SuffixTreeNode* n){
        if(n == nullptr){
            return;
        }
        for(Map<char, SuffixTreeNode*>::Iterator i = n->children.begin(); i != n->children.end(); ++i){
            if(i->second != nullptr){
                freeSuffixTreeByPostOrder(i->second);
            }
        }
        delete n;   
    }

    void buildSuffixTree(){
        for(int i = 0; i < size; i++){
            extendSuffixTree(i);
        }
        int labelHeight = 0;
        setSuffixIndexByDFS(root, labelHeight);
    }

    void printTree(SuffixTreeNode* n, int labelHeight){
        if(n == nullptr){
            return;
        }
        for(auto i : n->children){
            if(i.second != nullptr){
                printTree(i.second, labelHeight + edgeLength(i.second));
            }
        }
    }

    void doDFS(SuffixTreeNode* n, int labelHeight){
        if(n == nullptr){
            return;
        }
        int leaf = 1;
        for(auto i : n->children){
            if(i.second != nullptr){
                leaf = 0;
                doDFS(i.second, labelHeight + edgeLength(i.second));
            }
        }
        if(leaf == 1){
            n->suffixIndex = size - labelHeight;
        }
    }

    void getLongestRepeatedSubstring(){
        int labelHeight = 0;
        printTree(root, labelHeight);
        doDFS(root, labelHeight);
    }
};

#endif // SUFFIX_TREE_h
