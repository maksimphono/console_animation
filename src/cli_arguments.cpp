#include <iostream>
#include <regex>
#include <exception>

#include "include/cli_arguments.hpp"

using namespace std;

#define THROW_WRONG_PATH_EXP(path) \
    throw ArgumentException(format("Path \"{0}\" is invalid. Input and output paths must be in a Unix style", path));

namespace cli_arguments_ns {
    CliArguments cli_arguments;

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
            THROW_WRONG_PATH_EXP(value);
        }
    }

    CliArguments& get_cli_arguments(int n_arg, const char** v_args) {
        CliArguments& cli_arguments = cli_arguments_ns::cli_arguments;

        for (int i = 1; i < n_arg; i++) {
            //cout << i << endl;
            switch (i) {
                case 1: 
                    assert_path(v_args[i]); 
                    cli_arguments.path = v_args[i]; 
                    break;
                case 2:
                    uint8_t fps;
                    sscanf(v_args[i], "%hhu", &fps);
                    cli_arguments.fps = fps;
                    break;
                case 3: 
                    cli_arguments.size = v_args[i]; 
                    break;
                default: break;
            }
        }

        return cli_arguments;
    }

}