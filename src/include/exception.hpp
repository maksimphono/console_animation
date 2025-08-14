#pragma once

#include <string>
#include <stdexcept>

using namespace std;

class Exception : public runtime_error {
public:
    explicit Exception(const char* message) : runtime_error(message) {}
    explicit Exception(string message) : runtime_error(message) {}
};


#define DEFINE_EXCEPTION_CLASS(class_name, default_message) \
class class_name : public Exception { \
public: \
    explicit class_name() : Exception(default_message) {} \
    explicit class_name(const std::string& message) : Exception(message) {} \
};