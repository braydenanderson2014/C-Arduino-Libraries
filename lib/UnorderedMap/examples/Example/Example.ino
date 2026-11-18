#include <UnorderedMap.h>

UnorderedMap<int, int> values;

void setup() {
    Serial.begin(9600);
    values.put(1, 42);

    int value;
    if (values.get(1, value)) {
        Serial.println(value);
    }
}

void loop() {
}
