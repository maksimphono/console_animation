#include <iostream>
#include <string>    // For std::string
#include <ncurses.h> // Include the ncurses library
#include <vector>    // For std::vector (to store lines of text)
#include <chrono>    // For std::chrono::seconds, std::chrono::milliseconds
#include <thread>
#include <cstdint>

//#include "include/frames.hpp"
#include "include/render.hpp"

//using namespace frames_ns;
using namespace std;

int main() {
    try {
        string p = string("../Rick Astley - Never Gonna Give You Up (Official Video) (4K Remaster) [dQw4w9WgXcQ].mp4");
        uint8_t size[2] = {187, 69};
        vector<frames_ns::Frame> frames = frames_ns::create_frames_from_video(p, size, 2);
        render_ns::render(frames, size, 2);
    } catch (const exception& exp) {
        cerr << exp.what() << endl;
        return 1;
    }

    return 0;
}