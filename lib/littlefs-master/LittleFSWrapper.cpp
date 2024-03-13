#include <LittleFSWrapper.h>

extern LittleFSWrapper LittleFS;
bool LittleFSWrapper::begin() {
    return LittleFS.begin();
}

bool LittleFSWrapper::writeFile(const char *path, const char *message) {
    File file = LittleFS.open(path, "w"); 
    if(!file){
        return false;
    }

    file.print(message);
    file.close();
    return true;
}

String LittleFSWrapper::readFile(const char *path) {
    File file = LittleFS.open(path, "r");
    if(!file){
        return String(""); // Return empty string if the file doesn't open
    }

    String fileContent;
    while(file.available()){
        fileContent += char(file.read());
    }
    file.close();
    return fileContent;
}

bool LittleFSWrapper::deleteFile(const char *path) {
    return LittleFS.remove(path); 
}

bool LittleFSWrapper::renameFile(const char *oldName, const char *newName) {
    return LittleFS.rename(oldName, newName);
}

bool LittleFSWrapper::exists(const char *path) {
    return LittleFS.exists(path);
}

File LittleFSWrapper::open(const char *path, const char *mode) {
    return LittleFS.open(path, mode);
}

bool LittleFSWrapper::remove(const char *path) {
    return LittleFS.remove(path);
}

bool LittleFSWrapper::rename(const char *oldName, const char *newName) {
    return LittleFS.rename(oldName, newName);
}
