#include <DynamicStorageLibrary.h>

DynamicStorage<String, int> storage(DynamicStorage<String, int>::AUTO);

void setup() {
  Serial.begin(115200);
  delay(1000);

  storage.add(10);
  storage.add(20);
  storage.put("score", 42);

  Serial.println("Stored values:");
  Serial.println(storage.get(0));
  Serial.println(storage.get(1));
  Serial.println(storage.get("score"));

  if (storage.hasKey("score")) {
    Serial.println("score key exists");
  }
}

void loop() {
}
