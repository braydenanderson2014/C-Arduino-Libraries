#ifndef LITTLEFS_PROPERTIES_H
#define LITTLEFS_PROPERTIES_H

#include <Properties.h>

class LittleFSProperties : public Properties {
public:
       using IDENTIFIERTYPE = Properties::IDENTIFIERTYPE;
       using LittleFSPropertiesIterator = Properties::PropertiesIterator;

       LittleFSProperties() = default;
       explicit LittleFSProperties(fs::FS& filesystem) : Properties(filesystem) {}
       ~LittleFSProperties() = default;

       bool beginLFS(IDENTIFIERTYPE identifierType = Properties::EQUALS);
       bool beginSD(size_t cs = 4, IDENTIFIERTYPE identifierType = Properties::EQUALS);
};

#endif // LITTLEFS_PROPERTIES_H