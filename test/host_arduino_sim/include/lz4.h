#ifndef HOST_ARDUINO_SIM_LZ4_H
#define HOST_ARDUINO_SIM_LZ4_H

#include <cstring>

// Host simulation: behave as a passthrough codec so Json code can compile
// and exercise file paths without requiring real compression binaries.
inline int LZ4_compress_default(const char* src, char* dst, int srcSize, int dstCapacity) {
    if (!src || !dst || srcSize < 0 || dstCapacity < 0) {
        return 0;
    }
    if (srcSize > dstCapacity) {
        return 0;
    }
    if (srcSize == 0) {
        return 0;
    }
    std::memcpy(dst, src, static_cast<size_t>(srcSize));
    return srcSize;
}

inline int LZ4_compressBound(int srcSize) {
    return srcSize;
}

inline int LZ4_decompress_safe(const char* src, char* dst, int compressedSize, int dstCapacity) {
    if (!src || !dst || compressedSize < 0 || dstCapacity < 0) {
        return -1;
    }
    if (compressedSize > dstCapacity) {
        return -1;
    }
    if (compressedSize == 0) {
        return 0;
    }
    std::memcpy(dst, src, static_cast<size_t>(compressedSize));
    return compressedSize;
}

#endif
