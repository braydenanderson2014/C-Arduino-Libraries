/*
    Example Name: Example For Trie Tree Library

    Basic example for the Trie Tree library. This example demonstrates the basic functionality of the Trie Tree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <Arduino.h>
#include <TrieTree.h>

TrieTree trie; // Create a TrieTree object

void setup(){
    Serial.begin(9600);
    //Inserting some words into the trie
    trie.insert("hello");
    trie.insert("world");
    trie.insert("hi");
    trie.insert("hey");
    trie.insert("how");
    trie.insert("are");

    //Searching for words in the trie
    Serial.println(trie.search("hello")); // This will print "1" because "hello" is present in the trie

    //Printing all the words in the trie
    trie.printAllWords(); // This will print all the words in the trie

    //Finding all the words with a given prefix
    SimpleVector<String> words;
    trie.findAllWordsWithPrefix("he", words); // This will find all the words with the prefix "he"
    for (int i = 0; i < words.size(); i++) {
        Serial.println(words[i]); // This will print all the words with the prefix "he"
    }

    //Deleting a word from the trie
    trie.deleteWord("hello"); // This will delete the word "hello" from the trie

    //Searching for the deleted word
    Serial.println(trie.search("hello")); // This will print "0" because "hello" is not present in the trie

    //Printing all the words in the trie after deletion
    trie.printAllWords(); // This will print all the words in the trie after deletion

    //Finding all the words with a given suffix
    SimpleVector<String> words2;
    trie.findAllWordsWithSuffix("he", words2); // This will find all the words with the Suffix "he"
}

void loop(){

}
