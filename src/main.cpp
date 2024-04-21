#include <iostream>
#include "../include/SerialComm.h"
//#include "../include/ConfigManager.h"
#include "ConfigManager.cpp"

using std::string;

int main()
{
    ConfigManager test("/dev/tty.usbserial-A10KDLKN", "test", "test");

    bool res = test.login();
    std::cout << "Login Result: " << res << std::endl;
    
    return 0;

}
