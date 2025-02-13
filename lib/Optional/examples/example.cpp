#include "Optional.h"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    while (!Serial) {} // Wait for serial to initialize

    Serial.println("\n--- Basic Usage ---");
    Optional<int> maybeValue(42);
    if (maybeValue.hasValue()) {
        Serial.print("Stored Value: ");
        Serial.println(maybeValue.getValue());
    }

    Optional<int> emptyValue;
    Serial.println(emptyValue.hasValue() ? "Has value" : "No value present");

    Serial.println("\n--- Advanced Usage ---");
    Serial.println("\nUsing with Custom Types:");
    struct Data {
        int x;
        float y;
    };
    Optional<Data> maybeData(Data{10, 5.5});
    if (maybeData.hasValue()) {
        Data d = maybeData.getValue();
        Serial.print("x: "); Serial.println(d.x);
        Serial.print("y: "); Serial.println(d.y);
    }

    Serial.println("\nOverriding Default Values:");
    Optional<String> maybeString;
    Serial.println(maybeString.orElse("Default String"));

    Serial.println("\nTransforming Values with map():");
    Optional<int> num(10);

    Serial.println("\nUsing ifPresent() to Apply a Function:");
    num.ifPresent([](int x) { Serial.print("Value: "); Serial.println(x); });

    Serial.println("\nSwapping Two Optionals:");
    Optional<int> a(5), b(10);
    a.swap(b);
    Serial.print("a: "); Serial.println(a.getValue());
    Serial.print("b: "); Serial.println(b.getValue());

    Serial.println("\nEmplacing New Values:");
    Optional<String> text;
    text.emplace("Hello, Optional!");
    Serial.println(text.getValue());
}

void loop() {
    // Nothing to do in loop
}
