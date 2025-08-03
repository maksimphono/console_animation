#include <iostream>
#include <regex>
#include <exception>
#include <format>

#include "include/env_arguments.hpp"
#include "include/exception.hpp"

using namespace std;

#define THROW_PATH_INVALID_EXP(path) \
    throw ArgumentException(format("Can't find file \"{0}\", make sure filename and extension is spelled correctly. Supported file formats: mp4", path))

#define THROW_SIZE_INVALID_EXP(size) \
    throw ArgumentException(format("Sorry, can't accept size \"{0}\". Make sure to specify size as 'WxH' where 1 <= W, H <= 255", size))

#define THROW_FPS_INVALID_EXP(fps) \
    throw ArgumentException(format("Sorry, can't accept fps value {0}. Make sure to specify fps value as integer 1 <= fps <= 20", fps))

namespace env_arguments_ns {
    EnvArguments env_arguments;

    DEFINE_EXCEPTION_CLASS(ArgumentException, "Exception with arguments");

    void assert_path(string value) {
        regex pattern("^((/|.|..)[^/\\0]+)*(/)?$");
        smatch match_info;

        if (!regex_match(value, match_info, pattern)) {
            THROW_PATH_INVALID_EXP(value);
        }
    }

    void get_argument_size(string raw_size, EnvArguments& env_arguments) {
        regex pattern("^\\d{1,3}x\\d{1,3}$");
        smatch match_info;

        if (!regex_match(raw_size, match_info, pattern)) {
            THROW_SIZE_INVALID_EXP(raw_size);
        }
        uint32_t w, h;

        sscanf(raw_size.c_str(), "%ux%u", &w, &h);

        if (1 > w || w > 255 || 1 > h || h > 255) {
            THROW_SIZE_INVALID_EXP(raw_size);
        }

        env_arguments.size[0] = static_cast<uint8_t>(w);
        env_arguments.size[1] = static_cast<uint8_t>(h);
    }

    void get_argument_fps(string raw_fps, EnvArguments& env_arguments) {
            int fps;
            sscanf(raw_fps.c_str(), "%d", &fps);

            if (fps > 20 || fps < 1)
                THROW_FPS_INVALID_EXP(fps);

            env_arguments.fps = static_cast<uint8_t>(fps);
    }

    EnvArguments& get_env_arguments() {        
        EnvArguments& env_arguments = env_arguments_ns::env_arguments;

        const char* path = getenv("INPUT_PATH");
        const char* raw_fps = getenv("FPS");
        const char* raw_size = getenv("SIZE");

        if (path != nullptr) {
            assert_path(path);
            env_arguments.path = string(path);
        }

        if (raw_fps != nullptr) {
            get_argument_fps(raw_fps, env_arguments);
        }

        if (raw_size != nullptr) {
            get_argument_size(raw_size, env_arguments);
        }

        return env_arguments;
    }
}