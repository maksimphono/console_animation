#include <iostream>
#include "utils/exec_command.cpp"

using namespace std;

int main() {
    CommandExecutor ex("ls -%c /home %u:%s");

    ex.exec_command({}, 'l', 87, "qwerty");

    cout << ex.command_template[0] << endl;
    return 0;
}