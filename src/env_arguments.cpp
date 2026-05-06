#include <iostream>
#include <regex>
#include <exception>
#include <format>
#include <cstring>

#include "include/env_arguments.hpp"
#include "include/storage.hpp"
#include "include/exception.hpp"
#include "include/CLI11.hpp"

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

    static const string empty = "";
    static RawArguments default_arguments = {false, false, "", "", "2", "55x16", "0-10"};

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

    void EnvArguments::set_name(string raw_name){
        this->name = raw_name;
    }

    int get_raw_arguments(RawArguments& raw_arguments, int argc, char** argv){
        CLI::App app{"App description"};
        argv = app.ensure_utf8(argv);

        app.add_option("-i,--input", raw_arguments.path, "Path to input file");
        app.add_option("-f,--fps", raw_arguments.fps, "FPS");
        app.add_option("-s,--size", raw_arguments.size, "Size of the resulting video (width * height) in symbols");
        app.add_option("-t,--time", raw_arguments.time, "Which time fragment of the input file to convert");
        app.add_option("-n,--name", raw_arguments.name, "Name of the video");
        app.add_option("-d,--del", raw_arguments.delete_file, "Whether to delete video with specified name (name must be specified with '--name')");
        app.add_option("-l,--list", raw_arguments.list_stored_files, "List all saved files");

        CLI11_PARSE(app, argc, argv);
        return 0;
    }

    EnvArguments& get_env_arguments(int argc, char** argv) {
        EnvArguments& env_arguments = env_arguments_ns::env_arguments;
        RawArguments& raw_arguments = env_arguments_ns::default_arguments;

        get_raw_arguments(raw_arguments, argc, argv);

        if (raw_arguments.list_stored_files) {
            // if user want to list files, no other work is done
            env_arguments.list_stored_files = true;
        } else if (raw_arguments.name != empty) {
            if (raw_arguments.delete_file) {
                // if user wants to delete a file from the storage
                env_arguments.delete_file = true;
            } else {
                // frames will be loaded from the file
                env_arguments.loaded_from_file = true;
            }
            env_arguments.set_name(raw_arguments.name);
        } else {
            // frames will be created from mp4 file
            env_arguments.set_path(raw_arguments.path);
            env_arguments.set_fps(raw_arguments.fps);
            env_arguments.set_size(raw_arguments.size);
            env_arguments.set_time(raw_arguments.time);
        }

        return env_arguments;
    }
}