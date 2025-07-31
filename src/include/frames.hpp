#include <iostream>

using namespace std;

namespace frames_ns {
    class Frame {
    public:
        uint16_t len = 0;
        char* body = nullptr; // flat array, that contains all symbols of the frame
        uint8_t size[2] = {};
    public:
        Frame(string& chars, uint8_t size[2]){
            this->len = (size[0] + 1) * size[1];
            this->body = new char[this->len];
            memcpy(this->body, chars.c_str(), this->len);
            this->size[0] = size[0];
            this->size[1] = size[1];
        }
        void put(char* screen) {
            memcpy(this->body, screen, sizeof(char));
        }
    };

    void create_frames_from_video(string& path);
}