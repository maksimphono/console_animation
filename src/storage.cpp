#include "include/storage.hpp"

namespace storage_ns {
    bool check_existance(string& name) {
        return fs::exists(storage_path + "/" +  name);
    }

    vector<string> list_all_files() {
        vector<string> files;

        for (auto file = fs::directory_iterator(storage_path); file != fs::directory_iterator(); file++) {
            files.push_back(file->path().filename());
        }

        return files;
    }
}