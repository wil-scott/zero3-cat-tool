#include <iostream>
#include "../include/SerialComm.h"

using std::string;

int main()
{
    SerialComm serial("/dev/tty.usbserial-A10KDLKN");
    string LOGIN = ":";
    string BASH = "~#";
    string response = "";

    serial.send_command("\n");
    while (response.find(LOGIN) == string::npos) {
        response.append(serial.read_response());
    }
    response.clear();

    serial.send_command("root\n");
    while (response.find(LOGIN) == string::npos) {
        response.append(serial.read_response());
    }
    response.clear();

    serial.send_command("temp\n");
    while (response.find(BASH) == string::npos) {
        response.append(serial.read_response());
    }
    response.clear();

    serial.send_command("\n");
    while (response.find(BASH) == string::npos) {
        response.append(serial.read_response());
    }
    response.clear();
    
    serial.send_command("whoami\n");
    while (response.find(BASH) == string::npos) {
        response.append(serial.read_response());
    }
    std::cout << "Response: " << response << std::endl;

    return 0;

}
