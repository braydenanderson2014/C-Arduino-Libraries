#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <MathLib.h>
#include <stdarg.h> 
/**
 * Custom String class
 * Wrapped in a struct to avoid conflicts with the standard String class
 * 
*/
struct Custom_String {
    class String {
    private:
        char* Buffer; // Pointer to dynamically allocated Buffer
        unsigned int Length; // Length of the STRing, not including the null terminator

        // Utility function to Reallocate Buffer with a new size
        /**
         * @brief Reallocate Buffer with a new size
         * @private 
         * 
         * @param newSize - The new size of the Buffer
        */
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

        // Utility function to Copy String
        /**
         * @brief Copy String
         * @private
         * 
         * @param src - The source STRing to copy
        */
        void Copy(const char* src) {
            unsigned int srcLength = STR_LEN(src);
            Reallocate(srcLength + 1); // +1 for the null terminator
            for (unsigned int i = 0; i < srcLength; ++i) {
                Buffer[i] = src[i];
            }
            Length = srcLength; // Update Length excluding the null terminator
        }

        // Finds the first occurrence of the Sub_String in the STRing.
        // Returns a pointer to the Beginning of the Sub_String, or nullptr if not found.
        /**
        * @brief Finds the first occurrence of the Sub_String in the String.
        * @param STR - The STRing to search in
        * @param subSTR - The Sub_String to search for
        * 
        * @return char* - A pointer to the Beginning of the Sub_String, or nullptr if not found.
        */
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
        /**
         * @brief Get the Length of a STRing
         * @param STR - The STRing to get the Length of
         * 
         * @return unsigned int - The Length of the STRing
        */
        unsigned int STR_LEN(const char* STR) const {
            unsigned int LEN = 0;
            while (STR[LEN] != '\0') {
                LEN++;
            }
            return LEN;
        }

        /**
         * @brief Converts a float to a string.
         *
         * @param n The float to convert.
         * @param res The buffer to store the resulting string. This buffer must be large enough to hold the string representation of the float, including the decimal point and null terminator.
         * @param afterpoint The number of digits to include after the decimal point.
         * @return The original float value.
         *
         * The function works by first converting the integer part of the float to a string, then adding a decimal point to the string, and finally converting the fractional part of the float to a string.
         * The fractional part is first multiplied by 10^afterpoint to shift the decimal point to the right, then it's converted to an integer and that integer is converted to a string.
        */
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


        /**
         * Converts an integer to a string.
         *
         * @param value The integer to convert.
         * @param STR The buffer to store the resulting string. This buffer must be large enough to hold the string representation of the integer, including the negative sign (if applicable) and null terminator.
         * @param base The base to use for the conversion. This must be between 2 and 16. For base 10, digits are '0'-'9'. For bases 11-16, digits are 'a'-'f'.
         * @return The length of the resulting string, not including the null terminator.
         *
         * The function works by repeatedly dividing the integer by the base and storing the remainder as a digit in the string. The digits are stored in reverse order, so the string is reversed at the end.
         * If the integer is negative, a negative sign is added to the end of the string (which becomes the beginning after the string is reversed).
         */
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

        // Ensure the Buffer has enough capacity to store a STRing of the given length
        /**
         * @brief Ensures that the string has enough capacity to hold a specified number of characters.
         *
         * @param newLength The new length that the string needs to be able to hold. This does not include the null terminator.
         *
         * This function checks if the current capacity of the string is less than the specified new length. If it is, it increases the capacity.
         * The new capacity is twice the current capacity, unless this is still less than the new length, in which case the new capacity is set to the new length.
         * The capacity is always increased by one more than the new length to make room for the null terminator.
         * If the capacity needs to be increased, the function calls Reallocate to perform the reallocation.
         */
        void ensureCapacity(unsigned int newLength) {
            if (newLength >= capacity) {
                unsigned int newCapacity = capacity * 2;
                if (newCapacity < newLength) {
                    newCapacity = newLength;
                }
                Reallocate(newCapacity + 1); // +1 for the null terminator
            }
        }


    public:
        static const unsigned int NPOS = -1; // Maximum value for size_t

        unsigned int capacity; // Tracks the allocated size of 'buffer'

        // Existing constructors, destructor, methods...

        // Constructor to create a string filled with 'n' instances of character 'c'
        /**
         * @brief Create a STRing filled with 'n' instances of character 'c'
         * 
         * @param n - The number of instances of character 'c'
         * @param c - The character to fill the STRing with
         * 
         * This constructor creates a STRing with a specified number of instances of a character. It allocates a Buffer of the required size, fills it with the character, and null-terminates the STRing.
        */
        String(unsigned int n, char c) : Length(n), capacity(n + 1), Buffer(new char[n + 1]) {
            memset(Buffer, c, n);
            Buffer[n] = '\0'; // Null-terminate the string
        }

        // Default conSTRuctor
        /**
         * @brief Default conSTRuctor
         * 
         * This conSTRuctor creates an empty STRing by allocating a Buffer of size 1 and null-terminating it.
        */        
        String() : Buffer(new char[1]), Length(0) {
            Buffer[0] = '\0'; // Ensure null termination
        }

       // Constructor with C-string
        /**
         * @brief Create a STRing from a C-style STRing
         * 
         * @param str - The C-style STRing to create the STRing from
         * 
         * This constructor creates a STRing from a C-style STRing by copying the STRing into a newly allocated Buffer and null-terminating it.
        */
        String(const char* str) : Buffer(nullptr), Length(0) {
            if (str) Copy(str);
        }

        
        // Copy constructor
        /**
         * @brief Copy conSTRuctor
         * 
         * @param other - The STRing to copy
         * 
         * This constructor creates a STRing by copying another STRing. It allocates a Buffer of the same size as the source STRing, copies the source STRing into the Buffer, and null-terminates the STRing.
        */
        String(const String& other) : Buffer(nullptr), Length(0) {
            Copy(other.Buffer);
        }

        /**
         * @brief ConSTRuctor with character
         * 
         * @param other - The STRing to copy
        */
        String(String& other) : Buffer(nullptr), Length(0) {
            Copy(other.Buffer);
        }

        // ConSTRuctor with character
        /**
         * @brief ConSTRuctor with character
         * 
         * @param c - The character to create the STRing from
         * 
         * This constructor creates a STRing from a single character by allocating a Buffer of size 2, copying the character into the Buffer, and null-terminating the STRing.
        */
        String(char c) : String() {
            append(c);
        }

        // ConSTRuctor with integer
        /**
         * @brief ConSTRuctor with integer
         * 
         * @param value - The integer to create the STRing from
         * 
         * This constructor creates a STRing from an integer by converting the integer to a C-style STRing and then creating the STRing from the C-style STRing.
        */
        String(int value) : String() {
            char temp[12]; // 12 characters is enough for 32-bit integer
            SPRINTF(temp, "%d", value);
            Copy(temp);
        }

        // ConSTRuctor with float
        /**
         * @brief ConSTRuctor with float
         * 
         * @param value - The float to create the STRing from
         * 
         * This constructor creates a STRing from a float by converting the float to a C-style STRing and then creating the STRing from the C-style STRing.
        */
        String(float value) : String() {
            char temp[32]; // 32 characters is enough for 32-bit float
            SPRINTF(temp, "%f", value);
            Copy(temp);
        }


        // DeSTRuctor
        /**
         * @brief DeSTRuctor
         * 
         * This deSTRuctor deallocates the Buffer used by the STRing.
        */
        ~String() {
            delete[] Buffer;
        }

        size_t Capacity(){
            return capacity;
        }

        // Concatenate with another Custom_String::String
        /**
         * @brief Concatenate with another Custom_String::String
         * 
         * @param rhs - The right-hand side STRing to concatenate
         * @return String - The concatenated STRing
        */
        String operator+(const String& rhs) const {
            String result(*this); // Start with a copy of this string
            result += rhs; // Append the right-hand string
            return result;
        }

        // Concatenate with a C-style string (const char*)
        /**
         * @brief Concatenate with a C-style string (const char*)
         * 
         * @param rhs - The right-hand side C-style STRing to concatenate
         * @return String - The concatenated STRing
        */
        String operator+(const char* rhs) const {
            String result(*this); // Start with a copy of this string
            result += rhs; // Append the C-style string
            return result;
        }

        // Concatenate with a single character
        /**
         * @brief Concatenate with a single character
         * 
         * @param rhs - The right-hand side character to concatenate
         * @return String - The concatenated STRing
        */
        String operator+(char rhs) const {
            String result(*this); // Start with a copy of this string
            result.append(rhs); // Append the character
            return result;
        }
       

        // Ensure you have an operator+= overload to handle appending both Custom_String::String objects and const char* strings.
        // For example:

        // Concatenate and assign a const char* to a Custom_String::String
        /**
         * @brief Concatenate and assign a const char* to a Custom_String::String
         * 
         * @param rhs - The right-hand side C-style STRing to concatenate
         * @return String& - The concatenated STRing
        */
        String& operator+=(const char* rhs) {
            unsigned int rhsLength = strlen(rhs);
            ensureCapacity(Length + rhsLength); // Ensure there is enough space for the concatenated string
            memcpy(Buffer + Length, rhs, rhsLength); // Append the new string
            Length += rhsLength;
            Buffer[Length] = '\0'; // Re-null-terminate
            return *this;
        }

        // Concatenate and assign another Custom_String::String to this Custom_String::String
        /**
         * @brief Concatenate and assign another Custom_String::String to this Custom_String::String
         * 
         * @param rhs - The right-hand side STRing to concatenate
         * @return String& - The concatenated STRing
        */
        String& operator+=(const String& rhs) {
            ensureCapacity(Length + rhs.Length); // Ensure there is enough space for the concatenated string
            memcpy(Buffer + Length, rhs.Buffer, rhs.Length); // Append the new string
            Length += rhs.Length;
            Buffer[Length] = '\0'; // Re-null-terminate
            return *this;
        }

        /**
         * @brief Checks if the String is equal to another String
         * 
         * @param other - The other STRing to compare to
         * @return true - If the STRings are equal
        */
        bool operator==(const String& other) const {
            if (this->size() != other.size()) {
                return false;
            }
            return strncmp(this->Buffer, other.Buffer, this->size()) == 0;
        }

       // Copy assignment operator from String
        /**
         * @brief Copy assignment operator from String
         * 
         * @param other - The other STRing to copy
        */
        String& operator=(const String& other) {
            if (this != &other) {
                Copy(other.Buffer);
            }
            return *this;
        }

        /**
         * A simplified implementation of the sprintf function.
         *
         * @param STR The buffer to store the resulting formatted string. This buffer must be large enough to hold the formatted string, including the null terminator.
         * @param Format The format string. It can contain literal characters and format specifiers. Currently, only '%d' (for integers) and '%f' (for floats) are supported.
         * @param ... The values to format according to the format string.
         *
         * This function works by iterating over the format string. When it encounters a literal character, it copies it to the output string. 
         * When it encounters a format specifier, it retrieves the next argument from the argument list and formats it according to the specifier, 
         * then appends the formatted value to the output string. 
         * The formatted value is converted to a string using the ITOA function for integers and the FTOA function for floats.
         */
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
        /**
         * @brief Assignment operator from C-STRing
         * 
         * @param STR - The C-style STRing to assign
         * @return String& - The assigned STRing
        */
        String& operator=(const char* STR) {
            clear();
            if (STR) Copy(STR);
            return *this;
        }

        // Assignment operator from character
        /**
         * @brief Assignment operator from character
         * 
         * @param c - The character to assign
         * @return String& - The assigned STRing
        */
        String& operator=(char c) {
            clear();
            append(c);
            return *this;
        }

        // Assignment operator from integer
        /**
         * @brief Assignment operator from integer
         * 
         * @param value - The integer to assign
         * @return String& - The assigned STRing
        */
        String& operator=(int value) {
            char temp[12]; // 12 characters is enough for 32-bit integer
            SPRINTF(temp, "%d", value);
            *this = temp;
            return *this;
        }

        // Assignment operator from float
        /**
         * @brief Assignment operator from float
         * 
         * @param value - The float to assign
         * @return String& - The assigned STRing
        */
        String& operator=(float value) {
            char temp[32]; // 32 characters is enough for 32-bit float
            SPRINTF(temp, "%f", value);
            *this = temp;
            return *this;
        }

        

        // Append a character to the STRing
        /**
         * @brief Append a character to the STRing
         * 
         * @param c - The character to append
        */
        void append(char c) {
            Reallocate(Length + 2); // +1 for the new character, +1 for null terminator
            Buffer[Length++] = c;
        }

        // Append a C-STRing to the STRing
        /**
         * @brief Append a C-STRing to the STRing
         * 
         * @param STR - The C-style STRing to append
        */
        void append(const char* STR) {
            if (STR) {
                unsigned int newLength = Length + STR_LEN(STR);
                Reallocate(newLength + 1); // +1 for null terminator
                for (unsigned int i = 0; i < STR_LEN(STR); ++i) {
                    Buffer[Length++] = STR[i];
                }
            }
        }

        // Append another STRing to this STRing
        /**
         * @brief Append another STRing to this STRing
         * 
         * @param other - The other STRing to append
         * @return void
        */
        void append(const String& other) {
            append(other.Buffer);
        }

        /**
         * @brief trim the STRing (remove leading and trailing whitespace)
         * 
         * @param string - The STRing to trim
        */
        void trim(String& string){
            if (string.Buffer[0] == ' '){
                string.Buffer++;
            }
            if (string.Buffer[string.Length - 1] == ' '){
                string.Buffer[string.Length - 1] = '\0';
            }
        }

        

        /**
         * @brief does not equal operator
         * 
         * @param STR - The C-style STRing to append
         * @return bool - The appended STRing
        */
        bool operator!=(const char* STR) const {
            return !(*this == STR);
        }

        /**
         * @brief index of a character
         * 
         * @param c - The character to find
         * @param fromIndex - The index to start from
         * @return size_t - The index of the character
        */
        size_t indexOf(char c, size_t fromIndex = 0) const {
            for (size_t i = fromIndex; i < Length; ++i) {
                if (Buffer[i] == c) {
                    return i;
                }
            }
            return (size_t)-1;
        }
        // Append another String to this STRing

        


        // Get C-STRing
        /**
         * @brief Get C-STRing
         * 
         * @return const char* - The C-style STRing
        */
        const char* C_STR() const {
            return Buffer;
        }

        // Get Length of the STRing
        /**
         * @brief Get Length of the STRing
         * 
         * @return unsigned int - The Length of the STRing
        */
        unsigned int size() const {
            return Length;
        }

        // Clear the STRing
        /**
         * @brief Clear the STRing
        */
        void clear() {
            Reallocate(1); // Allocate a single character for null termination
        }

        // Compare with another STRing
        /**
         * @brief Compare with another STRing
         * 
         * @param other - The other STRing to compare to
         * @return int - The comparison result
        */
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
        /**
         * @brief Find Sub_String
         * 
         * @param subSTR - The Sub_String to find
         * @return int - The index of the Sub_String
        */
        int Find(const char* subSTR) const {
            const char* found = STRSTR(Buffer, subSTR);
            if (found) {
                return found - Buffer;
            }
            return -1;
        }

        // Convert to integer (basic implementation)
        /**
         * @brief Convert to integer (basic implementation)
         * 
         * @return int - The integer value
        */
        int toInt() const {
            int value = 0;
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] < '0' || Buffer[i] > '9') break;
                value = value * 10 + (Buffer[i] - '0');
            }
            return value;
        }

        // Convert to float (basic implementation)
        /**
         * @brief Convert to float (basic implementation)
         * 
         * @return float - The float value
        */
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
        /**
         * @brief Format STRing (basic implementation)
         * 
         * @param fmt - The format STRing
        */
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
        /**
         * @brief SubSTRing
         * 
         * @param start - The start index of the Sub_String
         * @param end - The end index of the Sub_String
         * @return String - The Sub_String
        */
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
        /**
         * @brief Reverse the STRing
        */
        void Reverse() {
            for (unsigned int i = 0; i < Length / 2; ++i) {
                char temp = Buffer[i];
                Buffer[i] = Buffer[Length - i - 1];
                Buffer[Length - i - 1] = temp;
            }
        }

        // Repeat the STRing
        /**
         * @brief Repeat the STRing
         * 
         * @param times - The number of times to repeat the STRing
         * @return String - The repeated STRing
        */
        String Repeat(unsigned int times) const {
            String result;
            for (unsigned int i = 0; i < times; ++i) {
                result += *this;
            }
            return result;
        }

        // Replace all occurrences of a Sub_String with another STRing
        /**
         * @brief Replace all occurrences of a Sub_String with another STRing
         * 
         * @param subSTR - The Sub_String to replace
         * @param Replacement - The STRing to replace the Sub_String with
         * @return String - The replaced STRing
        */
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
        /**
         * @brief Remove all occurrences of a character
         * 
         * @param c - The character to remove
         * @return String - The STRing with the character removed
        */
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
        /**
         * @brief Remove all occurrences of a Sub_String
         * 
         * @param subSTR - The Sub_String to remove
         * @return String - The STRing with the Sub_String removed
        */
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
        /**
         * @brief Convert to upper case
         * 
         * @return String - The upper case STRing
        */
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
        /**
         * @brief Convert to lower case
         * 
         * @return String - The lower case STRing
        */
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
        /**
         * @brief Trim whitespace from the Beginning and end of the STRing
         * 
         * @return String - The trimmed STRing
        */
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
        /**
         * @brief Split the STRing into Sub_Strings using a delimiter
         * 
         * @param delimiter - The delimiter to split the STRing by
         * @param count - The number of Sub_Strings
         * @return String* - The array of Sub_Strings
        */
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
        /**
         * @brief DeSTRuctor for Split result
         * 
         * @param result - The array of Sub_Strings
        */
        static void freeSplitResult(String* result) {
            delete[] result;
        }

        // Check if the STRing is empty
        /**
         * @brief Check if the STRing is empty
         * 
         * @return bool - If the STRing is empty
        */
        bool isEmpty() const {
            return Length == 0;
        }

        // Check if the STRing is a number
        /**
         * @brief Check if the STRing is a number
         * 
         * @return bool - If the STRing is a number
        */
        bool isNumber() const {
            for (unsigned int i = 0; i < Length; ++i) {
                if (Buffer[i] < '0' || Buffer[i] > '9') {
                    return false;
                }
            }
            return true;
        }

        // Check if the STRing is a float
        /**
         * @brief Check if the STRing is a float
         * 
         * @return bool - If the STRing is a float
        */
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
        /**
         * @brief Check if the STRing is a palindrome
         * 
         * @return bool - If the STRing is a palindrome
        */
        bool isPalindrome() const {
            for (unsigned int i = 0; i < Length / 2; ++i) {
                if (Buffer[i] != Buffer[Length - i - 1]) {
                    return false;
                }
            }
            return true;
        }

        // Check if the STRing contains a Sub_String
        /**
         * @brief Check if the STRing contains a Sub_String
         * 
         * @param subSTR - The Sub_String to check for
         * @return bool - If the STRing contains the Sub_String
        */
        bool contains(const char* subSTR) const {
            return STRSTR(Buffer, subSTR) != nullptr;
        }

        // Check if the STRing starts with a Sub_String
        /**
         * @brief Check if the STRing starts with a Sub_String
         * 
         * @param subSTR - The Sub_String to check for
         * @return bool - If the STRing starts with the Sub_String
        */
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
        /**
         * @brief Check if the STRing ends with a Sub_String
         * 
         * @param subSTR - The Sub_String to check for
         * @return bool - If the STRing ends with the Sub_String
        */
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
        /**
         * @brief Get Sub_String by index
         * 
         * @param index - The index of the Sub_String
         * @return char - The character at the index
        */
        char operator[](unsigned int index) const {
            if (index < Length) {
                return Buffer[index];
            }
            // Out-of-bounds access returns null character
            return '\0';
        }       

        /**
         * @brief toString... Converts a String toString.
        */
        String toString() const {
            return *this;
        }

        /**
         * @brief toString... Converts a char to String.
        */
        String toString(char value) const {
            return String(value);
        }

        /**
         * @brief toString... Converts an int to String.
        */
        String toString(int value) const {
            return String(value);
        }

        /**
         * @brief toString... Converts a float to String.
        */
        String toString(float value) const {
            return String(value);
        }

        /**
         * @brief print... Prints the STRing.
        */
        void print() const {
            Serial.print(Buffer);
        }
        // Output STReam operator

        /**
         * @brief removes a range of characters from the STRing
         * 
         * @param start - The start index of the range
         * @param end - The end index of the range
        */
        void removeRange(unsigned int start, unsigned int end) {
            if (start > end || start >= Length) {
                return;
            }
            if (end >= Length) {
                end = Length - 1;
            }
            unsigned int newLength = Length - (end - start + 1);
            for (unsigned int i = end + 1; i < Length; ++i) {
                Buffer[start++] = Buffer[i];
            }
            Length = newLength;
            Buffer[Length] = '\0'; // Null-terminate
        }


    };
    
};

#endif // CUSTOM_STRING_H