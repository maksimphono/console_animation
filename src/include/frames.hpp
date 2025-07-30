#include <iostream>

namespace frames_ns {
    class Frame {
    protected:
        char* body; // flat array, that contains all symbols of the frame
    public:
        Frame(string path){
        }
    }

    void create_frames_from_video(string path);
}