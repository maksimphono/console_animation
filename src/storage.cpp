#include "include/storage.hpp"

namespace storage_ns {
    class StorageWriter : public ofstream {
    public:
        StorageWriter(string name) : ofstream(create_path(name)) {
            //this->open(create_path(name), fstream::out);
        }
        ~StorageWriter() {
            this->close();
        }
        template <typename T = string>
        uint32_t write_entry(const char* name, T value) {
            *this << name << "=" << value << ";";
            return 1;
        }

        uint32_t write_metadata(EnvArguments& arguments) {
            // returns number of items written
            uint32_t n = 0;
            if (!this->is_open()) return 0;
            this->seekp(0);

            n += this->write_entry("INPUT_PATH", arguments.path);
            n += this->write_entry<int>("FPS", (int)arguments.fps);
            n += this->write_entry("SIZE", format("{0}x{1}", arguments.size[0], arguments.size[1]));
            n += this->write_entry("TIME", format("{0}-{1}", arguments.time[0], arguments.time[1]));

            *this << endl;

            return n;
        }
        uint32_t write_frames(vector<Frame>& frames) {
            // returns number of frames written
            uint32_t n = 0;

            for (auto& frame : frames) {
                *this << frame.body;
                n++;
            }

            return n;
        }
    };

    fs::path create_path(string& name) {
        return fs::path(storage_path + "/" + name);
    }

    bool check_existance(string& name) {
        return fs::exists(create_path(name));
    }

    vector<string> list_all_files() {
        vector<string> files;

        for (auto file = fs::directory_iterator(storage_path); file != fs::directory_iterator(); file++) {
            files.push_back(file->path().filename());
        }

        return files;
    }

    void save_file(string name, vector<Frame>& frames, EnvArguments& arguments) {
        StorageWriter writer(name);

        writer.write_metadata(arguments);
        writer.write_frames(frames);
    }
}