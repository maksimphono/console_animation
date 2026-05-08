#include <string>
#include <cstdint>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdarg.h>
//#include <cstdio>

//#include "include/exception.hpp"

namespace exec_command_ns{
    using namespace std;
    
    typedef struct {
        uint8_t idx;
        string tmplt;
    } idx_tmplt_pair_t;

    class CommandExecutor {
    public:
        vector<idx_tmplt_pair_t> command_templates; // command, that will be executed
        uint8_t command_length = 0;
        char** command = NULL;
        uint32_t buffer_size;

        void create_command_array(string command_templates) {
            stringstream ss(command_templates);
            vector<string> tokens;
            string token;
            bool cmd_is_template = false;

            while(ss >> token) {
                tokens.push_back(token);
            }

            this->command_length = tokens.size();
            this->command = new char*[this->command_length + 1];
            this->command_templates = {};

            for (uint8_t i = 0; i < this->command_length; i++) {
                this->command[i] = new char[tokens[i].length() + 1];
                strcpy(this->command[i], tokens[i].c_str());

                for (auto& sym : tokens[i]) {
                    if (sym == '%') {
                        this->command_templates.push_back({i, tokens[i]});
                        break;
                    }
                }
            }
            this->command[this->command_length] = nullptr; // null-terminated (for execvp)
        }

    public:
        CommandExecutor(string command_templates, uint32_t buffer_size = 256) {
            this->buffer_size = buffer_size;
            this->create_command_array(command_templates);
        }
        ~CommandExecutor() {
            for (uint8_t i = 0; i < this->command_length; i++) {
                delete[] this->command[i];
            }
            //cout << endl;
            delete[] this->command;
        }

        template <typename T=string>
        T exec(int in_pipe = -9, ...) {
            va_list args, args_cpy;
            int size = 0;

            va_start(args, in_pipe);

            for (auto& pair : this->command_templates) {
                va_copy(args_cpy, args);
                int size = vsnprintf(nullptr, 0, pair.tmplt.c_str(), args); // count number of symbols if successfully written

                // can't figure out how to reallocate space here, so just deleting and allocating again
                if (size > strlen(this->command[pair.idx])) {
                    char* tmp = new char[size + 1];
                    memset(tmp, 0, size + 1);
                    delete[] this->command[pair.idx];
                    this->command[pair.idx] = tmp;
                }

                vsprintf(this->command[pair.idx], pair.tmplt.c_str(), args_cpy);
            }
            va_end(args_cpy);

            va_end(args);

            int stdin_pipe[2], stdout_pipe[2];

            if (in_pipe == -9)
                pipe(stdin_pipe);
            else
                stdin_pipe[0] = in_pipe;

            pipe(stdout_pipe);

            for (int i = 0; i < this->command_length; i++)
                cout << this->command[i] << " ";
            cout << endl;

            pid_t pid = fork();
            if (pid == 0) {
                dup2(stdin_pipe[0], STDIN_FILENO);
                dup2(stdout_pipe[1], STDOUT_FILENO);

                perror(this->command[0]);
                close(stdin_pipe[1]);
                close(stdout_pipe[0]);

                execvp(this->command[0], this->command);
                exit(0);
            }

            close(stdin_pipe[0]);
            close(stdout_pipe[1]);

            close(stdin_pipe[1]);

            if constexpr (is_same_v<T, int>) {
                // if pipe output is required
                return stdout_pipe[0];
            }
            else {
                string result = "";

                char* buffer = new char[this->buffer_size];
                memset(buffer, 0, this->buffer_size);

                while (read(stdout_pipe[0], buffer, this->buffer_size) != 0) {
                    cout << "qwerrtttt" << endl;
                    result += buffer;
                    memset(buffer, 0, this->buffer_size);
                }

                close(stdout_pipe[0]);
                delete[] buffer;
                if constexpr (is_same_v<T, string>)
                    // if string output is required
                    return result;
                else
                    // try to construct instance of that class from a string
                    return T(result);
            }

        }
    };

}

template <typename T>
static T exec_command(std::string cmd, std::vector<char> bytes_in = {}, uint32_t bts = 1024) {
    T a;
    return a;
}