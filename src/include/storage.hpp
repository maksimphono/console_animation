#pragma once

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
    public:
        StorageReader(string name) : ifstream(create_path(name)) {
            //string raw_content = "";

            //cout << raw_content;
        }
        ~StorageReader() {
            this->close();
        }
        EnvArgsMap& get_arguments() {
            EnvArgsMap& args = this->env_args;
            if (!this->is_open()) return args;
            uint32_t metadata_length = 0;
            char* metadata_line = nullptr;
            regex pattern("[^;]+=[^;]+");

            for (char ch = '\0'; ch != '\n'; ch = this->get()) metadata_length++;
            metadata_line = new char[metadata_length];

            this->seekg(0);
            this->getline(metadata_line, metadata_length);

            string metadata_str(metadata_line);
            delete[] metadata_line;

            auto words_begin = std::sregex_iterator(metadata_str.begin(), metadata_str.end(), pattern);
            auto words_end = std::sregex_iterator();

            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                string str(i->str());
                args[str.substr(0, str.find("="))] = str.substr(str.find("=") + 1);
            }

            return args;
        }

        const char* get_env(const char* name) {
            if (this->env_args.empty())
                this->get_arguments();
            return this->env_args[string(name)].c_str();
        }
    };

    vector<string> list_all_files();

    bool check_exsistance(string& name);

    void write_meta_data(ofstream& file, string& name, EnvArguments& arguments);

    void save_file(string name, vector<Frame>& frames, EnvArguments& arguments);
    // saves the sequence of frames along with specific arguments to a new file

    vector<Frame> load_file(string& name, EnvArguments& arguments);
    // loads sequence of frames by a name from file, also must set arguments to specific values, written in the file

    EnvArgsMap load_metadata(string name, EnvArguments& arguments);
}