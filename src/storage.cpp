#include "include/storage.hpp"
#include "include/frames.hpp"
#include "include/env_arguments.hpp"

namespace storage_ns {

    template <typename T = string>
    uint32_t StorageWriter::write_metadata_entry(const char* name, T value) {
        *this << name << "=" << value << ";";
        return 1;
    }

    uint32_t StorageWriter::write_metadata(EnvArguments& arguments) {
        // returns number of items written
        uint32_t n = 0;
        if (!this->is_open()) return 0;

        this->seekp(0);
        n += this->write_metadata_entry<int>("FPS", (int)arguments.fps);
        n += this->write_metadata_entry("SIZE", format("{0}x{1}", arguments.size[0], arguments.size[1]));
        n += this->write_metadata_entry("TIME", format("{0}-{1}", arguments.time[0], arguments.time[1]));

        *this << endl;
        return n;
    }

    uint32_t StorageWriter::write_frames(vector<Frame>& frames) {
        // returns number of frames written
        uint32_t n = 0;

        for (auto& frame : frames) {
            *this << frame.body;
            n++;
        }
        return n;
    }

    string StorageReader::read_metadata_line() {
        uint32_t metadata_length = 0;
        char* metadata_line = nullptr;

        for (char ch = '\0'; ch != '\n'; ch = this->get()) metadata_length++;
        metadata_line = new char[metadata_length];

        this->seekg(0);
        this->getline(metadata_line, metadata_length);

        string metadata_str(metadata_line);
        delete[] metadata_line;

        return metadata_str;
    }

    EnvArguments& StorageReader::read_metadata(EnvArguments& arguments) {
        if (!this->is_open()) return arguments;

        string metadata_str = this->read_metadata_line();
        regex pattern("[^;]+=[^;]+");
        map<string, string> raw_arguments;

        auto words_begin = std::sregex_iterator(metadata_str.begin(), metadata_str.end(), pattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            string str(i->str());
            raw_arguments[str.substr(0, str.find("="))] = str.substr(str.find("=") + 1);
        }

        arguments.set_fps(raw_arguments["FPS"]);
        arguments.set_size(raw_arguments["SIZE"]);
        arguments.set_time(raw_arguments["TIME"]);

        return arguments;
    }

    vector<Frame> StorageReader::read_frames(EnvArguments& arguments) {
        vector<Frame> frames;
        uint32_t frame_length = (arguments.size[0] + 1) * arguments.size[1];

        this->seekg(0);
        this->ignore(0xffffffff, '\n');
        char* buffer = new char[frame_length + 1];
        string frame_string = "";

        while (!this->eof()) {
            this->read(buffer, frame_length);
            buffer[frame_length] = '\0';

            frame_string = string(buffer);
            frames.push_back(Frame(frame_string, arguments.size));
        }

        delete[] buffer;
        return frames;
    }

    fs::path create_path(string& name) {
        return fs::path(storage_path + "/" + name);
    }

    bool check_exsistance(string& name) {
        return fs::exists(create_path(name));
    }

    void list_all_files(ostream& stream, string separator) {
        for (auto file = fs::directory_iterator(storage_path); file != fs::directory_iterator(); file++) {
            stream << file->path().filename() << separator;
        }
    }

    bool delete_file(string name) {
        if (name == "" || check_exsistance(name) == false) {
            THROW_FILE_NOT_FOUND_EXP(name);
        }

        fs::remove(create_path(name));

        return !check_exsistance(name);
    }

    uint32_t save_file(string name, vector<Frame>& frames, EnvArguments& arguments) {
        StorageWriter writer(name);

        writer.write_metadata(arguments);
        return writer.write_frames(frames);
    }

    vector<Frame> load_file(string name, EnvArguments& arguments) {
        if (name == "" || check_exsistance(name) == false) {
            THROW_FILE_NOT_FOUND_EXP(name);
        }

        StorageReader reader(name);

        reader.read_metadata(arguments);
        vector<Frame> frames = reader.read_frames(arguments);

        return frames;
    }
}