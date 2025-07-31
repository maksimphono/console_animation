#pragma once

#include <iostream>
#include <string>    // For std::string
#include <ncurses.h> // Include the ncurses library
#include <vector>    // For std::vector (to store lines of text)
#include <chrono>    // For std::chrono::seconds, std::chrono::milliseconds
#include <thread>
#include <cstdint>

#include "frames.hpp"

namespace render_ns {
    void render(vector<frames_ns::Frame> frames, uint8_t size[2], uint8_t fps);
}