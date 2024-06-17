#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "SerialComm.h"
#include <string>
#include <regex>
#include <iostream>

using std::string;

class ConfigManager {
private:
    SerialComm serial;
    string response;
    string time_lag;
    string wifi_network;
    string wifi_passwd;
    string root_user;
    string root_pass;
    string LOGIN = "login:";
    string PASSWD = "Password:";
    string BASH = "~#";
    const char interrupt_char = 0x03;

    void clear_response();
    string extractTimeDiscrepancy(const string& response);
    bool get_prompt(string prompt_type, int max_attempts, bool send_interrupt, int int_interval);

public:
    ConfigManager(const string device, string wifi_network, string wifi_passwd, string root_user, string root_pass);

    bool login();
    bool configure_wifi();
    bool run_apt_update_for_clock_fix();
    bool send_command(string command);
    bool fix_clock();
};

#endif // CONFIGMANAGER_H

