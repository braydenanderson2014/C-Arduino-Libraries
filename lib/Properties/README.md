# Properties Class

The `Properties` class is a C++ class that provides a simple key-value store for managing properties. It allows you to store and retrieve key-value pairs, save properties to an SD card, load properties from an SD card, and perform other operations commonly used for managing configurations and settings.

## Usage

To use the `Properties` class, follow these steps:

1. Include the necessary header files (`Properties.h` and `Hashtable.h`) in your C++ program.
2. Create an instance of the `Properties` class.
3. Use the provided methods to set, get, remove, or manipulate properties as needed.

## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Properties.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <Properties.h> 
```

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
* Update to the README.md file
* [WARNING]: Underlying Libraries that the Properties library depends on have been updated. This may cause issues. Please report any issues to the Author. This library will be tested and patched (if needed) as soon as possible.
            


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
