#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

#include "./frames.hpp"
#include "./env_arguments.hpp"

using namespace std;
using EnvArguments = env_arguments_ns::EnvArguments;
using Frame = frames_ns::Frame;

namespace fs = filesystem;

namespace storage_ns {
    typedef map<string, string> EnvArgsMap;

    const string storage_path("./storage");

    vector<string> list_all_files();

    fs::path create_path(string& name);

    bool check_exsistance(string& name);

    void write_meta_data(ofstream& file, string& name, EnvArguments& arguments);

    void save_file(string name, vector<Frame>& frames, EnvArguments& arguments);
    // saves the sequence of frames along with specific arguments to a new file

    vector<Frame> load_file(string& name, EnvArguments& arguments);
    // loads sequence of frames by a name from file, also must set arguments to specific values, written in the file

    EnvArgsMap load_metadata(string name, EnvArguments& arguments);
}