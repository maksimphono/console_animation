#include <iostream>

using namespace std;

namespace frames_ns {
    class Frame {
    protected:
        char* body; // flat array, that contains all symbols of the frame
        uint8_t size[2] = {};
    public:
        Frame(string& chars, uint8_t size[2]){
            this->body = new char[size[0] * size[1]];
            this->size[0] = size[0];
            this->size[1] = size[1];
        }
        void put(char* screen) {
            memcpy(this->body, screen, sizeof(char));
        }
    };

    void create_frames_from_video(string& path);
}