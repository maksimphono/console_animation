#include <iostream>
#include "utils/exec_command.cpp"

using namespace std;

int main() {
    CommandExecutor ex("ls -la /home");

    cout << ex.command[0] << endl;
    return 0;
}