#include <cstdlib>
#include <filesystem>
#include <string>
#include <cstring>
#include <format>
#include <vector>

#include "utils/check_path.cpp"
#include "utils/exec_command.cpp"
#include "include/frames.hpp"

using namespace std;
namespace fs = std::filesystem;

#define TEMP_PATH "./.frame.deleteme.jpg"

namespace frames_ns {
    vector<Frame> _frames; // array, that holds all the frames

    Frame* pick_frame(string& path, Timestamp& ts, uint8_t size[2]) {
        // TODO: make this multithreaded
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

    vector<Frame>& create_frames_from_video(string& path, uint8_t size[2], uint8_t fps, uint32_t time_limit_sec = 20) {
        vector<Frame>& frames = _frames;

        if (!check_path(path)) return frames;
        uint16_t inc_ms = 1000 / fps;
        uint32_t duration = get_video_duration(path);
        duration = (time_limit_sec * 1000 > duration)?duration:(time_limit_sec * 1000);
        duration -= duration % inc_ms;
        Timestamp ts(0,0,1,0);

        frames.clear();

        while (ts.time < duration) {
            frames.push_back(*pick_frame(path, ts, size));
            ts.inc(inc_ms);
        }

        cleanup();

        return frames;
    }
}