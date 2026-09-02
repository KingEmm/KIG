#include "StringUtils.h"

std::string StringUtils::wcharToString(const WCHAR* wstr) {
    if (!wstr) return "";

    // Get required buffer size for UTF-8 string
    int sizeNeeded = WideCharToMultiByte(
        CP_UTF8,            // Convert to UTF-8
        0,                  // No special flags
        wstr,               // Source wide string
        -1,                 // Null-terminated
        nullptr, 0,         // No output buffer yet
        nullptr, nullptr    // No default char
    );

    if (sizeNeeded <= 0) return "";

    // Allocate buffer and perform conversion
    std::string str(sizeNeeded - 1, '\0'); // -1 to exclude null terminator
    WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr,
        -1,
        &str[0],
        sizeNeeded,
        nullptr,
        nullptr
    );

    return str;
}
std::string StringUtils::wcharToStringWithLen(const WCHAR* wstr, int sizeNeeded) {
    std::string str(sizeNeeded , '\0'); // -1 to exclude null terminator
    WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr,
        -1,
        &str[0],
        sizeNeeded,
        nullptr,
        nullptr
    );

    return str;
}