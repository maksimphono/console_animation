#include "include/storage.hpp"
#include "include/render.hpp"

using namespace std;

namespace render_ns {
    typedef enum {BREAK, NORMAL, FILE_SAVING} State;

    void sleep(uint32_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms)); // sleep for 500 milliseconds
    }

    void process_file_saving(vector<Frame> frames, EnvArguments& arguments) {
        string name = "";
        string answer = "";
        cout << "Input a name to save the file:" << endl;

        for (;;) {
            cin >> name;

            if (name == "") {
                cout << "Sorry, did you pressed ENTER by accident? Input at leat one symbol:" << endl;
            } else if (storage_ns::check_exsistance(name)) {
                cout << format("File with name '{0}' already exist, do you want to override it?\n[Y/n]: ", name);
                cin >> answer;
                if (answer == "y" || answer == "Y") {
                    if (storage_ns::save_file(name, frames, arguments) == frames.size()) {
                        cout << format("File '{0}' saved successfully!", name) << endl;
                        break;
                    } else {
                        // throw error
                    }
                } else {
                    cout << format("File '{0}' wasn't saved.", name) << endl;
                    cout << "Input a name to save the file:" << endl;
                }
            } else {
                if (storage_ns::save_file(name, frames, arguments) == frames.size()) {
                    cout << format("File '{0}' saved successfully!", name) << endl;
                    break;
                } else {
                    // throw error
                }
            }
        }
    }

    void process_key(char ch, State& state) {
        switch(ch) {
            case 'q':
            case 'Q':
                state = BREAK;
                return;
            case 's':
                // save file
                state =  FILE_SAVING;
                return;
            default:
                state = NORMAL;
                return;
        }
    }

    void render(vector<frames_ns::Frame> frames, EnvArguments& arguments) {
        State state = NORMAL;
        uint8_t fps = arguments.fps;
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

            process_key(ch, state);

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