#include "include/frames.hpp"
#include "utils/check_path.cpp"
#include "utils/exec_command.cpp"
#include "utils/TerminalRestorer.cpp"
#include "utils/ThreadPool.cpp"

#include "include/storage.hpp"

namespace fs = std::filesystem;

using namespace std;
using ThreadPool = threadpool_ns::ThreadPool;

#define TEMP_PATH "./.frames.deleteme"

namespace frames_ns {
    vector<Frame> _frames; // array, that holds all the frames

    Timestamp::Timestamp(uint8_t h, uint8_t m, uint8_t s, uint16_t ms)
        : hours(h), minutes(m), seconds(s), miliseconds(ms) {
        this->time = this->miliseconds + (uint32_t)seconds * 1000 + (uint32_t)minutes * 60000 + (uint32_t)hours * 3600000;
    }

    Timestamp::Timestamp(uint32_t time_ms) {
        this->time = time_ms;

        this->miliseconds = this->time % 1000;
        this->seconds = (this->time / 1000) % 60;
        this->minutes = (this->time / 60000) % 60;
        this->hours = this->time / 3600000;
    }

    void Timestamp::inc(uint16_t ms) {
        this->time += (uint32_t)ms;

        this->miliseconds = this->time % 1000;
        this->seconds = (this->time / 1000) % 60;
        this->minutes = (this->time / 60000) % 60;
        this->hours = this->time / 3600000;
    }

    string Timestamp::to_string() {
        return format("{0}h{1}m{2}s{3}ms", this->hours, this->minutes, this->seconds, this->miliseconds);
    }

    Frame pick_frame(string& path, Timestamp& ts, uint8_t size[2]) {
        constexpr const char* pick_frame_command_template = "ffmpeg -loglevel -8 -ss {0}:{1}:{2}.{3} -i \"{4}\" -frames:v 1 {5}";
        constexpr const char* convert_frame_command_template = "jp2a --size={0}x{1} {2}";
        string output = "";
        string temp_file_path = format("{0}/.frame_{1}.jpg", TEMP_PATH, ts.to_string());

        if (check_path(temp_file_path))
            fs::remove(temp_file_path);

        exec_command(format(pick_frame_command_template, ts.hours, ts.minutes, ts.seconds, ts.miliseconds, path, temp_file_path));

        if (!check_path(temp_file_path)) {
            THROW_CANT_CREATE_TEMP_DIR_EXP(temp_file_path);
        }

        output = exec_command(format(convert_frame_command_template, size[0], size[1], temp_file_path));

        if (output.length() != (size[0] + 1) * size[1]) {
            THROW_JP2A_PROGRAM_ISSUE_EXP;
        }

        return Frame(output, size);
    }

    uint32_t get_video_duration(string& path) {
        constexpr const char* command_template = "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"{0}\"";
        double duration = 0;
        const string output = exec_command(format(command_template, path));

        if (output.length() == 0) {
            THROW_VIDEO_DURATION_EXP;
        }

        sscanf(output.c_str(), "%lf", &duration);

        return static_cast<uint32_t>(duration * 1000); // duration in miliseconds
    }

    void cleanup() {
        if (check_path(TEMP_PATH)) {
            for (const auto& file : fs::directory_iterator(TEMP_PATH)) {
                if (fs::is_regular_file(file.status()))
                    fs::remove(file.path());
            }
        }
    }

    vector<Frame>& create_frames_from_video(EnvArguments& arguments) {
        vector<Frame>& frames = _frames;

        TerminalRestorer restorer;

        uint32_t* time = arguments.time;

        uint16_t inc_ms = 1000 / arguments.fps;
        uint32_t video_end_time = get_video_duration(arguments.path);
        uint32_t video_start_time = 0;
        video_end_time = (time[1] * 1000 > video_end_time)?video_end_time:(time[1] * 1000);
        video_end_time -= video_end_time % inc_ms;
        video_start_time = (time[0] * 1000 > video_end_time)?(video_end_time - 2000):(time[0] * 1000);

        Timestamp ts(video_start_time);
        ThreadPool pool(std::thread::hardware_concurrency());

        queue<future<Frame>> futures;

        frames.clear();

        if (check_path(TEMP_PATH)) {
            cleanup();
        } else {
            fs::create_directory(TEMP_PATH);
        }

        while (ts.time < video_end_time) {
            futures.push(pool.submit(pick_frame, arguments.path, ts, arguments.size));

            ts.inc(inc_ms);
        }

        while (!futures.empty()) {
            frames.push_back(futures.front().get());
            futures.pop();
        }

        //cleanup();

        return frames;
    }

    vector<Frame>& create_frames(EnvArguments& arguments) {
        vector<Frame>& frames = _frames;

        if (arguments.loaded_from_file) {
            frames = storage_ns::load_file(arguments.name, arguments);
            return frames;
        } else {
            return create_frames_from_video(arguments);
        }
    }
}