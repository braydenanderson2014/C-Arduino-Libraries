/*
    Example Name: Example For Initializer_List Library

    Basic example for the Initializer_List library. This example demonstrates the basic functionality of the Initializer_List library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/


#include <Initializer_List.h>
#include <SimpleVector.h>

void setup() {
    // Create a SimpleVector using an initializer list
    SimpleVector<int> vec({1, 2, 3, 4, 5});    // Adds 1, 2, 3, 4, 5 to the vector as the initial values
    //Here is an Example of the intializer_list<T> being used in the SimpleVector class... This is a real snip from the SimpleVector.h file
    //Since the SimpleVector class is a template class, the initializer_list<T> is used to initialize the vector with the values from the initializer list
    //Since the SimpleVector class is a .h file, i put the code in comments to show the example of the initializer_list<T> being used in the SimpleVector class
    
    /*
        SimpleVector(initializer_list<T> initList) : array(new T[initList.size()]), count(initList.size()), capacity(initList.size()) {
        int i = 0;
        for (const auto& value : initList) {
            array[i++] = value;
        }
    }
    */
}

void loop() {
    // Nothing to do here
}