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
            //this->body[this->len - 1] = '\0';
        }
        ~Frame() {
        }
        void put(char* screen) {
            //memcpy(screen, this->body, this->len);
        }
    };

    class Timestamp {
    public:
        uint8_t hours;
        uint8_t minutes; 
        uint8_t seconds; 
        uint16_t miliseconds;
        uint32_t time; // total time in miliseconds
        Timestamp(uint8_t h = 0, uint8_t m = 0, uint8_t s = 0, uint16_t ms = 0)
            : hours(h), minutes(m), seconds(s), miliseconds(ms) {
            this->time = this->miliseconds + (uint32_t)seconds * 1000 + (uint32_t)minutes * 60000 + (uint32_t)hours * 3600000;
        }
        ~Timestamp() {}
        void inc(uint16_t ms) {
            this->time += (uint32_t)ms;

            this->miliseconds = this->time % 1000;
            this->seconds = (this->time / 1000) % 60;
            this->minutes = (this->time / 60000) % 60;
            this->hours = this->time / 3600000;
        }
    };

    Frame* pick_frame(string& path, Timestamp& ts, uint8_t size[2]);

    uint32_t get_video_duration(string& path);

    void cleanup();

    vector<Frame>& create_frames_from_video(string& path, uint8_t size[2], uint8_t fps, uint32_t);
}