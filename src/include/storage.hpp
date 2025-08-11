#pragma once

#include <format>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>
#include <ostream>

//#include "./frames.hpp"
//#include "./env_arguments.hpp"
#include "./exception.hpp"

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

#define THROW_FILE_NOT_FOUND_EXP(name) \
    throw StorageException("Sorry, file with this name wasn't found. Did you spell it correctly?")

namespace storage_ns {
    DEFINE_EXCEPTION_CLASS(StorageException, "Unknown error occured with storage!");
    
    const string storage_path("./storage");

    fs::path create_path(string& name);

    class StorageWriter : public ofstream {
    protected:
        template <typename T>
        uint32_t write_metadata_entry(const char* name, T value);
    public:
        StorageWriter(string name) : ofstream(create_path(name)) {}
        ~StorageWriter() {
            this->close();
        }

        uint32_t write_metadata(EnvArguments& arguments);
        // returns number of items written

        uint32_t write_frames(vector<Frame>& frames);
        // returns number of frames written
    };


    class StorageReader : public ifstream {
    protected:
        string read_metadata_line();
    public:
        StorageReader(string name) : ifstream(create_path(name)) {}
        ~StorageReader() {
            this->close();
        }        
        EnvArguments& read_metadata(EnvArguments& arguments);

        vector<Frame> read_frames(EnvArguments& arguments);
    };

    void list_all_files(ostream& stream = std::cout, string separator = "\n");

    bool check_exsistance(string& name);

    void write_meta_data(ofstream& file, string& name, EnvArguments& arguments);

    uint32_t save_file(string name, vector<Frame>& frames, EnvArguments& arguments);
    // saves the sequence of frames along with specific arguments to a new file

    vector<Frame> load_file(string name, EnvArguments& arguments);
    // loads sequence of frames by a name from file, also must set arguments to specific values, written in the file
}