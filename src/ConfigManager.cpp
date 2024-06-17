#include "../include/ConfigManager.h"

ConfigManager::ConfigManager(const string device, string wifi_network, string wifi_passwd, string root_user, string root_pass)
    : serial(device.c_str()), wifi_network(wifi_network), wifi_passwd(wifi_passwd), root_user(root_user), root_pass(root_pass) {
}

void ConfigManager::clear_response() {
    response.clear();
}

string ConfigManager::extractTimeDiscrepancy(const string& response) {
    std::regex timeReg("\\(invalid for another (\\d+)d (\\d+)h (\\d+)min (\\d+)s\\)");
    std::smatch timeMatch;

    if (std::regex_search(response, timeMatch, timeReg)) {
        if (timeMatch.size() == 5) {
            int days = std::stoi(timeMatch[1]);
            int hours = std::stoi(timeMatch[2]);
            int minutes = std::stoi(timeMatch[3]);
            int seconds = std::stoi(timeMatch[4]);

            // calculate time in seconds
            int totalTime = days * 24 * 60 * 60 + hours * 60 * 60 + minutes * 60 + seconds;

            std::cout << "Total parsed time: " << totalTime << " seconds." << std::endl;
            return std::to_string(totalTime);
        }
    }
    return "";
}

bool ConfigManager::get_prompt(string prompt_type, int max_attempts, bool send_interrupt, int int_interval) {
    response.clear();

    for (int attempts = 0; attempts < max_attempts; attempts++) {
        response.append(serial.read_response());
        if (response.find(prompt_type) != string::npos) {
            return true;
        } else if (send_interrupt && attempts % int_interval == 0) {
            std::cout << "send interrupt" << std::endl;
            serial.send_command(&interrupt_char);
            serial.send_command("\n");
        }
        usleep(10);
    }
    return false;
}

bool ConfigManager::login() {
    const string temp_user = root_user + "\n";
    const string temp_pass = root_pass + "\n";

    if (get_prompt(LOGIN, 100000, true, 10000)) {
        serial.send_command(temp_user.c_str());
    } else {
        return false;
    }
    clear_response();
    std::cout << "start of passwd" << std::endl;
    if (get_prompt(PASSWD, 100000, false, 0)) {
        serial.send_command(temp_pass.c_str());
    } else {
        return false;
    }
    clear_response();
    std::cout << "start of get bash" << std::endl;
    if (get_prompt(BASH, 100000, false, 0)) {
        return true;
    }
    return false;
}

bool ConfigManager::configure_wifi() {
    string connect_cmd = "nmcli device wifi connect " + wifi_network + " password " + wifi_passwd + "\n";
    string ping_cmd = "ping -c2 8.8.8.8\n";

    if (!get_prompt(BASH, 1000000, true, 1000)) {
        std::cout << "Unable to configure wifi - no bash prompt found." << std::endl;
        return false;
    }
    clear_response();
    // attempt to connect
    serial.send_command(connect_cmd.c_str());
    if (!get_prompt("successfully activated", 1000000, false, 0)) {
        return false;
    }
    clear_response();
    // nmcli connection successful - get bash prompt
    if (!get_prompt(BASH, 100000, false, 0)) {
        std::cout << "Unable to get bash prompt." << std::endl;
        return false;
    }
    std::cout << "nmcli command complete - bash prompt found" << std::endl;
    clear_response();
    // connection presumed successful - get prompt and test with ping
    if (!get_prompt(BASH, 1000000, true, 1000)) {
        std::cout << "Unable to get bash prompt for ping test." << std::endl;
        return false;
    }
    std::cout << "Sending ping command" << std::endl;
    serial.send_command(ping_cmd.c_str());
    clear_response();
    if (get_prompt("2 packets transmitted", 10000000, false, 0)) {
        return true;
    }
    return false;
}

bool ConfigManager::run_apt_update_for_clock_fix() {
    string update_command = "apt update \n";

    if (!get_prompt(BASH, 1000000, true, 1000)) {
        std::cout << "Unable to run apt update - no bash prompt found." << std::endl;
        return false;
    }
    clear_response();
    // attempt to run command
    serial.send_command(update_command.c_str());
    if (!get_prompt("Updates for this repository will not be applied.", 100000000, false, 0)) {
        return false;
    }
    std::cout << "Time discrepancy string obtained. Parsing..." << std::endl;
    time_lag = extractTimeDiscrepancy(response);
    clear_response();
    if (time_lag.length() != 0) {
        return true;
    } else {
        return false;
    }
}

bool ConfigManager::send_command(string command) {
    if (!get_prompt(BASH, 1000000, true, 1000)) {
        std::cout << "Unable to get bash prompt - no bash prompt found." << std::endl;
        return false;
    }
    // attempt to run command
    serial.send_command(command.c_str());
    return true;
}

bool ConfigManager::fix_clock() {
    string fix_clock_command = "date -s \"$(date --date='" + time_lag + " seconds' '+%Y-%m-%d %H:%M:%S')\"";

    if (!get_prompt(BASH, 1000000, true, 1000)) {
        std::cout << "Unable to run clock command - no bash prompt found." << std::endl;
        return false;
    }
    clear_response();
    // attempt clock fix command
    serial.send_command(fix_clock_command.c_str());
    if (!get_prompt(BASH, 10000000, false, 0)) {
        std::cout << "Unable to get bash prompt after clock command." << std::endl;
        clear_response();
        return false;
    }
    clear_response();
    return true;
}

