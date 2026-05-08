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
    CommandExecutor ls("ls -%s %s", 4096);
    CommandExecutor wc("wc -%s", 4096);



    int p = ls.exec<Pipe>(-9, "la", "/home/build");
    C c = wc.exec<C>(p, "l");

    cout << c.s;
    return 0;
}