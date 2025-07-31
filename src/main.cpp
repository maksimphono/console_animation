#include <iostream>
#include <string>    // For std::string
#include <ncurses.h> // Include the ncurses library
#include <vector>    // For std::vector (to store lines of text)
#include <chrono>    // For std::chrono::seconds, std::chrono::milliseconds
#include <thread>
#include <cstdint>

//#include "include/frames.hpp"
#include "include/render.hpp"
#include "include/env_arguments.hpp"

//using namespace frames_ns;
using namespace std;

int main() {
    try {
        env_arguments_ns::EnvArguments& env_arguments = env_arguments_ns::get_env_arguments();
        //string p = string("../Rick Astley - Never Gonna Give You Up (Official Video) (4K Remaster) [dQw4w9WgXcQ].mp4");
        //uint8_t size[2] = {187, 69};
        
        //vector<frames_ns::Frame> frames = frames_ns::create_frames_from_video(env_arguments.path, env_arguments.size, env_arguments.fps);
        //render_ns::render(frames, env_arguments.size, 2);
    } catch (const exception& exp) {
        cerr << exp.what() << endl;
        return 1;
    }

    return 0;
}