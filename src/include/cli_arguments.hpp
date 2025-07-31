#pragma once

#include <string>
#include <cstdint>

using namespace std;

/*
    Arguments are given in a particular order:
    path, size, fps
    
    If value wasn't provided - default will be used
*/

#define DEFAULT_FPS 2
#define DEFAULT_SZIE {55, 16}

namespace cli_arguments_ns {
    typedef struct {
        // dedicated structure, that will store values of every arguments
        string path = "";
        uint8_t fps = DEFAULT_FPS; // frames per second
        uint8_t size[2] = DEFAULT_SZIE; // default size
    } CliArguments;

    void assert_path(string value);

    CliArguments& get_cli_arguments(int , const char**);
}