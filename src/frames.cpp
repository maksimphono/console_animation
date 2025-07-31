#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <cstring>
#include <format>
#include <vector>

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

        delete[] buffer;

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

    Frame* pick_frame(string& path, Timestamp& ts, uint8_t size[2]) {
        constexpr const char* pick_frame_command_template = "ffmpeg -loglevel -8 -ss {0}:{1}:{2}.{3} -i \"{4}\" -frames:v 1 {5}";
        constexpr const char* convert_frame_command_template = "jp2a --size={0}x{1} {2}";
        string output = "";
        Frame* frame = nullptr;

        if (check_path(TEMP_PATH))
            fs::remove(TEMP_PATH); // clear the temporary file

        exec_command(format(pick_frame_command_template, ts.hours, ts.minutes, ts.seconds, ts.miliseconds, path, TEMP_PATH));

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

    uint32_t get_video_duration(string& path) {
        constexpr const char* command_template = "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"{0}\"";
        double duration = 0;
        const string output = exec_command(format(command_template, path));

        if (output.length() == 0) {
            // TODO: throw an error
        }

        sscanf(output.c_str(), "%lf", &duration);

        return static_cast<uint32_t>(duration * 1000); // duration in miliseconds
    }

    void cleanup() {
        if (check_path(TEMP_PATH)) {
            fs::remove(TEMP_PATH);
        }
    }
    
    vector<Frame> frames;

    vector<Frame>& create_frames_from_video(string& path, uint8_t size[2], uint8_t fps) {
        vector<Frame>& frames = frames_ns::frames;
        if (!check_path(path)) return frames;
        uint16_t inc_ms = 1000 / fps;
        uint32_t duration = 5100 - 5100 % inc_ms;// get_video_duration(path);
        Timestamp ts(0,0,0,0);

        frames.clear();

        while (ts.time < duration) {
            frames.push_back(*pick_frame(path, ts, size));
            ts.inc(inc_ms);
            //cout << format("{0}:{1}:{2}:{3}\n", ts.hours, ts.minutes, ts.seconds, ts.miliseconds);
        }

        cleanup();

        return frames;
    }
}