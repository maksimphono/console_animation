#include "include/render.hpp"

using namespace std;

namespace render_ns {
    void render(vector<frames_ns::Frame> frames, uint8_t size[2], uint8_t fps) {
        initscr();
        noecho(); // turn off automatic echoing of typed characters
        cbreak(); // make characters available to the program as soon as they are typed
        keypad(stdscr, TRUE); // enable the interpretation of special keys
        nodelay(stdscr, TRUE); // make getch() a non-blocking call
        nodelay(stdscr, TRUE);
        curs_set(0);
        clear();

        int ch;
        uint32_t i = 0;

        while (true) {
            ch = getch();

            if (ch == 'q' || ch == 'Q') {
                // If 'q' or 'Q' is pressed, break out of the loop to quit
                break;
            }

            //clear(); // Clear the screen before redrawing
            mvprintw(0, 0, frames[i].body.c_str());
            //move(current_line_idx + 2, current_col_idx);

            refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(250)); // sleep for 500 milliseconds
            i = ++i % frames.size();
        }

        endwin(); // cleanup
    }
}