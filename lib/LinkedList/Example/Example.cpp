#include <LinkedList.h>
// Instantiate the list globally
LinkedList<int> myList;

void setup() {
    Serial.begin(9600);
    
    // Use the list
    myList.append(1);
    myList.append(2);
    myList.prepend(0);
    myList.insert(3, 3);

    Serial.println("Size: " + String(myList.getSize()));

    for (size_t i = 0; i < myList.getSize(); i++) {
        Serial.println("Element at position " + String(i) + ": " + String(myList.get(i)));
    }

    if (myList.contains(2)) {
        Serial.println("List contains 2.");
    }

    myList.remove(2);
    myList.clear();
}