#ifndef SUFFIX_TREE_h
#define SUFFIX_TREE_h

#include <Arduino.h>
#include <Map.h>
#include <TypeTraits.h>
struct SuffixTreeNode {
    static_assert(is_Char<char>::value || is_String<String>::value, "SuffixTreeNode only supports char or String types");
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

    // Utility function to traverse the tree and find the longest repeated substring
    void getLongestRepeatedSubstringUtil(SuffixTreeNode* node, String& longest, String current) {
        if (node == nullptr) return;

        // Check if this node is an internal node with more than one child
        if (node != root && node->children.size() > 1) {
            // Since it's an internal node, update the longest substring if necessary
            if (current.length() > longest.length()) {
                longest = current;
            }
        }

        for (auto it : node->children) {
            SuffixTreeNode* child = it.second;
            if (child != nullptr) {
                // Construct the substring for this branch
                String nextSubstring = current + text.substring(child->start, *(child->end) + 1);
                getLongestRepeatedSubstringUtil(child, longest, nextSubstring);
            }
        }
    }

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
        for(Map<char, SuffixTreeNode*>::MapIterator i = n->children.begin(); i != n->children.end(); ++i){
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

    void getLongestRepeatedSubstring() {
        SuffixTreeNode* node = root;
        String result = "";
        getLongestRepeatedSubstringUtil(node, result, "");
        Serial.print("Longest Repeated Substring: ");
        Serial.println(result);
    }

    bool patternSearch(String pat){
        SuffixTreeNode* node = root;
        int length = pat.length();
        int i = 0;
        while(i < length){
            if(node->children[pat[i]] == nullptr){
                return false;
            }
            node = node->children[pat[i]];
            int j = 0;
            for(j = 0; j < edgeLength(node); j++){
                if(i == length){
                    return true;
                }
                if(pat[i] != text[node->start + j]){
                    return false;
                }
                i++;
            }
        }
        return true;
    }

    void getLongestCommonSubstring(String str1, String str2){
        String str = str1 + "$" + str2 + "#";
        SuffixTree tree(str);
        tree.getLongestRepeatedSubstring();
    }

    ~SuffixTree(){
        freeSuffixTreeByPostOrder(root);
        delete rootEnd;
    }

    void printTree(){
        printTree(root, 0);
    }

    int countPatternOccurences(String pat){
        SuffixTreeNode* node = root;
        int length = pat.length();
        int i = 0;
        while(i < length){
            if(node->children[pat[i]] == nullptr){
                return 0;
            }
            node = node->children[pat[i]];
            int j = 0;
            for(j = 0; j < edgeLength(node); j++){
                if(i == length){
                    return node->suffixIndex;
                }
                if(pat[i] != text[node->start + j]){
                    return 0;
                }
                i++;
            }
        }
        return node->suffixIndex;
    }

    SimpleVector<int> getSuffixIndexes(String pat){
        SuffixTreeNode* node = root;
        int length = pat.length();
        int i = 0;
        while(i < length){
            if(node->children[pat[i]] == nullptr){
                return SimpleVector<int>();
            }
            node = node->children[pat[i]];
            int j = 0;
            for(j = 0; j < edgeLength(node); j++){
                if(i == length){
                    return SimpleVector<int>(node->suffixIndex);
                }
                if(pat[i] != text[node->start + j]){
                    return SimpleVector<int>();
                }
                i++;
            }
        }
        return SimpleVector<int>(node->suffixIndex);
    }

    SimpleVector<int> getSuffixIndexes(){
        return SimpleVector<int>(root->suffixIndex);
    }

    SimpleVector<int> findAllOccurences(String pat){
        SuffixTreeNode* node = root;
        int length = pat.length();
        int i = 0;
        while(i < length){
            if(node->children[pat[i]] == nullptr){
                return SimpleVector<int>();
            }
            node = node->children[pat[i]];
            int j = 0;
            for(j = 0; j < edgeLength(node); j++){
                if(i == length){
                    return SimpleVector<int>(node->suffixIndex);
                }
                if(pat[i] != text[node->start + j]){
                    return SimpleVector<int>();
                }
                i++;
            }
        }
        return SimpleVector<int>(node->suffixIndex);
    }

    String longestCommonSubstring(const String& otherText){
        String str = text + "$" + otherText + "#";
        SuffixTree tree(str);
        String result = "";
        tree.getLongestRepeatedSubstringUtil(tree.root, result, "");
        return result;
    }

    String shortestCommonSubstring(const String& otherText){
        String str = text + "$" + otherText + "#";
        SuffixTree tree(str);
        String result = "";
        tree.getLongestRepeatedSubstringUtil(tree.root, result, "");
        return result;
    }

    String longestPalindrome(){
        String str = text + "$" + text + "#";
        SuffixTree tree(str);
        String result = "";
        tree.getLongestRepeatedSubstringUtil(tree.root, result, "");
        return result;
    }

    SimpleVector<String> getAllPalindromes(){
        SimpleVector<String> result;
        for(int i = 0; i < size; i++){
            for(int j = i; j < size; j++){
                String str = text.substring(i, j + 1);
                if(isPalindrome(str)){
                    result.push_back(str);
                }
            }
        }
        return result;
    }

    bool isPalindrome(String str){
        int length = str.length();
        for(int i = 0; i < length / 2; i++){
            if(str[i] != str[length - i - 1]){
                return false;
            }
        }
        return true;
    }

    SimpleVector<String> getAllPatterns(){
        SimpleVector<String> result;
        for(int i = 0; i < size; i++){
            for(int j = i; j < size; j++){
                result.push_back(text.substring(i, j + 1));
            }
        }
        return result;
    }

};

#endif // SUFFIX_TREE_h
