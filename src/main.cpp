#include <iostream>
#include "utils/exec_command.cpp"

using namespace std;

typedef int Pipe;

int main() {
    CommandExecutor ls("ls -%s %s", 4096);
    CommandExecutor wc("wc -%s", 4096);

    int p = ls.exec<Pipe>(-9, "la", "/home");
    string out = wc.exec<string>(p, "l");

    cout << out;
    return 0;
}