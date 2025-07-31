#include <iostream>
#include <regex>
#include <exception>

#include "include/env_arguments.hpp"

using namespace std;


namespace env_arguments_ns {
    EnvArguments env_arguments;

    class ArgumentException : public exception {
    private:
        string _message;
    public:
        ArgumentException(const char* message) : _message(message) {}
        ArgumentException(string message) : _message(message) {}

        const char* what() const noexcept override {
            return this->_message.c_str();
        }
    };

    void assert_path(string value) {
        regex pattern("^((/|.|..)[^/\\0]+)*(/)?$");
        smatch match_info;

        if (!regex_match(value, match_info, pattern)) {
            // throw error
        }
    }

    EnvArguments& get_env_arguments() {        
        EnvArguments& env_arguments = env_arguments_ns::env_arguments;

        const char* path = getenv("INPUT_PATH");
        const char* raw_fps = getenv("FPS");
        const char* raw_size = getenv("SIZE");

        assert_path(path); 
        env_arguments.path = string(path);

        uint8_t fps;
        sscanf(raw_fps, "%hhu", &fps);
        env_arguments.fps = fps;

        uint8_t w, h;
        sscanf(raw_size, "%hhux%hhu", &w, &h);
        env_arguments.size[0] = w;
        env_arguments.size[1] = h;

        return env_arguments;
    }

}