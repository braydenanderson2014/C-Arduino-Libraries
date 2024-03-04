# Suffix Tree Library for Arduino

## Overview
The Suffix Tree library provides an efficient data structure for quick pattern matching and substring operations within a given text. It's particularly useful in applications involving text analysis, such as finding the longest repeated substring, pattern searching, and more.

## Features
* Pattern Search: Quickly search for the presence of a pattern within the text.
* Longest Repeated Substring: Find the longest repeated substring within the text, useful for data compression algorithms and detecting repeated patterns.
* Longest Common Substring: Determine the longest common substring between two texts, a common problem in text analysis and bioinformatics.
* Support for char and String Types: The library supports both char and String types for versatile applications.
* Dynamic Tree Construction: Builds the suffix tree dynamically as the text is processed, allowing efficient insertion and search operations.

## Usage
To use the Suffix Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "SuffixTree.h"
```

## Creating a Suffix Tree
Instantiate a Suffix Tree with a given text:

```cpp

String myText = "banana";
SuffixTree mySuffixTree(myText);
``` 

## Searching for a Pattern
Check if a pattern exists within the text:

```cpp

bool found = mySuffixTree.patternSearch("ana");
Serial.println(found ? "Pattern found" : "Pattern not found");
```

## Finding the Longest Repeated Substring
Get the longest repeated substring in the text:

```cpp

mySuffixTree.getLongestRepeatedSubstring();
```

## Longest Common Substring
To find the longest common substring between the original text and another string:

```cpp

String otherText = "panama";
String lcs = mySuffixTree.longestCommonSubstring(otherText);
Serial.println("Longest Common Substring: " + lcs);
```

## Palindrome Detection
Detect the longest palindrome within the text:

```cpp

String palindrome = mySuffixTree.longestPalindrome();
Serial.println("Longest Palindrome: " + palindrome);
```
## Applications
* Bioinformatics: Searching for motifs or repeated sequences in DNA or protein sequences.
* Text Editing: Implementing features like autocomplete, spell checking, and find/replace operations.
* Data Compression: Identifying repeated sequences that can be compressed.
* Forensics: Analyzing textual data for patterns or hidden messages.

## Customization
* Extend the library to include additional text analysis functionalities, such as different variations of pattern search or substring enumeration.
* Adapt the library for more complex data types or to optimize for specific applications, such as large-scale text processing or real-time analysis.

## License
Ensure to comply with the library's licensing terms when integrating it into your projects, respecting open-source contributions and proprietary restrictions where applicable.