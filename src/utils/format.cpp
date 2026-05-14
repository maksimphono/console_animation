#pragma once
#include <stdarg.h>
#include <string>
#include <string.h>

static std::string myformat(const char* fmt, ...) {
    std::string result;
    va_list args, args_cpy;

    va_start(args, fmt);
    va_copy(args_cpy, args);
    
    size_t size = vsnprintf(nullptr, 0, fmt, args);
    char* buffer = new char[size + 1];
    memset(buffer, 0, size + 1);

    vsprintf(buffer, fmt, args_cpy);

    result = string(buffer);

    delete[] buffer;
    va_end(args_cpy);
    va_end(args);

    return result;
}