#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>
#include <cstring>
#include <format>
#include <iostream>
#include <vector>

#include "./exception.hpp"

using namespace std;

#define THROW_CANT_CREATE_TEMP_DIR_EXP(path) \
    throw FramesException(format("Error, can't create temporary directory {0}.", path));

#define THROW_JP2A_PROGRAM_ISSUE_EXP \
    throw FramesException(format("Error, something is wrong with underlying utility 'jp2a'."));

#define THROW_VIDEO_DURATION_EXP \
    throw FramesException(format("Error, something is wrong with underlying utility 'ffprobe'."));


namespace frames_ns {
    
    DEFINE_EXCEPTION_CLASS(FramesException, "Somthing went wrong with frames creation");
    class Frame {
    public:
        uint16_t len = 0;
        string body = ""; // flat array, that contains all symbols of the frame
        uint8_t size[2] = {};
    public:
        Frame(string& chars, uint8_t size[2]){
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
        Timestamp(uint8_t h = 0, uint8_t m = 0, uint8_t s = 0, uint16_t ms = 0);
        void inc(uint16_t ms);
    };

    Frame pick_frame(string& path, Timestamp& ts, uint8_t size[2]);

    uint32_t get_video_duration(string& path);

    void cleanup();

    vector<Frame>& create_frames_from_video(string& path, uint8_t size[2], uint8_t fps, uint32_t);
}