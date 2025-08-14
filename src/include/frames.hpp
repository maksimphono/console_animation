#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>
#include <cstring>
#include <format>
#include <iostream>
#include <vector>
#include <future>

#include <thread>
#include <queue>
#include <map>

#include "./env_arguments.hpp"
#include "./exception.hpp"

using namespace std;
using EnvArguments = env_arguments_ns::EnvArguments;

#define THROW_CANT_CREATE_TEMP_DIR_EXP(path) \
    throw FramesException(format("Error, can't create temporary directory {0}.", path));

#define THROW_JP2A_PROGRAM_ISSUE_EXP \
    throw FramesException(format("Error, something is wrong with underlying utility 'jp2a'."));

#define THROW_VIDEO_DURATION_EXP \
    throw FramesException(format("Error, something is wrong with underlying utility 'ffprobe'."));

#define THROW_INPUT_FILE_NOT_FOUND_EXP \
    throw FramesException("Error, input video file wasn't found. Did you spell it correctly?");


namespace frames_ns {
    
    DEFINE_EXCEPTION_CLASS(FramesException, "Somthing went wrong with frames creation");
    class Frame {
    public:
        uint16_t len = 0;
        string body = ""; // flat array, that contains all symbols of the frame
        uint8_t size[2] = {};
    public:
        Frame() {}
        Frame(string& chars, uint8_t size[2]){
            this->len = (uint16_t)(size[0] + 1) * (uint16_t)size[1];
            this->body = string(chars);
        }
        Frame(char* chars, uint8_t size[2]){
            this->len = (uint16_t)(size[0] + 1) * (uint16_t)size[1];
            this->body = string(chars);
        }
        void set(string& chars, uint8_t size[2]) {
            this->len = (uint16_t)(size[0] + 1) * (uint16_t)size[1];
            this->body = string(chars);
        }
    };

    class Timestamp {
    public:
        uint8_t hours;
        uint8_t minutes; 
        uint8_t seconds; 
        uint16_t miliseconds;
        uint32_t time; // total time in miliseconds
        Timestamp(uint8_t h, uint8_t m, uint8_t s, uint16_t ms);
        Timestamp(uint32_t time_ms = 0);
        void inc(uint16_t ms);
        string to_string();
    };

    Frame pick_frame(string& path, Timestamp& ts, uint8_t size[2]);

    uint32_t get_video_duration(string& path);

    void cleanup();

    vector<Frame>& create_frames_from_video(EnvArguments&);

    vector<Frame>& create_frames(EnvArguments&);
}