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
        if (env_arguments.delete_file) {
            storage_ns::delete_file(env_arguments.name);
        } else if (env_arguments.list_stored_files) {
            storage_ns::list_all_files(cout);
        } else {
            vector<frames_ns::Frame> frames = frames_ns::create_frames(env_arguments);
            render_ns::render(frames, env_arguments);
        }
    } catch (const exception& exp) {
        cerr << exp.what() << endl;
        return 1;
    }

    return 0;
}