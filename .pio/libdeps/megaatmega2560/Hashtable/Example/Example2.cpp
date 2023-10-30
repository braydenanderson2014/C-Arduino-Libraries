#include <Hashtable.h>

// Custom class for values
class Person {
public:
    String name;
    int age;

    Person(const String& n, int a) : name(n), age(a) {}
};

void setup() {
    Serial.begin(9600);

    Hashtable<String, Person> people;

    // Adding custom objects to the hashtable
    people.put("Alice", Person("Alice", 30));
    people.put("Bob", Person("Bob", 25));
    people.put("Charlie", Person("Charlie", 40));

    // Retrieving custom objects by key
    Person alice = people.get("Alice");
    Person bob = people.get("Bob");

    Serial.print("Alice: ");
    Serial.print(alice.name);
    Serial.print(", Age: ");
    Serial.println(alice.age);

    Serial.print("Bob: ");
    Serial.print(bob.name);
    Serial.print(", Age: ");
    Serial.println(bob.age);

    // Iterating through keys
    SimpleVector<String> keys = people.keys();
    for (const String& key : keys) {
        Person person = people.get(key);
        Serial.print("Key: ");
        Serial.print(key);
        Serial.print(", Name: ");
        Serial.print(person.name);
        Serial.print(", Age: ");
        Serial.println(person.age);
    }
}

void loop() {
    // Your Arduino code that runs repeatedly goes here
}
