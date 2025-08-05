#pragma once

#include <string>
#include <cstdint>

using namespace std;

#define DEFAULT_FPS 2
#define DEFAULT_SIZE {55, 16}
#define DEFAULT_TIME {0, 10}

namespace env_arguments_ns {
    class EnvArguments {
    public:
        // dedicated structure, that will store values of every arguments
        string path = "";
        uint8_t fps = DEFAULT_FPS; // frames per second
        uint8_t size[2] = DEFAULT_SIZE; // default size
        uint32_t time[2] = DEFAULT_TIME;

        EnvArguments() {}

        void set_path(string raw_path);
        void set_size(string raw_size);
        void set_fps(string raw_fps);
        void set_time(string time);
    };

    void assert_path(string value);

    EnvArguments& get_env_arguments();
}