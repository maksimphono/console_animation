#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <thread>

#include "include/render.hpp"
#include "include/env_arguments.hpp"
#include "include/storage.hpp"

using namespace std;

int main() {
    try {
        env_arguments_ns::EnvArguments& env_arguments = env_arguments_ns::get_env_arguments();        
        //vector<frames_ns::Frame> frames = storage_ns::load_file(env_arguments.name, env_arguments);
        if (env_arguments.list_stored_files) {
            storage_ns::list_all_files(cout);
        } else {
            vector<frames_ns::Frame> frames = frames_ns::create_frames(env_arguments);
            //vector<frames_ns::Frame> frames = frames_ns::create_frames_from_video(env_arguments);
            //storage_ns::save_file("fr_size-157x59_fps-5_time-0-5", frames, env_arguments);
            render_ns::render(frames, env_arguments);
        }
    } catch (const exception& exp) {
        cerr << exp.what() << endl;
        return 1;
    }

    return 0;
}