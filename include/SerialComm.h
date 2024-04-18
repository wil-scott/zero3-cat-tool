#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

class SerialComm {
private:
    int serial_port;
    struct termios tty;

    bool write_data(const char* data, size_t length);
    bool read_data(char* buffer, size_t buffer_size);

public:
    SerialComm(const char* port_name);
    ~SerialComm();
    bool send_command(const char* command);
    std::string read_response();
};

#endif