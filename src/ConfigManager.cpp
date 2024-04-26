#include "../include/SerialComm.h"
#include <string>
//#include "../include/ConfigManager.h"

using std::string;

class ConfigManager {
private:
    SerialComm serial;
    string time_lag;
    string wifi_network;
    string wifi_passwd;
    string sudo_package = "sudo";
    string ntp_package = "ntp";
    string LOGIN = "login:";
    string PASSWD = "Password:";
    string BASH = "~#";
    const char interrupt_char = 0x03;

public:
    ConfigManager(const string device, string wifi_network, string wifi_passwd) 
        : serial(device.c_str()), wifi_network(wifi_network), wifi_passwd(wifi_passwd) {
        
    }

    bool login() {
        /*
        * Logs in to Zero3 over serial using default root/password values.
        * 
        * :return: true if login successful, else false
        */
        std::cout << "start of login" << std::endl;
        for (int attempt = 0; attempt < 3; attempt++) {
            std::cout << "login attempt " << attempt << std::endl;
            if (get_prompt(LOGIN, 100000, true, 10000)) {
                serial.send_command("root\n");
            } else {
                continue;
            }
            std::cout << "start of passwd" << std::endl;
            if (get_prompt(PASSWD, 100000, false, 0)) {
                serial.send_command("temp\n");
            } else {
                continue;
            }
            std::cout << "start of get bash" << std::endl;
            if (get_prompt(BASH, 100000, false, 0)) {
                return true;
            }
        }
        return false;
    }

    bool configure_wifi() {
        /*
        * Uses the network name and password to connect to wifi.
        *
        * :return: true if successful else false 
        */
        // Format command string
        string connect_cmd = "nmcli device wifi connect " + wifi_network + " password " + wifi_passwd + "\n";
        string ping_cmd = "ping -c2 8.8.8.8\n";
        string response;
        
        if (!get_prompt(BASH, 1000000, true, 1000)) {
            std::cout << "Unable to configure wifi - no bash prompt found." << std::endl;
            return false;
        }
        // attempt to connect
        serial.send_command(connect_cmd.c_str());
        if (!get_prompt("successfully activated", 1000000, false, 0)) {
            return false;
        } 
        // nmcli connection successful - get bash prompt
        if (!get_prompt(BASH, 100000, false, 0)) {
            std::cout << "Unable to get bash prompt." << std::endl;
            return false;
        }
        std::cout << "nmcli command complete - bash prompt found" << std::endl;
        
        // connection presumed successful - get prompt and test with ping
        if (!get_prompt(BASH, 1000000, true, 1000)) {
            std::cout << "Unable to get bash prompt for ping test." << std::endl;
            return false;
        }
        std::cout << "Sending ping command" << std::endl;
        serial.send_command(ping_cmd.c_str());

        if (get_prompt("2 packets transmitted", 1000000, false, 0)) {
            return true;
        }

        return false;
    }

    bool run_apt_update() {

    }

    bool run_apt_upgrade() {

    }

    bool run_apt_install(string package) {

    }

    bool get_clock_discrepancy() {

    }

    bool fix_clock() {

    }

    bool create_user() {

    }
private:
    string get_output() {
        /*
        * 
        * 
        * 
        */
        string response;
        
    }

    bool get_prompt(string prompt_type, int max_attempts, bool send_interrupt, int int_interval) {
        /*
        * Reads serial output until the prompt is found or max attempts is reached.
        * 
        * :param prompt_type: a string representing the desired symbol/string
        * :param max_attempts: an int representing the max number of attempts for getting prompt
        * :param send_interrupt: a bool, true if interrupt is to be sent to trigger prompt, else false
        * :return: true if prompt is found in serial output, else false
        */
        string response;

        for (int attempts = 0; attempts < max_attempts; attempts++) {
            response.append(serial.read_response());
            if (response.find(prompt_type) != string::npos) {
                return true;
            } else if (send_interrupt && attempts % int_interval == 0) {
                //std::cout << response << std::endl;
                std::cout << "send interrupt" << std::endl;
                serial.send_command(&interrupt_char);
                //std::cout << "send newline" << std::endl;
                serial.send_command("\n");
            }
            usleep(10);
        }
        return false;
    }




};