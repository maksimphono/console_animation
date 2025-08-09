#pragma once

#include <ncurses.h>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdint>
#include <csignal>
#include <unistd.h>

#include "frames.hpp"
#include "exception.hpp"

namespace render_ns {

    DEFINE_EXCEPTION_CLASS(StorageSaveException, "Unknown error! Unable to save the file :((");

    void sleep(uint32_t ms);
    void render(vector<frames_ns::Frame> frames, EnvArguments&);
}