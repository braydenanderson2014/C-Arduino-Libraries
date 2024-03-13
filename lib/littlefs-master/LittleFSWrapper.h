#ifndef LITTLEFSWRAPPER_H
#define LITTLEFSWRAPPER_H


#include "FS.h"
#include "LittleFS.h"

class LittleFSWrapper {
public:
    LittleFSWrapper(){
        // Constructor might be empty if no initialization is needed
    } 
    bool begin(); // Initialize the filesystem

    // File operations
    bool writeFile(const char *path, const char *message); 
    File open(const char *path, const char *mode);
    bool remove(const char *path);
    bool rename(const char *oldName, const char *newName);
    String readFile(const char *path);
    bool deleteFile(const char *path);
    bool renameFile(const char *oldName, const char *newName); 
    bool exists(const char *path);
};


#endif // LITTLEFSWRAPPER_H
