#include <iostream>
#include <string>    // For std::string
#include <vector>    // For std::vector (to store lines of text)
#include <cstdint>

#include "include/render.hpp"
#include "include/env_arguments.hpp"

using namespace std;

int main() {
    try {
        env_arguments_ns::EnvArguments& env_arguments = env_arguments_ns::get_env_arguments();        
        vector<frames_ns::Frame> frames = frames_ns::create_frames_from_video(env_arguments.path, env_arguments.size, env_arguments.fps, 5);
        render_ns::render(frames, env_arguments.fps);
    } catch (const exception& exp) {
        cerr << exp.what() << endl;
        return 1;
    }

    return 0;
}