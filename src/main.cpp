#include <iostream>
#include "utils/exec_command.cpp"

using namespace std;

int main() {
    CommandExecutor ls("ls -%s %s", 4096);
    CommandExecutor wc("wc -%s", 4096);

    int p = ls.exec_command(-9, "la", "/home");
    p = wc.exec_command(p, "l");

    char* buffer = new char[256];
    memset(buffer, 0, 256);

    while (read(p, buffer, 256) != 0) {
        write(1, buffer, 256);
        memset(buffer, 0, 256);
    }

    close(p);

    //cout << out << endl;
    return 0;
}