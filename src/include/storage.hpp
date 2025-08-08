#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

//#include "./frames.hpp"
//#include "./env_arguments.hpp"

namespace frames_ns {
    class Frame;
}

namespace env_arguments_ns {
    class EnvArguments;
}

using namespace std;
using EnvArguments = env_arguments_ns::EnvArguments;
using Frame = frames_ns::Frame;

namespace fs = filesystem;

namespace storage_ns {
    typedef map<string, string> EnvArgsMap;

    const string storage_path("./storage");

    fs::path create_path(string& name);

    class StorageReader : public ifstream {
    private:
        vector<string> data;
        EnvArgsMap env_args;

        string read_metadata_line();
    public:
        StorageReader(string name) : ifstream(create_path(name)) {}
        ~StorageReader() {
            this->close();
        }        
        EnvArguments& read_metadata(EnvArguments& arguments);

        vector<Frame> read_frames(EnvArguments& arguments);
    };

    vector<string> list_all_files();

    bool check_exsistance(string& name);

    void write_meta_data(ofstream& file, string& name, EnvArguments& arguments);

    void save_file(string name, vector<Frame>& frames, EnvArguments& arguments);
    // saves the sequence of frames along with specific arguments to a new file

    vector<Frame> load_file(string name, EnvArguments& arguments);
    // loads sequence of frames by a name from file, also must set arguments to specific values, written in the file
}