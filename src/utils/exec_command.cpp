#include <iostream>
#include <string>
#include <cstdint>
#include <unistd.h>
//#include <cstdio>

using namespace std;

template <typename T>
static T exec_command(string command, vector<char> bytes_in = {}, uint32_t buffer_size = 256) {
    T result;
    char* buffer = new char[buffer_size];

    int stdin_pipe[2], stdout_pipe[2];
    pipe(stdin_pipe);
    pipe(stdout_pipe);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(stdin_pipe[0], STDIN_FILENO);
        dup2(stdout_pipe[1], STDOUT_FILENO);

        perror("Child: ");
        perror(command.c_str());
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);

        execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
        exit(1);
    }

    close(stdin_pipe[0]);
    close(stdout_pipe[1]);

    if (!bytes_in.empty()) {
        write(stdin_pipe[1], bytes_in.data(), buffer_size * sizeof(bytes_in[0]));
    }

    close(stdin_pipe[1]);
    /*
    if (!pipe) {
        std::cerr << "Error: popen() failed to execute command: " << command << std::endl;
        // TODO: throw an error
        return {};
    }
    */
    while (read(stdout_pipe[0], buffer, buffer_size * sizeof(buffer)) != 0) {
        result.insert(result.end(), buffer, buffer + buffer_size);
    }

    close(stdout_pipe[0]);
    /*
    int status = pclose(pipe);
    if (status == -1) {
        cerr << "Can't close pipe" << endl;
        // TODO: throw an error
    }
    */
    delete[] buffer;
    return result;
}