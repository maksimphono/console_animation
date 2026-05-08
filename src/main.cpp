/*
#include <iostream>
#include "utils/exec_command.cpp"

using namespace std;
using namespace exec_command_ns;

typedef int Pipe;

class C{
public:
    string s;
    C(string& s) {
        this->s = s;
    }
};

int main() {
    CommandExecutor ff("ffmpeg -loglevel -8 -ss %d:%d:%d.000 -i %s -frames:v 1 -f image2pipe -", 512);
    CommandExecutor jp("jp2a - --size=%dx%d", 512);

    //cout << ff.command_templates.at(ff.command_templates.size() - 1).tmplt << endl;

    //string s = ff.exec<string>(-9, 0, 0, 2, "/home/Rick.mp4");
    //int p = ff.exec<int>(-9, 0, 0, 2, "/home/Rick.mp4");
    int p = ff.exec<int>(-9, 0, 0, 2, "/home/build/Rick Astle Never Gonna give you up (Full version).mp4");
    string s = jp.exec<string>(p, 70, 30);

    cout << s;
    return 0;
}
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <thread>

#include "include/render.hpp"
#include "include/env_arguments.hpp"
#include "include/storage.hpp"

#include "include/CLI11.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        env_arguments_ns::EnvArguments& env_arguments = env_arguments_ns::get_env_arguments(argc, argv);
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