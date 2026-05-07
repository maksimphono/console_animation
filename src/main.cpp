#include <iostream>
#include "utils/exec_command.cpp"

using namespace std;

int main() {
    CommandExecutor ex("ls -%s /home", 4096);

    string out = ex.exec_command({}, "la");

    //cout << out << endl;
    return 0;
}