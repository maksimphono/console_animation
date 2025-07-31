#pragma once

#include <ncurses.h>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdint>

#include "frames.hpp"

namespace render_ns {
    void sleep(uint32_t ms);
    void render(vector<frames_ns::Frame> frames, uint8_t fps);
}