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

#define THROW_TIME_INVALID_EXP \
    throw ArgumentException("Time argument wasn't specified correctly, time must be specified in form of 'S-E' where 0 <= S < E")

#define return_if_empty(str) if (str == "") return

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

    void EnvArguments::set_path(string raw_path) {
        return_if_empty(raw_path);
        assert_path(raw_path);

        this->path = raw_path;
    }

    void EnvArguments::set_size(string raw_size) {
        return_if_empty(raw_size);
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

        this->size[0] = static_cast<uint8_t>(w);
        this->size[1] = static_cast<uint8_t>(h);
    }

    void EnvArguments::set_fps(string raw_fps) {
        return_if_empty(raw_fps);
        int fps;

        sscanf(raw_fps.c_str(), "%d", &fps);

        if (fps > 20 || fps < 1)
            THROW_FPS_INVALID_EXP(fps);

        this->fps = static_cast<uint8_t>(fps);
    }
    void EnvArguments::set_time(string raw_time) {
        return_if_empty(raw_time);
        uint32_t start, end;
        regex pattern("^\\d+-\\d+$");
        smatch match_info;

        if (!regex_match(raw_time, match_info, pattern)) {
            THROW_TIME_INVALID_EXP;
        }

        sscanf(raw_time.c_str(), "%u-%u", &start, &end);

        if (start >= end)
            THROW_TIME_INVALID_EXP;

        this->time[0] = start;
        this->time[1] = end;
    }

    const char* get_env(const char* name){
        try {
            return getenv(name);
        } catch(std::exception& exp) {
            return nullptr;
        }
    }

    EnvArguments& get_env_arguments() {
        EnvArguments& env_arguments = env_arguments_ns::env_arguments;

        env_arguments.set_path(get_env("INPUT_PATH"));
        env_arguments.set_fps(get_env("FPS"));
        env_arguments.set_size(get_env("SIZE"));
        env_arguments.set_time(get_env("TIME"));

        return env_arguments;
    }
}