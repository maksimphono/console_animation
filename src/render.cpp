#include "include/storage.hpp"
#include "include/render.hpp"
//#include "utils/TerminalRestorer.cpp"

using namespace std;

namespace render_ns {
    typedef enum {BREAK, NORMAL, FILE_SAVING} State;

    void sleep(uint32_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms)); // sleep for 500 milliseconds
    }

    void process_file_saving(vector<Frame>& frames, EnvArguments& arguments) {
        string name = "";
        string answer = "";

        auto handle_resize = [](int signum) {};

        auto save_file = [&name, &frames, &arguments]() {
            if (storage_ns::save_file(name, frames, arguments) == frames.size()) {
                cout << format("File '{0}' saved successfully!", name) << endl;
            } else {
                throw StorageSaveException();
            }
        };

        signal(SIGWINCH, handle_resize);
        for (;;) {
            cout << "Input a name to save the file:" << endl;
            cin >> name;

            if (name == "") {
                cout << "Sorry, can't save smpty string, input at leat one symbol!" << endl;
            } else if (storage_ns::check_exsistance(name)) {

                cout << format("File with name '{0}' already exist, do you want to override it?\n[Y/n]: ", name);
                cin >> answer;

                if (answer == "y" || answer == "Y") {
                    save_file();
                    break;
                } else {
                    cout << format("File '{0}' wasn't saved.", name) << endl;
                }

            } else {
                save_file();
                break;
            }
        }
    }

    void render(vector<frames_ns::Frame> frames, EnvArguments& arguments) {
        State state = NORMAL;
        uint8_t fps = arguments.fps;

        auto process_key = [&state](char ch) {
            switch(ch) {
                case 'q':
                case 'Q':
                    state = BREAK;
                    return;
                case 's':
                    state =  FILE_SAVING;
                    return;
                default:
                    state = NORMAL;
                    return;
            }
        };

        initscr();
        noecho(); // turn off automatic echoing of typed characters
        cbreak(); // make characters available to the program as soon as they are typed
        keypad(stdscr, TRUE); // enable the interpretation of special keys
        nodelay(stdscr, TRUE); // make getch() a non-blocking call
        nodelay(stdscr, TRUE);
        curs_set(0);
        clear();

        char ch;
        uint32_t i = 0;

        while (true) {
            ch = getch();

            process_key(ch);

            if (state != NORMAL) {
                break;
            }

            mvprintw(0, 0, frames[i].body.c_str());

            refresh();
            sleep(1000 / fps);
            i = ++i % frames.size();
        }

        endwin(); // cleanup

        if (state == FILE_SAVING) {
            process_file_saving(frames, arguments);
        }
    }
}