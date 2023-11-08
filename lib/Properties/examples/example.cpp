#include <Properties.h>

void setup() {
    Serial.begin(9600);

    // Create a Properties object
    Properties properties;

    // Set properties
    properties.setProperty("name", "John");
    properties.setProperty("age", "30");
    properties.setProperty("city", "New York");
    //Save Properties
    properties.saveToSD("test.txt");
    
    //Set Properties with file path
    properties.setProperty("country", "USA", "test2.txt");
    properties.setProperty("state", "New York", "test.properties");

    // Retrieve properties
    String name = properties.getProperty("name");
    String age = properties.getProperty("age");
    String city = properties.getProperty("city");

    Serial.println("Name: " + name);
    Serial.println("Age: " + age);
    Serial.println("City: " + city);

    // Check if a property exists
    bool hasCountry = properties.containsKey("country");
    Serial.println("Has 'country' property: " + String(hasCountry));

    Serial.println("Properties Current Capacity: " + String(properties.size()));
    Serial.println("Properties Current Size: " + String(properties.elements()));

    for(Properties:PropertiesIterator it = properties.begin(); it != properties.end(); ++it) {
        Serial.println("Key: " + it.key() + ", Value: " + it.value());
    }
    
    properties.removeProperty("name");
    properties.removeProperty("age");

    // Clear the properties
    properties.clear();

    // Check if the properties are empty
    bool isEmpty = properties.isEmpty();
    Serial.println("Properties is empty: " + String(isEmpty));

    // Load Properties
    Properties.loadFromSD("test.txt");

}
