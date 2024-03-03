#ifndef TRIE_TREE_h
#define TRIE_TREE_h

#include <Arduino.h>
#include <Map.h>
#include <SimpleVector.h>

class TrieTree {
    private:
        struct TrieNode {
            Map<char, TrieNode*> children;
            bool isEndOfWord;
            
            TrieNode() : isEndOfWord(false){}
        };
        TrieNode* root;

        void printAllWordsHelper(TrieNode* node, String word){
            if(node->isEndOfWord){
                Serial.println(word.c_str());
            }
            for (auto pair : node->children){
                printAllWordsHelper(pair.second, word + pair.first);
            }
        }

        void findAllWords(TrieNode* node, String prefix, SimpleVector<String>& words){
            if(node->isEndOfWord){
                words.push_back(prefix);
            }
            for (auto pair : node->children){
                findAllWords(pair.second, prefix + pair.first, words);
            }
        }
    public:
        TrieTree() : root(new TrieNode()) {}

        ~TrieTree(){
            delete root;
        }

        void insert(String word){
            TrieNode* node = root;
            for (char ch : word){
                if(node->children.count(ch) == 0){
                    node->children[ch] = new TrieNode();

                }
                node = node->children[ch];
            }
            node->isEndOfWord = true;
        }

        bool search(String word){
            TrieNode* node = root;
            for (char ch : word){
                if(node->children.count(ch) == 0){
                    return false;
                }
                node = node->children[ch];
            }
            return node->isEndOfWord;
        }

        bool startsWith(String prefix){
            TrieNode* node = root;
            for (char ch : prefix){
                if(node->children.count(ch) == 0){
                    return false;
                }
                node = node->children[ch];
            }
            return true;
        }

        bool isEmpty(){
            return root->children.empty();
        }

        void printAllWords() {
            printAllWordsHelper(root, "");
        }

        SimpleVector<String> autoComplete(String prefix){
            SimpleVector<String> words;
            TrieNode* node = root;
            for (char ch : prefix){
                if(node->children.count(ch) == 0){
                    return words;
                }
                node = node->children[ch];
            }
            findAllWords(node, prefix, words);
            return words;
        }
        
};

#endif
