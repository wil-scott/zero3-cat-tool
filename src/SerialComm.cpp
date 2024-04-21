#include "../include/SerialComm.h"

using std::string;

SerialComm::SerialComm(const char* port_name) {
    /*
    * Initializes an instance of the SerialComm object.
    * 
    * :param port_name: the name of the serial device to write to/from        
    */        
    serial_port = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
    std::cout << serial_port << std::endl;
    // error check serial port was opened correctly
    if (serial_port < 0) {
        std::cerr <<"Error " << errno << " from opening " << port_name << std::endl;
    }

    // Initalize tty struct
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr <<"Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    cfsetspeed(&tty, B115200);      // Set baud rate 
    tty.c_cflag &= ~PARENB;         // Disable parity bit
    tty.c_cflag &= ~CSTOPB;         // Use 1 stop bit (not 2)
    tty.c_cflag &= ~CSIZE;          // Clear size bits
    tty.c_cflag |= CS8;             // set 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;        // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL;  // Enable reading and ignore control lines

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // No software flow control
    // Disable special handling of return bytes 
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL ); 
    
    tty.c_lflag &= ~ICANON; // No canonical mode
    tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT, SUSP
    tty.c_oflag &= ~OPOST;  // Disable special interpretation of output bytes
    tty.c_oflag &= ~ONLCR;  // Disable conversion of newline to carriage return/line feed
    tty.c_cc[VMIN] = 0;     // Read doesn't block
    tty.c_cc[VTIME] = 10;   // wait for data

    // save configuration of termios struct
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr <<"Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
    }
}

SerialComm::~SerialComm() {
    /* 
    * Destroys object and closes serial port.
    */
    close(serial_port);
}

bool SerialComm::send_command(const char* command) {
    /*
    * Writes a string over serial connection.
    * 
    * :param command: A char pointer to a string/array, representing the command
    * :return: True if successful, else false
    */
    if (!write_data(command, string(command).length())) {
        return false;
    }

    return true;
}

string SerialComm::read_response() {
    /*
    * Reads data sent over the serial connection.
    * 
    * :return: a string representing the data received from serial
    */
    char buffer[1024];
    if (!read_data(buffer, sizeof(buffer))) {
        return "";
    }

    return string(buffer);
}
    
bool SerialComm::write_data(const char* data, size_t length) {
        /*
        * Writes bytes to serial.
        * 
        * :param data: a pointer to a string/char array, the data to be sent
        * :param length: the length of the string/char array to be sent
        * :return: true if successful, else false
        */
        ssize_t bytes_written = write(serial_port, data, length);
        if (bytes_written < 0) {
            std::cerr << "Error writing data to serial port: " << errno << std::endl;
            std::cerr << strerror(errno) << std::endl;
            std::cerr << serial_port << std::endl;
            exit(-1);
        }
        //std::cout << "Wrote data to device." << std::endl;
        return true;
}

bool SerialComm::read_data(char* buffer, size_t buffer_size) {
        /*
        * Reads bytes from serial.
        * 
        * :param buffer: a pointer to a char array
        * :param buffer_size: an int representing the size of the char array
        * :return: true if data read successfully, else false
        */
        ssize_t bytes_read = 0;
        bytes_read = read(serial_port, buffer, buffer_size);
        if (bytes_read < 0) {
            std::cerr << "Error reading data from serial port." << std::endl;
            return false;
        }
        buffer[bytes_read] = '\0';
        return true;
}
