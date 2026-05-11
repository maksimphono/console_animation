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

#define THROW_CANT_DELETE_FILE_EXP \
    throw StorageException("Unexpected error while removing the file. Check file permissions.")

#define APP_REL_PATH fs::path(".conanim")
#define STORAGE_REL_PATH fs::path(".conanim/storage")

namespace storage_ns {
    DEFINE_EXCEPTION_CLASS(StorageException, "Unknown error occured with storage!");

    static fs::path storage_path = fs::path("");
    void init_storage_path();

    fs::path create_path(string& name);

    class Writer {
    public:
        virtual ~Writer() = default;
        template <typename T>
        uint32_t write_metadata_entry(const char* name, T value);

        virtual uint32_t write_metadata(EnvArguments& arguments) = 0;
        // returns number of items written

        virtual uint32_t write_frames(vector<Frame>& frames) = 0;
        // returns number of frames written
    protected:
        virtual ostream& get_stream() = 0;
    };

    class StorageWriter : public Writer {
    private:
        ofstream strm;
    public:
        StorageWriter(string name) : strm(create_path(name)) {}
        ~StorageWriter() {this->strm.close();}

        uint32_t write_metadata(EnvArguments& arguments) override;

        uint32_t write_frames(vector<Frame>& frames) override;

    protected:
        ostream& get_stream() override {return this->strm;}
    };

    class StreamWriter : public Writer {
    private:
        ostream& strm;
    public:
        StreamWriter(ostream& stream) : strm(stream) {}
        ~StreamWriter() {this->flush();}

        uint32_t write_metadata(EnvArguments& arguments) override;

        uint32_t write_frames(vector<Frame>& frames) override;

        void flush();
    protected:
        ostream& get_stream() override {return this->strm;}
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

    void delete_file(string name);

    uint32_t write_to_stdout(vector<Frame>& frames, EnvArguments& arguments);

    uint32_t save_file(string name, vector<Frame>& frames, EnvArguments& arguments);
    // saves the sequence of frames along with specific arguments to a new file

    vector<Frame> load_file(string name, EnvArguments& arguments);
    // loads sequence of frames by a name from file, also must set arguments to specific values, written in the file
}