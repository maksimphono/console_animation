#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

using namespace std;

bool check_path(string path) {
    if (!filesystem::exists(path)) {
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