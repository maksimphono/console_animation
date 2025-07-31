#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

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