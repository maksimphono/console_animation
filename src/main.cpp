#include <iostream>
#include <string>    // For std::string
#include <ncurses.h> // Include the ncurses library
#include <vector>    // For std::vector (to store lines of text)
#include <chrono>    // For std::chrono::seconds, std::chrono::milliseconds
#include <thread>
#include <cstdint>

#include "cli_arguments.cpp"

using namespace std;

void mainloop() {
    initscr();
    noecho(); // turn off automatic echoing of typed characters
    cbreak(); // make characters available to the program as soon as they are typed
    keypad(stdscr, TRUE); // enable the interpretation of special keys
    nodelay(stdscr, TRUE); // make getch() a non-blocking call
    clear();

    mvprintw(0, 0, "First line");
    refresh();

    int ch;
    uint32_t i = 0;

    while (true) {
        ch = getch();

        if (ch == 'q' || ch == 'Q') {
            // If 'q' or 'Q' is pressed, break out of the loop to quit
            break;
        }
        
        clear(); // Clear the screen before redrawing
        //move(current_line_idx + 2, current_col_idx);

        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // sleep for 500 milliseconds
    }

    endwin(); // cleanup
}

int main(int arg_num, const char** vargs) {
    try {
        cli_arguments_ns::get_cli_arguments(arg_num, vargs);
        //mainloop();
    } catch (const exception& exp) {
        cerr << exp.what() << endl;
        return 1;
    }

    return 0;
}