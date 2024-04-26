#include <iostream>
#include "../include/SerialComm.h"
//#include "../include/ConfigManager.h"
#include "ConfigManager.cpp"

using std::string;

int main(int argc, char* argv[])
{
    string network_id = argv[1];
    string passwd = argv[2];
    ConfigManager test("/dev/tty.usbserial-A10KDLKN", network_id, passwd);

    bool res = test.login();
    std::cout << "Login Result: " << res << std::endl;
    bool res_2 = test.configure_wifi();
    std::cout << "Network result: " << res_2 << std::endl;
    
    return 0;

}
