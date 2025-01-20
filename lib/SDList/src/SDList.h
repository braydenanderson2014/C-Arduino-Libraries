#ifndef SDList_h
#define SDList_h

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <ArrayList.h>

enum Mode {SDCARD, MEMORY};

template <typename T>
class SDList {
    private:
        String filename = "list.txt"; // The name of the file to read and write to
        Mode mode = MEMORY; // The mode of the SDList object (SDCARD or MEMORY) Default is MEMORY
        int csPin = 4; // The chip select pin for the SD card
        ArrayList<T> arrayList = ArrayList<T>(); // The ArrayList to store elements in memory

        /**
         * @private
         * @brief Check if the file exists
         * 
         * @return bool True if the file exists, and if it is false, it creates the file and returns true. Only returns false is if the mode is MEMORY
         * 
        */
        bool checkFile() {
            if (mode == MEMORY) {
                return false;
            } else {
                if (!SD.exists(filename)) {
                    File file = SD.open(filename, FILE_WRITE);
                    file.close();
                    return true;
                }
                return true;
            }
        }

        /**
         * @private
         * @brief Check if the element exists in the file
         * 
         * @param element The element to check
         * 
         * @return bool True if the element exists in the file, and false if it does not
        */
        bool checkElement(const T& element) {
            File file = SD.open(filename, FILE_READ);
            if (file) {
                String line;
                while (file.available()) {
                    line = file.readStringUntil('\n');
                    if (line == element) {
                        file.close();
                        return true;  // Return true when a match is found
                    }
                }
                file.close();
            }
            return false;  // Return false if no match was found or the file couldn't be opened
            
        }

        /**
         * @private
         * @brief Check if the file is empty
         * 
         * @return bool True if the file is empty, and false if it is not
        */
        bool isFileEmpty() {
            File file = SD.open(filename, FILE_READ);
            bool isEmpty = file.size() == 0;
            file.close();
            return isEmpty;
        }

        /**
         * @private
         * @brief Add an element to the file
         * 
         * @param element The element to add
         * 
         * @details The function will add the element to the file. If the file is empty, it will use print to avoid leading newline.
        */
        void addElement(const T& element) {
            if (mode == MEMORY) {
                return;
            } else {
                File file = SD.open(filename, FILE_WRITE);
                if (isFileEmpty()) {
                    file.print(element); // Use print for the first element to avoid leading newline
                } else {
                    file.println(element); // Use println for subsequent elements
                }
                file.close();
            }
        }

        /**
         * @private
         * @brief Read the file into memory
         * 
         * @details The function will read the file into memory and clear the ArrayList before reading the file
        */
        void readFileIntoMemory(){
            File file = SD.open(filename, FILE_READ);
            if (file) {
                arrayList.clear();  // Clear the arrayList before reading the file
                String line;
                while (file.available()) {
                    line = file.readStringUntil('\n');
                    arrayList.add(line);  // Add each line from the file to the arrayList
                }
                file.close();
            }
            SD.remove(filename); // Remove the file after reading it into memory
            checkFile(); // Create a new file
        }

        /**
         * @private
         * @brief Dump the memory into the file
         * 
         * @details The function will dump the memory into the file
        */
        void dumpMemoryToFile(){
            File file = SD.open(filename, FILE_WRITE);
            for(int i = 0; i < arrayList.size(); i++){
                file.println(arrayList.get(i));
            }
            file.close();
            arrayList.clear();
        }
    public:
        /**
         * @brief Construct a new SDList object
         * 
         * @param mode SDCARD or MEMORY
         * @param initialCapacity Passes directly into the ArrayList constructor
         * 
        */
        SDList(Mode mode = SDCARD, size_t initialCapacity = 8) : arrayList(ArrayList<T>::DYNAMIC2, initialCapacity) {
            this -> mode = mode;
            //this -> begin(csPin);
        }

        /**
         * @brief Destroy the SDList object
         * 
        */
        ~SDList() {
            clear();
        }

        /**
         * @brief Get the underlying ArrayList object
         * 
         * @return ArrayList<T> The underlying ArrayList object
         */
        ArrayList<T> getUnderlyingStructure() {
            return arrayList;
        }

        /**
         * @brief Begin the SDList object
         * 
         * @param csPin The chip select pin for the SD card
         * 
         * @note This function is required to run in SD mode but not required for Memory mode
        */
        void begin(int csPin = 4) {
             if(mode == SDCARD) {
                bool success = SD.begin(csPin);
                if (!success) {
                    this -> mode = MEMORY;
                } else {
                    this -> mode = SDCARD;
                    checkFile();
                }
                this -> csPin = csPin;
            } else {
                this -> mode = MEMORY;
            }
        }

        /**
         * @brief Begin the SDList object
         * 
         * @param csPin The chip select pin for the SD card
         * @param filename The name of the file to read and write to
         * 
         * @note This function is required to run in SD mode if multiple instances of the library are used but not required for Memory mode
         * if you are only using one instance of the library, you can use this begin function to set the filename or use the begin(csPin) function to use the default
        */
        void begin(int csPin, String filename) {
            this -> filename = filename;
            begin(csPin);
        }
    
        /**
         * @brief Set the Mode of the SDList object
         * 
         * @param mode SDCARD or MEMORY
         * 
         * @details If the mode is SDCARD, the function will attempt to initialize the SD card and check if the file exists. 
         * If the file does not exist, it will be created. If the SD card cannot be initialized, the mode will be set to MEMORY. 
         * If the mode is MEMORY, the function will read the file into memory and set the mode to MEMORY.
         * if you are changing from one mode to another, the data will be copied from the previous mode to the new mode. 
         * (from file to memory, or from memory to file)
        */
        void setMode(Mode mode) {
            this->mode = mode;
            if(mode == SDCARD) {
                bool success = SD.begin(csPin);
                if (!success) {
                    this -> mode = MEMORY;
                    readFileIntoMemory();
                } else {
                    this -> mode = SDCARD;
                    checkFile();
                    dumpMemoryToFile();
                }
            }
        }

        /**
         * @brief Get the Mode of the SDList object
         * 
         * @return Mode SDCARD or MEMORY
        */
        Mode getMode() const {
            return mode;
        }

        /**
         * @brief Append an element to the SDList object
         * 
         * @param element The element to append
         * 
         * @details If the mode is SDCARD, the function will check if the element exists in the file. 
         * If the element does not exist, it will be added to the file. 
         * If the mode is MEMORY, the element will be added to the ArrayList.
        */
        void append(const T& element)  {
            arrayList.add(element);
            if(mode == SDCARD) {
                bool success = checkElement(element);
                if(!success) {
                    addElement(element);
                } else {
                    Serial.println("Element already exists" + element);
                }
            }
        }

        /**
         * @brief Clear the SDList object
         * 
         * @details If the mode is MEMORY, the function will clear the ArrayList. 
         * If the mode is SDCARD, the function will clear the file.
        */
        void clear() {
            if (mode == MEMORY) {
                arrayList.clear();
            } else {
                SD.remove(filename);
                checkFile();
            }
        }

        /**
         * @brief Get an element from the SDList object
         * 
         * @param index The index of the element to get
         * 
         * @return T The element at the specified index
         * 
         * @details If the mode is SDCARD, the function will read the file into memory, get the element at the specified index, and clear the ArrayList. 
         * If the mode is MEMORY, the function will get the element at the specified index.
         * 
         * @note File reading is slower then memory reading.
        */
        T get(int index)  {
            if(mode == SDCARD){
                File file = SD.open(filename, FILE_READ);
                if (file) {
                    arrayList.clear();  // Clear the arrayList before reading the file
                    String line;
                    while (file.available()) {
                        line = file.readStringUntil('\n');
                        if(line == "") {
                            continue;
                        }
                        arrayList.add(line);  // Add each line from the file to the arrayList
                    }
                    file.close();
                }
                T element = arrayList.get(index);  // Get the element at the specified index
                arrayList.clear();  // Clear the arrayList after getting the element
                return element;
            }else {
                return arrayList.get(index);
            }
        }

        /**
         * @brief Insert an element into the SDList object
         * 
         * @param index The index to insert the element
         * @param element The element to insert
         * 
         * @details If the mode is MEMORY, the function will insert the element into the ArrayList at the specified index. 
         * If the mode is SDCARD, the function will read the file into memory, insert the element into the ArrayList at the specified index, and dump the memory into the file.
         * 
         * @note File reading is slower then memory reading.
         * @note if the index is greater than the size of the list, the element will be appended to the end of the list.
         * @note if the element already exists in the list, it will not be added.
         * @note if the list is empty, the element will be appended to the list.
        */
        void insert(int index, const T& element) {
            if (mode == MEMORY) {
                arrayList.insert(index, element);
            } else {
                if(checkElement(element)){
                    return;
                }
                if(size() == 0 || index >= size()){
                    append(element);
                    return;
                } 
                File file = SD.open(filename, FILE_READ);
                if (file) {
                    String line;
                    int i = 0;
                    String content = "";
                    while (file.available()) {
                        line = file.readStringUntil('\n');
                        if (i == index) {
                            content += element + "\n";
                        }
                        content += line + "\n";
                        i++;
                    }
                    file.close();
                    file = SD.open(filename, FILE_WRITE);
                    file.print(content);
                    file.close();
                }
            }
        }

        /**
         * @brief Remove an element from the SDList object
         * 
         * @param index The index of the element to remove
         * 
         * @details If the mode is MEMORY, the function will remove the element from the ArrayList at the specified index. 
         * If the mode is SDCARD, the function will read the file into memory, remove the element from the ArrayList at the specified index, and dump the memory into the file.
         * 
         * @note File reading is slower then memory reading.
        */
        void remove(int index) {
            if (mode == MEMORY) {
                arrayList.remove(index);
            } else {
                File file = SD.open(filename, FILE_READ);
                if (file) {
                    String line;
                    int i = 0;
                    String content = "";
                    while (file.available()) {
                        line = file.readStringUntil('\n');
                        if (i != index) {
                            content += line + "\n";
                        }
                        i++;
                    }
                    file.close();
                    file = SD.open(filename, FILE_WRITE);
                    file.print(content);
                    file.close();
                }
            }
        }

        /**
         * @brief Get the size of the SDList object
         * 
         * @return size_t The size of the SDList object
         * 
         * @details If the mode is MEMORY, the function will return the size of the ArrayList. 
         * If the mode is SDCARD, the function will return the number of lines in the file.
         * 
         * @note File reading is slower then memory reading.
        */
        size_t size() const {
            if (mode == MEMORY) {
                return arrayList.size();
            } else {
                File file = SD.open(filename, FILE_READ);
                if (file) {
                    String line;
                    int i = 0;
                    while (file.available()) {
                        line = file.readStringUntil('\n');
                        i++;
                    }
                    file.close();
                    return i;
                } else {
                    return 0;
                }
            }
        }
        
        /**
         * @brief Get the capacity of the SDList object
         * 
         * @return size_t The capacity of the SDList object
         * 
         * @details If the mode is MEMORY, the function will return the capacity of the ArrayList. 
         * If the mode is SDCARD, the function will return the number of lines in the file.
         * 
         * @note File reading is slower then memory reading.
        */
        size_t capacity() const {
            if (mode == MEMORY) {
                return arrayList.capacity();
            } else {
                File file = SD.open(filename, FILE_READ);
                if (file) {
                    String line;
                    int i = 0;
                    while (file.available()) {
                        line = file.readStringUntil('\n');
                        i++;
                    }
                    file.close();
                    return i;
                } else {
                    return 0;
                }
            }
        }
        
        /**
         *  @brief Check if the SDList object is empty
         * 
         * @return bool True if the SDList object is empty, and false if it is not
        */
        bool isEmpty() const {
           return size() == 0;
        }
};

#endif // SDList_h