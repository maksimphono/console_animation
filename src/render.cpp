#include "include/render.hpp"

using namespace std;

namespace render_ns {
    void sleep(uint32_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms)); // sleep for 500 milliseconds
    }

    void render(vector<frames_ns::Frame> frames, uint8_t fps) {
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
                break;
            }

            mvprintw(0, 0, frames[i].body.c_str());

            refresh();
            sleep(1000 / fps);
            i = ++i % frames.size();
        }

        endwin(); // cleanup
    }
}