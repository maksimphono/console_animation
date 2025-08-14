#pragma once

#include <termios.h>
#include <unistd.h>

class TerminalRestorer {
private:
    struct termios original_termios;
public:
    TerminalRestorer() {
        tcgetattr(STDIN_FILENO, &original_termios);
    }
    ~TerminalRestorer() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    }
};