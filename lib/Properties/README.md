# Properties Class

The `Properties` class is a C++ class that provides a simple key-value store for managing properties. It allows you to store and retrieve key-value pairs, save properties to an SD card, load properties from an SD card, and perform other operations commonly used for managing configurations and settings.

## Usage

To use the `Properties` class, follow these steps:

1. Include the necessary header files (`Properties.h` and `Hashtable.h`) in your C++ program.
2. Create an instance of the `Properties` class.
3. Use the provided methods to set, get, remove, or manipulate properties as needed.

## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Properties.git" OR
git clone "https://github.com/braydenanderson2014/SimpleProperties.git"

git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Hashtable.git" OR
git clone "https://github.com/braydenanderson2014/ArduinoHashtable.git"

git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/SimpleVector.git" OR
git clone "https://github.com/braydenanderson2014/SimpleVector.git"


```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <Properties.h> 
```

If you use the Properties library outside of platformio, please also add the 
```cpp
#include <Hashtable.h>
#include <SimpleVector.h>
#include <iostream.h>
```

You may also need:
* RTCLib
* LiquidCrystal
* LiquidCrystal_I2C

# ARDUINO
## ChangeLog
### Version 1.0.0:
* Initial Release
### Version 1.0.1 - BETA: [CURRENT-RELEASE] -> [BETA]
* Added Support for Several New File types Including (.csv, .json, .xml, .toml, .ini, .yaml)
* Added Support for creating your own custom Key-Value Pair file. The saveToSD, store, and loadFromSD functions will read a new enum variable called IDENTIFIERTYPE. The Available options are: EQUALS (=), COLEN (:), SEMICOLEN (;), HYPHEN (-), COMMA (,), FORWARD_SLASH (\), BACKWARD_SLASH (/)

# PlatformIO
## ChangeLog
### Version 1.0.0:
* Initial Release 
### Version 1.0.1:
* Added the ability to get the number of properties in the file
* Update to the Library Json File
### Version 1.0.2:
* Update to the Library Json File
### Version 1.0.3:
* Update to the Library Json File
* Added This ChangeLog to the Library Json File
* Library has been updated to work properly with the latest Hashtable Release.
* This Library has been partially tested. USE AT YOUR OWN RISK. Library Considered [Semi-Stable].
### Version 1.0.4:
* Update to the Library Json File
* Patching the iterator, and the save Function. Iterator may still have issues, Working toward fixing issue.
* Patch is untested
* This library has been partially tested and is now considered largely stable
### Version 1.0.5:
* Patched the Iterator. It now seems to work with no issues. [TESTED]
* Patched the Save Function. It now seems to save and create files as expected. [PARTIALLY-TESTED]
* Added Elements(); Function that returns the current number of elements in the properties.
* Adjusted size() Function that returns the current capacity of the underlying table.
### Version 1.0.6:
* Added [PROPERTIES]: to the beginning of each Print Statement for easier debugging
* Added a deleteFile() function to delete a file off of an SD Card.
* Update to the README.md file
* [WARNING]: Underlying Libraries that the Properties library depends on have been updated. This may cause issues. Please report any issues to the Author. This library will be tested and patched (if needed) as soon as possible.
### Version 1.0.7:
* [WARNING]: Underlying Libraries have been updated. This may cause issues. This library has been partially tested since the last update. Please report any issues to the Author.
* Modified the Serial.print() statements so that they dont print by default. You have to pass a bool to the constructor to activate them.
* Debug Variable gets passed to the Hashtable Constructor, so that library also conforms to your wishes.
### Version 1.0.8:
* Removed Debug Messages From Library to keep library imprint small... Plan is to create a secondary library that will be used for debugging.
### Version 1.0.9:
* Update to SaveToSD() Function... Function now returns a boolean (1 = true, 0 = false)
* Update to LoadFromSD() Function... Function now returns a boolean (1 = true, 0 = false)
* Added new save() Function ... This function just calls SaveToSD() but it is for those who like simpler functions
* Added new load() function ... This function just calls the LoadfromSD() but it is for those who like simpler functions
* Added new store() function ... This function is similar to SavetoSD() but allows for comments.
* Added Function comments to all Functions.
### Version 1.1.0: [CURRENT-RELEASE] -> [BETA]
* Added Support for Several New File types Including (.csv, .json, .xml, .toml, .ini, .yaml)
* Added Support for creating your own custom Key-Value Pair file. The saveToSD, store, and loadFromSD functions will read a new enum variable called IDENTIFIERTYPE. The Available options are: EQUALS (=), COLEN (:), SEMICOLEN (;), HYPHEN (-), COMMA (,), FORWARD_SLASH (\), BACKWARD_SLASH (/)

            
            


Here's an example of how to use the `Properties` class:

```cpp
#include <Arduino.h>
#include <SD.h>
#include "Properties.h"

Properties myProperties;

void setup() {
    Serial.begin(9600);
    // Initialize SD card (make sure SD card is properly connected)
    if (!SD.begin(4)) {
        Serial.println("Failed to initialize SD card.");
    }
}

void loop() {
    // Set a property
    myProperties.setProperty("name", "John");

    // Get a property
    String name = myProperties.getProperty("name");
    Serial.println("Name: " + name);

    // Check if a property exists
    bool hasAge = myProperties.containsKey("age");
    Serial.println("Property 'age' exists: " + String(hasAge ? "Yes" : "No"));

    // Remove a property
    myProperties.removeProperty("name");

    // Check if the properties are empty
    bool isEmpty = myProperties.isEmpty();
    Serial.println("Properties are empty: " + String(isEmpty ? "Yes" : "No"));

    delay(1000);
}


```
