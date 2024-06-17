# zero3-cat-tool
This tool is designed to work with the Zero3 Boot Wizard and the Orange Pi Zero3. It requires a serial connection to the device. The tool will:
1. Prompt the user for the /dev/tty* of the Zero3,
2. Prompt the user for root user/password information and wifi id/password information,
3. Connect over serial and login automatically,
4. Use nmcli to connect to the local wifi network, and
5. Attempt to rectify the system clock/debian server datetime discrepancy.

# Requirements
1. A Linux system
2. gcc-g++ compiler
3. a USB-to-TTY connector to faciliate the serial connection between your workstation and the Zero3

# How to Use:
1. Clone the tool's repository into your workspace and navigate into it
2. Compile the tool with this command: `g++ -I./include -o main src/ConfigManager.cpp src/SerialComm.cpp main.cpp`
3. Run the tool with sudo privileges (either as root or with the sudo command): `sudo ./main` or `./main`
4. Input the /dev/tty* and other required data when prompted by the tool

# Known Issues:
1. Serial communication to a device/command line interface can be a bit spotty. The code attempts to walk the line between locking the program (by polling continuously for a bash prompt for all enternity, for example) versus timing out/failing gracefully. The tool may not work the first time around, so you may need to manually power on/off or reset the device to the login prompt.
2. The time discrepancy code is buggy - I've found the quality of the wifi connection to be a significant factor, as the tool may time out before the sourcelist has been checked via the apt update command.

# Final Thoughts:
This tool is not perfect, but provides a base level of functionality for handling some of the configuration left over from the Boot Wizard. I've never used C++ in a project like this, so as a learning experience it was fantastic. I plan to revisit it and address some of the bugs when I need a break from my other projects.
