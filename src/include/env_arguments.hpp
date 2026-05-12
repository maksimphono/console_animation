#pragma once

#if __has_include(<format>)
    #include <format>
#else
    #include <fmt/format.h>
    using namespace fmt;
#endif

#include <string>
#include <cstdint>

#include "exception.hpp"

namespace storage_ns {
    class StorageReader;
}

using namespace std;
using StorageReader = storage_ns::StorageReader;

#define DEFAULT_FPS 2
#define DEFAULT_SIZE {55, 16}
#define DEFAULT_TIME {0, 10}

#define MAX_FPS 24
#define MIN_FPS 1
#define MAX_SIZE 255
#define MIN_SIZE 4

#define THROW_PATH_INVALID_EXP(path) \
    throw ArgumentException(format("Can't find file \"{0}\", make sure filename and extension is spelled correctly. Supported file formats: mp4", path))

#define THROW_SIZE_INVALID_EXP(size) \
    throw ArgumentException(format("Sorry, can't accept size \"{0}\". Make sure to specify size as 'WxH' where 1 <= W, H <= 255", size))

#define THROW_FPS_INVALID_EXP(fps) \
    throw ArgumentException(format("Sorry, can't accept fps value {0}. Make sure to specify fps value as integer 1 <= fps <= 20", fps))

#define THROW_TIME_INVALID_EXP \
    throw ArgumentException("Time argument wasn't specified correctly, time must be specified in form of 'S-E' where 0 <= S < E")


namespace env_arguments_ns {
    DEFINE_EXCEPTION_CLASS(ArgumentException, "Exception with arguments");

    typedef struct RawArguments {
        bool list_stored_files;
        bool delete_file;
        bool write_to_stdout;
        string name;
        string path;
        string fps;
        string size;
        string time;
        unsigned int cores;
    } RawArguments;

    class EnvArguments {
    public:
        // dedicated structure, that will store values of every arguments
        string name = "";
        string path = "";
        unsigned int cores = 1;
        uint8_t fps = DEFAULT_FPS; // frames per second
        uint8_t size[2] = DEFAULT_SIZE; // default size
        uint32_t time[2] = DEFAULT_TIME;
        bool loaded_from_file = false;
        bool list_stored_files = false;
        bool delete_file = false;
        bool write_to_stdout = false;

        EnvArguments() {}

        void set_path(string raw_path);
        void set_size(string raw_size);
        void set_fps(string raw_fps);
        void set_time(string time);
        void set_name(string name);
        void set_cores(unsigned int);
    };

    void assert_path(string value);

    EnvArguments& get_env_arguments(int, char**);
    int get_raw_arguments(RawArguments&, int, char**);

}