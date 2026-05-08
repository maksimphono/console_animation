#include <iostream>
#include "utils/exec_command.cpp"

using namespace std;
using namespace exec_command_ns;

typedef int Pipe;

class C{
public:
    string s;
    C(string& s) {
        this->s = s;
    }
};

int main() {
    CommandExecutor ff("ffmpeg -loglevel -8 -ss %d:%d:%d.000 -i %s -frames:v 1 -f image2pipe -", 4096);
    CommandExecutor jp("jp2a - --size=%dx%d", 4096);

    //cout << ff.command_templates.at(ff.command_templates.size() - 1).tmplt << endl;

    //string s = ff.exec<string>(-9, 0, 0, 2, "/home/Rick.mp4");
    //int p = ff.exec<int>(-9, 0, 0, 2, "/home/Rick.mp4");
    int p = ff.exec<int>(-9, 0, 0, 2, "/home/build/Rick Astle Never Gonna give you up (Full version).mp4");
    string s = jp.exec<string>(p, 70, 30);

    cout << s;
    return 0;
}