# MyDictionary

`MyDictionary` is a custom dictionary class designed for Arduino projects. It provides a simple interface for storing and retrieving key-value pairs, where keys are strings and values are integers. This Library was renamed from DictionaryImplementation to MyDictionary.

## Features

- Implements a dictionary using an unordered map. (Also An Available Library)
- Provides basic dictionary operations such as insertion, deletion, and retrieval.
- Uses custom error handling suitable for Arduino's constrained environment.

## ChangeLog
### Version 1.0.0
* Initial Release
### Version 1.0.1
* Renamed Library from DictionaryImplementation to MyDictionary (This is a Dictionary class Implementation)
* This Library is not tested and may not work. USE AT YOUR OWN RISK! Please report any Bugs to the Author
### Version 1.0.2
* Update to README
* Modified the Library to utilize `<String, String>` instead of `<String, int>`
* This Library is not tested and may not work. USE AT YOUR OWN RISK! Please report any Bugs to the Author
* Added [Dictionary]: to `Serial.print()` statements to make it easier to read the output
### Version 1.0.3
* Modified the Constructor to take in a boolean value to determine if the Dictionary should print to Serial or not
* Modified the UnorderedMap creation to take in a boolean value to determine if the Map should print to Serial or not
* Modified the Hashtable creation to take in a boolean value to determine if the Hashtable should print to Serial or not
* Added new clear() function to clear the Dictionary


    
## Usage

To use `MyDictionary`, include the header file in your Arduino sketch and create an instance of `MyDictionary`.

```cpp
#include "MyDictionary.h"

MyDictionary myDict;
myDict.put("key1", 100);

```

### Retrieving a Value
```cpp
int value = myDict.get("key1");
if (value != -1) {
    // Use the value
}
```
### Removing a Value
```cpp
if (!myDict.remove("key1")) {
    // Handle the error
}
```
### Checking Dictionary Size
```cpp

int size = myDict.size();

```
### Checking if the Dictionary is Empty
```cpp
bool isEmpty = myDict.isEmpty();
```
## Error Handling
The get, put, and remove methods use return values to indicate success or failure, as throwing exceptions is not supported in the Arduino environment.

get returns -1 if the key is not found.
put returns 1 as a success indicator.
remove returns 0 if the key is not found and 1 as a success indicator.

## Dependencies
MyDictionary depends on the Dictionary and UnorderedMap classes. Make sure these classes are available in your project.

Contributing
Contributions to MyDictionary are welcome. Please adhere to the provided coding standards and include unit tests with your pull requests.

License
Specify the license under which your code is available. Common licenses for open-source projects include MIT, GPL, and Apache 2.0.