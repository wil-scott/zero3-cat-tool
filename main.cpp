#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include "include/SerialComm.h"
#include "include/ConfigManager.h"

using std::string;

void get_device_details(std::map<string, string>& device_details, const string& key) {
    string input;
    while (true) {
        std::cout << "Enter " << key << ": ";
        if (std::getline(std::cin, input)) {
            if (!input.empty()) {
                device_details[key] = input;
                break;

            } else {
                std::cout << key << " cannot be empty - please enter a value. \n" << std::endl;
            }
        } else {
            std::cout << "Error reading input. Please try again.\n" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        }
    }
}

int main()
{
    // Get device data from user
    std::map<string, string> device_details;
    get_device_details(device_details, "tty_dev_id");
    get_device_details(device_details, "network_id");
    get_device_details(device_details, "network_passwd");
    get_device_details(device_details, "root_user");
    get_device_details(device_details, "root_pass");
    
    // Set up ConfigManager object
    ConfigManager test(device_details["tty_dev_id"], 
                    device_details["network_id"],
                    device_details["network_passwd"],
                    device_details["root_user"],
                    device_details["root_pass"]
                    );
    
    // Run through config loop
    
    bool res = test.login();
    std::cout << "Login Result: " << res << std::endl;
    bool res_2 = test.configure_wifi();
    std::cout << "Network result: " << res_2 << std::endl;
    bool res_3 = test.run_apt_update_for_clock_fix();
    std::cout << "apt update for clock: " << res_3 << std::endl;
    bool res_4 = test.fix_clock();
    std::cout << "clock fixed: " << res_4 << std::endl;
    return 0;

}
