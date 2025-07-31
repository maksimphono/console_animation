#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <cstring>
#include <format>

#include "include/frames.hpp"

using namespace std;
namespace fs = std::filesystem;

#define TEMP_PATH "./.frame.deleteme.jpg"

namespace frames_ns {
    string exec_command(string command, uint32_t buffer_size = 256) {
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

    bool check_path(string path) {
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

    Frame* pick_frame(string& path, uint8_t h, uint8_t m, uint8_t s, uint16_t ms, uint8_t size[2]) {
        constexpr const char* pick_frame_command_template = "ffmpeg -ss {0}:{1}:{2}.{3} -i \"{4}\" -frames:v 1 {5}";
        constexpr const char* convert_frame_command_template = "jp2a --size={0}x{1} {2}";
        string output = "";
        Frame* frame = nullptr;

        if (check_path(TEMP_PATH))
            fs::remove(TEMP_PATH); // clear the temporary file

        exec_command(format(pick_frame_command_template, h, m, s, ms, path, TEMP_PATH));

        if (!check_path(TEMP_PATH)) {
            cerr << "Couldn't pick a frame" << endl;
            return nullptr;
            // TODO: throw error
        }

        output = exec_command(format(convert_frame_command_template, size[0], size[1], TEMP_PATH));

        if (output.length() != (size[0] + 1) * size[1]) {
            cerr << "Couldn't pick a frame" << endl;
            return nullptr;
            // TODO: throw error
        }

        frame = new Frame(output, size);

        return frame;
    }
    
    void create_frames_from_video(string& path) {
        if (!check_path(path)) return;
        uint8_t size[2] = {66, 19}, fps = 2;

        cout << pick_frame(path, 0, 0, 0, 500, size)->body << endl;
        //Frame* frame = pick_frame(path, 0, 0, 0, 500, size);
    }
}