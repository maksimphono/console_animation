#include <iostream>
#include <regex>
#include <exception>

#include "include/cli_arguments.hpp"

using namespace std;


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
            // throw error
        }
    }

    CliArguments& get_cli_arguments(int n_arg, const char** v_args) {
        CliArguments& cli_arguments = cli_arguments_ns::cli_arguments;

        for (int i = 1; i < n_arg; i++) {
            const char* argument = v_args[i];
            switch (i) {
                case 1: 
                    assert_path(argument); 
                    cli_arguments.path = argument; 
                    break;
                case 2:
                    uint8_t fps;
                    sscanf(argument, "%hhu", &fps);
                    cli_arguments.fps = fps;
                    break;
                case 3: 
                    uint8_t w, h;
                    sscanf(argument, "%hhux%hhu", &w, &h);
                    cli_arguments.size[0] = w;
                    cli_arguments.size[1] = h;
                    break;
                default: break;
            }
        }

        return cli_arguments;
    }

}