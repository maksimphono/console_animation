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
#define DEFAULT_SIZE {55, 16}
#define DEFAULT_TIME {0, 10}

namespace env_arguments_ns {
    typedef struct {
        // dedicated structure, that will store values of every arguments
        string path = "";
        uint8_t fps = DEFAULT_FPS; // frames per second
        uint8_t size[2] = DEFAULT_SIZE; // default size
        uint32_t time[2] = DEFAULT_TIME;
    } EnvArguments;

    void assert_path(string value);

    void get_argument_size(string raw_size, EnvArguments& env_arguments);

    void get_argument_fps(string raw_fps, EnvArguments& env_arguments);

    EnvArguments& get_env_arguments();
}