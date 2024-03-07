#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <MathLib.h>
#include <stdarg.h> 

struct Custom_String {
    class String {
    private:
        char* Buffer; // Pointer to dynamically allocated Buffer
        unsigned int Length; // Length of the STRing, not including the null terminator

        // Utility function to Reallocate Buffer with a new size
        void Reallocate(unsigned int newSize) {
            char* newBuffer = new char[newSize];
            for (unsigned int i = 0; i < Length && i < newSize; ++i) {
                newBuffer[i] = Buffer[i];
            }
            delete[] Buffer;
            Buffer = newBuffer;
            Buffer[newSize - 1] = '\0'; // Ensure null termination
            Length = newSize - 1; // Update Length excluding the null terminator
        }

        // Utility function to Copy STRing
        void Copy(const char* src) {
            while (*src) {
                append(*src++);
            }
        }

        // Finds the first occurrence of the Sub_String in the STRing.
    // Returns a pointer to the Beginning of the Sub_String, or nullptr if not found.
        char* STRSTR(const char* STR, const char* subSTR) const {
            if (*subSTR == '\0') {
                return (char*)STR; // If subSTR is an empty STRing, STR is returned.
            }

            while (*STR != '\0') {
                const char* Begin = STR;
                const char* pattern = subSTR;

                // While current characters match and end of either STRing is not reached
                while (*STR && *pattern && *STR == *pattern) {
                    STR++;
                    pattern++;
                }

                // If the complete subSTR has been found
                if (*pattern == '\0') {
                    return (char*)Begin;
                }

                STR = Begin + 1; // Increment starting point
            }

            return nullptr; // SubSTRing not found
        }

        //STR_LEN
        unsigned int STR_LEN(const char* STR) const {
            unsigned int LEN = 0;
            while (STR[LEN] != '\0') {
                LEN++;
            }
            return LEN;
        }

        float FTOA(float n, char* res, int afterpoint) {
            int ipart = (int)n;
            float fpart = n - (float)ipart;
            int i = ITOA(ipart, res, 10);
            if (afterpoint != 0) {
                res[i] = '.';
                fpart = fpart * Pow(10, afterpoint);
                ITOA((int)fpart, res + i + 1, 10);
            }
            return n;
        }

        int ITOA(int value, char* STR, int base) {
            static char digits[] = "0123456789abcdef";
            char* ptr = STR, *ptr1 = STR, tmp_char;
            int tmp_value;

            do {
                tmp_value = value;
                value /= base;
                *ptr++ = digits[ABS(tmp_value % base)];
            } while (value);

            // Apply negative sign
            if (tmp_value < 0) *ptr++ = '-';
            *ptr-- = '\0';
            while(ptr1 < ptr) {
                tmp_char = *ptr;
                *ptr--= *ptr1;
                *ptr1++ = tmp_char;
            }
            return ptr - STR;
        } 
    public:
        // Default conSTRuctor
        String() : Buffer(new char[1]), Length(0) {
            Buffer[0] = '\0'; // Ensure null termination
        }

        // ConSTRuctor with C-STRing
        String(const char* STR) : String() {
            if (STR) Copy(STR);
        }

        // Copy conSTRuctor
        String(const String& other) : String() {
            Copy(other.Buffer);
        }

        // ConSTRuctor with character
        String(char c) : String() {
            append(c);
        }

        // ConSTRuctor with integer
        String(int value) : String() {
            char temp[12]; // 12 characters is enough for 32-bit integer
            SPRINTF(temp, "%d", value);
            Copy(temp);
        }

        // ConSTRuctor with float
        String(float value) : String() {
            char temp[32]; // 32 characters is enough for 32-bit float
            SPRINTF(temp, "%f", value);
            Copy(temp);
        }


        // DeSTRuctor
        ~String() {
            delete[] Buffer;
        }

        // Assignment operator from String
        String& operator=(const String& other) {
            if (this != &other) {
                clear();
                Copy(other.Buffer);
            }
            return *this;
        }

        void SPRINTF(char* STR, const char* Format, ...) {
            va_list args;
            va_start(args, Format);

            while (*Format != '\0') {
                if (*Format == '%') {
                    Format++;
                    if (*Format == 'd') {
                        int i = va_arg(args, int);
                        ITOA(i, STR, 10);
                        while (*STR != '\0') STR++;
                    } else if (*Format == 'f') {
                        float f = (float)va_arg(args, double);
                        FTOA(f, STR, 2);  // 2 decimal places
                        while (*STR != '\0') STR++;
                    }
                } else {
                    *STR = *Format;
                    STR++;
                }
                Format++;
            }

            *STR = '\0';

            va_end(args);
        }

        // Assignment operator from C-STRing
        String& operator=(const char* STR) {
            clear();
            if (STR) Copy(STR);
            return *this;
        }

        // Assignment operator from character
        String& operator=(char c) {
            clear();
            append(c);
            return *this;
        }

        // Assignment operator from integer
        String& operator=(int value) {
            char temp[12]; // 12 characters is enough for 32-bit integer
            SPRINTF(temp, "%d", value);
            *this = temp;
            return *this;
        }

        // Assignment operator from float
        String& operator=(float value) {
            char temp[32]; // 32 characters is enough for 32-bit float
            SPRINTF(temp, "%f", value);
            *this = temp;
            return *this;
        }

        // Concatenation operator with C-STRing
        String operator+(const char* STR) const {
            String result(*this);
            result += STR;
            return result;
        }

        // Append a character to the STRing
        void append(char c) {
            Reallocate(Length + 2); // +1 for the new character, +1 for null terminator
            Buffer[Length++] = c;
        }

        // Append a C-STRing to the STRing
        String& operator+=(const char* STR) {
            if (STR) {
                while (*STR) {
                    append(*STR++);
                }
            }
            return *this;
        }

        // Append another String to this STRing
        String& operator+=(const String& other) {
            return (*this += other.Buffer);
        }


        // Get C-STRing
        const char* C_STR() const {
            return Buffer;
        }

        // Get Length of the STRing
        unsigned int size() const {
            return Length;
        }

        // Clear the STRing
        void clear() {
            Reallocate(1); // Allocate a single character for null termination
        }

        // Compare with another STRing
        int Compare(const String& other) const {
            unsigned int i = 0;
            while (i < Length && i < other.Length) {
                if (Buffer[i] != other.Buffer[i]) {
                    return Buffer[i] - other.Buffer[i];
                }
                ++i;
            }
            return Length - other.Length;
        }

        // Find Sub_String
        int Find(const char* subSTR) const {
            const char* found = STRSTR(Buffer, subSTR);
            if (found) {
                return found - Buffer;
            }
            return -1;
        }

        // Convert to integer (basic implementation)
        int toInt() const {
            int value = 0;
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] < '0' || Buffer[i] > '9') break;
                value = value * 10 + (Buffer[i] - '0');
            }
            return value;
        }

        // Convert to float (basic implementation)
        float toFloat() const {
            float value = 0.0f;
            float sign = 1.0f;
            unsigned int i = 0;
            if (Buffer[0] == '-') {
                sign = -1.0f;
                i = 1;
            }
            for (; i < Length && Buffer[i] != '.'; ++i) {
                if (Buffer[i] < '0' || Buffer[i] > '9') break;
                value = value * 10.0f + (Buffer[i] - '0');
            }
            if (Buffer[i] == '.') {
                float factor = 0.1f;
                for (++i; i < Length; ++i) {
                    if (Buffer[i] < '0' || Buffer[i] > '9') break;
                    value += (Buffer[i] - '0') * factor;
                    factor *= 0.1f;
                }
            }
            return value * sign;
        }

        // Format STRing (basic implementation)
        void Format(const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);

            // Clear the current STRing
            Reallocate(1);
            Buffer[0] = '\0';
            Length = 0;

            // Process the Format STRing
            while (*fmt) {
                if (*fmt == '%') {
                    fmt++;
                    if (*fmt == 's') {
                        // Handle STRing argument
                        const char* STR = va_arg(args, const char*);
                        while (*STR) {
                            append(*STR++);
                        }
                    } else if (*fmt == 'd') {
                        // Handle integer argument
                        int value = va_arg(args, int);
                        char STR[12];  // Buffer for integer STRing representation
                        ITOA(value, STR, 10);
                        char* p = STR;
                        while (*p) {
                            append(*p++);
                        }
                    }
                } else {
                    // Handle normal character
                    append(*fmt);
                }
                fmt++;
            }

            va_end(args);
        }


        // SubSTRing
        String Sub_String(unsigned int start, unsigned int end) const {
            if (start > end || start >= Length) {
                return String();
            }
            if (end >= Length) {
                end = Length - 1;
            }
            String result;
            result.Reallocate(end - start + 2);
            for (unsigned int i = start; i <= end; ++i) {
                result.append(Buffer[i]);
            }
            return result;
        }

        // Reverse the STRing
        void Reverse() {
            for (unsigned int i = 0; i < Length / 2; ++i) {
                char temp = Buffer[i];
                Buffer[i] = Buffer[Length - i - 1];
                Buffer[Length - i - 1] = temp;
            }
        }

        // Repeat the STRing
        String Repeat(unsigned int times) const {
            String result;
            for (unsigned int i = 0; i < times; ++i) {
                result += *this;
            }
            return result;
        }

        // Replace all occurrences of a Sub_String with another STRing
        String Replace(const char* subSTR, const char* Replacement) const {
            String result;
            unsigned int start = 0;
            unsigned int found;
            while ((found = STRSTR(Buffer + start, subSTR) - Buffer) != (unsigned int)-1) {
                result += Sub_String(start, found - 1);
                result += Replacement;
                start = found + STR_LEN(subSTR);
            }
            result += Sub_String(start, Length - 1);
            return result;
        }

        // Remove all occurrences of a character
        String remove(char c) const {
            String result;
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] != c) {
                    result.append(Buffer[i]);
                }
            }
            return result;
        }

        // Remove all occurrences of a Sub_String
        String remove(const char* subSTR) const {
            String result;
            unsigned int start = 0;
            unsigned int found;
            while ((found = STRSTR(Buffer + start, subSTR) - Buffer) != (unsigned int)-1) {
                result += Sub_String(start, found - 1);
                start = found + STR_LEN(subSTR);
            }
            result += Sub_String(start, Length - 1);
            return result;
        }

        // Convert to upper case
        String toUpper() const {
            String result(*this);
            for (unsigned int i = 0; i < result.Length; ++i) {
                if (result.Buffer[i] >= 'a' && result.Buffer[i] <= 'z') {
                    result.Buffer[i] -= 32;
                }
            }
            return result;
        }

        // Convert to lower case
        String toLower() const {
            String result(*this);
            for (unsigned int i = 0; i < result.Length; ++i) {
                if (result.Buffer[i] >= 'A' && result.Buffer[i] <= 'Z') {
                    result.Buffer[i] += 32;
                }
            }
            return result;
        }

        // Trim whitespace from the Beginning and end of the STRing
        String Trim() const {
            unsigned int start = 0;
            while (start < Length && (Buffer[start] == ' ' || Buffer[start] == '\t' || Buffer[start] == '\n' || Buffer[start] == '\r')) {
                ++start;
            }
            unsigned int end = Length - 1;
            while (end > start && (Buffer[end] == ' ' || Buffer[end] == '\t' || Buffer[end] == '\n' || Buffer[end] == '\r')) {
                --end;
            }
            return Sub_String(start, end);
        }

        // Split the STRing into Sub_Strings using a delimiter
        String* Split(char delimiter, unsigned int* count) const {
            *count = 0;
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] == delimiter) {
                    ++(*count);
                }
            }
            String* result = new String[*count + 1];
            unsigned int j = 0;
            unsigned int start = 0;
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] == delimiter) {
                    result[j++] = Sub_String(start, i - 1);
                    start = i + 1;
                }
            }
            result[j] = Sub_String(start, Length - 1);
            return result;
        }

        // DeSTRuctor for Split result
        static void freeSplitResult(String* result) {
            delete[] result;
        }

        // Check if the STRing is empty
        bool isEmpty() const {
            return Length == 0;
        }

        // Check if the STRing is a number
        bool isNumber() const {
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] < '0' || Buffer[i] > '9') {
                    return false;
                }
            }
            return true;
        }

        // Check if the STRing is a float
        bool isFloat() const {
            bool dot = false;
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] == '.') {
                    if (dot) {
                        return false; // More than one dot
                    }
                    dot = true;
                } else if (Buffer[i] < '0' || Buffer[i] > '9') {
                    return false;
                }
            }
            return dot;
        }

        // Check if the STRing is a palindrome
        bool isPalindrome() const {
            for (unsigned int i = 0; i < Length / 2; ++i) {
                if (Buffer[i] != Buffer[Length - i - 1]) {
                    return false;
                }
            }
            return true;
        }

        // Check if the STRing contains a Sub_String
        bool contains(const char* subSTR) const {
            return STRSTR(Buffer, subSTR) != nullptr;
        }

        // Check if the STRing starts with a Sub_String
        bool startsWith(const char* subSTR) const {
            unsigned int i = 0;
            while (subSTR[i] != '\0') {
                if (Buffer[i] != subSTR[i]) {
                    return false;
                }
                ++i;
            }
            return true;
        }

        // Check if the STRing ends with a Sub_String
        bool endsWith(const char* subSTR) const {
            unsigned int i = 0;
            unsigned int j = Length - STR_LEN(subSTR);
            while (subSTR[i] != '\0') {
                if (Buffer[j] != subSTR[i]) {
                    return false;
                }
                ++i;
                ++j;
            }
            return true;
        }

        // Get Sub_String by index
        char operator[](unsigned int index) const {
            if (index < Length) {
                return Buffer[index];
            }
            // Out-of-bounds access returns null character
            return '\0';
        }       


        String toString() const {
            return *this;
        }

        String toString(int value) const {
            return String(value);
        }

        String toString(float value) const {
            return String(value);
        }


        // Output STReam operator


    };
    
};

#endif // CUSTOM_STRING_H