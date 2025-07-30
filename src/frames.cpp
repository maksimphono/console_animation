#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

#include "include/frames.hpp"

using namespace std;
namespace fs = std::filesystem;


namespace frames_ns {
    string exec_command(string& command, uint32_t buffer_size = 256) {
        string result;
        char* buffer = new char[buffer_size];
        FILE* pipe = popen(command.c_str(), "r");
        
        if (!pipe) {
            std::cerr << "Error: popen() failed to execute command: " << command << std::endl;
            // TODO: throw an error
            return "";
        }

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }

        int status = pclose(pipe);

        if (status == -1) {
            cerr << "Can't close pipe" << endl;
            // TODO: throw an error
        }

        delete buffer;

        return result;
    }

    bool check_path(string& path) {
        if (!fs::exists(path)) {
            cerr << "Can't open file " << path << endl;
            // TODO: throw an error
            return false;
        }
        FILE* f = fopen(path.c_str(), "rb");
        if (!f) {
            // TODO: throw an error
            cerr << "Error opening the file " << path << endl;
            return false;
        }
        fclose(f);

        return true;
    }
    
    void create_frames_from_video(string& path) {
        if (!check_path(path)) return;

        
    }
}