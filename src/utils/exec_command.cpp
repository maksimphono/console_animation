#include <iostream>
#include <string>
#include <cstdint>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdarg.h>
//#include <cstdio>

using namespace std;

class CommandExecutor {
public:
    char** command_template = NULL; // command, that will be executed
    uint8_t command_length = 0;
    char** command = NULL;
    uint32_t buffer_size = 256;
    bool* cmd_is_template = nullptr;

    void create_command_array(string command_template) {
        stringstream ss(command_template);
        vector<string> tokens;
        string token;

        while(ss >> token) {
            tokens.push_back(token);
        }

        this->command_length = tokens.size();
        this->command_template = new char*[this->command_length];
        this->cmd_is_template = new bool[this->command_length];
        this->command = new char*[this->command_length];

        for (uint8_t i = 0; i < this->command_length; i++) {
            this->cmd_is_template[i] = false;

            for (auto& sym : tokens[i]) {
                if (sym == '%') {
                    this->cmd_is_template[i] = true;
                    break;
                }
            }
            if (this->cmd_is_template[i]) {
                this->command_template[i] = new char[tokens[i].length() + 1];
                this->command[i] = new char[tokens[i].length() + 1]; 
                strcpy(this->command_template[i], tokens[i].c_str());
            } else {
                this->command_template[i] = nullptr;
                this->command[i] = new char[tokens[i].length() + 1];
                cout << "qwr" << endl;
                sprintf(this->command[i], tokens[i].c_str());
            }
            printf("%u ", this->cmd_is_template[i]);
            //sprintf(this->command[i], "%s", );
        }
    }

public:
    CommandExecutor(string command_template, uint32_t buffer_size = 256) {
        this->create_command_array(command_template);
        //cout << this->command[0] << endl;
    }
    ~CommandExecutor() {
        for (uint8_t i = 0; i < this->command_length; i++) {
            if (this->cmd_is_template)
                delete[] this->command_template[i];
            delete[] this->command[i];
        }
        delete[] this->command_template;
        delete[] this->command;
    }

    template <typename T=string>
    T exec_command(vector<char> bytes_in, ...) {
        T result;
        va_list args;
        int size = 0;

        va_start(args, bytes_in);

        for (uint8_t i = 0; i < this->command_length; i++) {
            if (this->cmd_is_template[i]) {
                int size = snprintf(nullptr, 0, this->command_template[i], args); // count number of symbols if successfully written
                realloc(this->command[i], size + 1);
                vsprintf(this->command[i], this->command_template[i], args);
            }
        }

        for (int i = 0; i < this->command_length; i++) {
            cout << this->command[i] << " ";
        }

        cout << endl;

        va_end(args);
        /*
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

            execvp(command[0], command, NULL);
            exit(1);
        }

        close(stdin_pipe[0]);
        close(stdout_pipe[1]);

        if (!bytes_in.empty()) {
            write(stdin_pipe[1], bytes_in.data(), buffer_size * sizeof(bytes_in[0]));
        }

        close(stdin_pipe[1]);
        if (!pipe) {
            std::cerr << "Error: popen() failed to execute command: " << command << std::endl;
            // TODO: throw an error
            return {};
        }
        while (read(stdout_pipe[0], buffer, buffer_size * sizeof(buffer)) != 0) {
            result.insert(result.end(), buffer, buffer + buffer_size);
        }

        close(stdout_pipe[0]);
        
        delete[] buffer;
        */
        return result;
    }
};

template <typename T>
static T exec_command(string cmd, vector<char> bytes_in = {}, uint32_t bts = 1024) {
    T a;
    return a;
}