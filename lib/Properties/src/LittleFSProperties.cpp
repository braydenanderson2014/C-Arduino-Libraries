#include <LittleFSProperties.h>

#if defined(ESP8266)
    #include <LittleFSWrapper.h>
    #define SIMPLE_PROPERTIES_HAS_LITTLEFS
#elif defined(ESP32) || defined(ESPRESSIF32) || defined(ESP32S2) || defined(ESP32C3)
    #include <LittleFS.h>
    #define SIMPLE_PROPERTIES_HAS_LITTLEFS
#endif

bool LittleFSProperties::beginSD(size_t cs, IDENTIFIERTYPE identifierType) {
    return Properties::begin(cs, identifierType);
}

bool LittleFSProperties::beginLFS(IDENTIFIERTYPE identifierType) {
#if defined(SIMPLE_PROPERTIES_HAS_LITTLEFS)
    if (!LittleFS.begin()) {
        return false;
    }
    return Properties::begin(LittleFS, identifierType);
#else
    (void)identifierType;
    return false;
#endif
}
