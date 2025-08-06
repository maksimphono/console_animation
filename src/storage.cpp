#include "include/storage.hpp"

namespace storage_ns {
    class StorageWriter : public ofstream {
    public:
        StorageWriter(string name) : ofstream(create_path(name)) {
            this->open(create_path(name), fstream::out);
        }
        ~StorageWriter() {
            this->close();
        }
        uint32_t write_metadata(EnvArguments& arguments) {
            // returns number of items written
            cout << this->is_open();
            return 0;
            //if (!this->is_open()) return 0;
            //this->seekp(0);
            //StorageWriter& _this = *this;

            //_this << "INPUT_PATH=" << arguments.path << ";";
            //_this << "FPS=" << arguments.fps;
        }
        uint32_t write_frames(vector<Frame>& frames) {
            // returns number of frames written
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
        //string n = "qwert";
        //cout << create_path(n);
        StorageWriter writer(name);

        writer.write_metadata(arguments);
        //file.write_frames(frames);
    }
}