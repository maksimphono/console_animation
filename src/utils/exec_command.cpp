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
        this->command = new char*[this->command_length + 1];

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
                strcpy(this->command[i], tokens[i].c_str());
            }
            printf("%u ", this->cmd_is_template[i]);
        }
        this->command[this->command_length] = nullptr;
        this->buffer_size = buffer_size;
    }

public:
    CommandExecutor(string command_template, uint32_t buffer_size = 256) {
        this->create_command_array(command_template);
        //cout << this->command[0] << endl;
    }
    ~CommandExecutor() {
        for (uint8_t i = 0; i < this->command_length; i++) {
            if (this->cmd_is_template[i])
                delete[] this->command_template[i];
            delete[] this->command[i];
        }
        delete[] this->command_template;
        delete[] this->command;
    }

    template <typename T=string>
    T exec_command(int in_pipe = -9, ...) {
        va_list args;
        int size = 0;

        va_start(args, in_pipe);

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

        int stdin_pipe[2], stdout_pipe[2];
        
        if (in_pipe == -9)
            pipe(stdin_pipe);
        else
            stdin_pipe[0] = in_pipe;

        pipe(stdout_pipe);

        pid_t pid = fork();
        if (pid == 0) {
            dup2(stdin_pipe[0], STDIN_FILENO);
            dup2(stdout_pipe[1], STDOUT_FILENO);

            perror(this->command[0]);
            close(stdin_pipe[1]);
            close(stdout_pipe[0]);

            //write(STDOUT_FILENO, "qwert\0", 6);

            execvp(this->command[0], this->command);
            exit(0);
        }

        close(stdin_pipe[0]);
        close(stdout_pipe[1]);

        close(stdin_pipe[1]);

        if constexpr (is_same_v<T, string>) {
            // if string output is required
            string result = "";

            char* buffer = new char[this->buffer_size];
            memset(buffer, 0, this->buffer_size);

            while (read(stdout_pipe[0], buffer, this->buffer_size) != 0) {
                result += buffer;
                memset(buffer, 0, this->buffer_size);
            }

            close(stdout_pipe[0]);
            delete[] buffer;
            return result;
        }
        else if constexpr (is_same_v<T, int>) {
            // if pipe output is required
            return stdout_pipe[0];
        } else {
            
        }
            
    }
};

template <typename T>
static T exec_command(string cmd, vector<char> bytes_in = {}, uint32_t bts = 1024) {
    T a;
    return a;
}