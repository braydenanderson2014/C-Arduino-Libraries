#include <Properties.h>

void setup() {
    Serial.begin(9600);

    // Create a Properties object
    Properties properties;

    // Set properties
    properties.setProperty("name", "John");
    properties.setProperty("age", "30");
    properties.setProperty("city", "New York");

    properties.setProperty("country", "USA", "test.txt");
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

    // Iterate through keys
    Properties::KeyIterator keyIterator = properties.keysIterator();
    while (keyIterator.hasNext()) {
        String key = keyIterator.next();
        Serial.println("Key: " + key);
    }

    // Iterate through values
    Properties::ValueIterator valueIterator = properties.valuesIterator();
    while (valueIterator.hasNext()) {
        String value = valueIterator.next();
        Serial.println("Value: " + value);
    }

    properties.removeProperty("name");
    properties.removeProperty("age");

    // Clear the properties
    properties.clear();
}
