#include <UnoQBridge.h>

UnoQBridge bridge;

void setup() {
    Serial.begin(115200);
    bridge.begin();
    Serial.println("UnoQBridge experimental scaffold ready");
}

void loop() {
    if (!bridge.isReady()) {
        delay(250);
        return;
    }

    delay(1000);
}
