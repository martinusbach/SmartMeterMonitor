#include "telegram_generator.hpp"

#include <termios.h> // Contains POSIX terminal control definitions
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <iostream>


volatile sig_atomic_t keep_running;

void signal_handler(int signum)
{
    std::cout << "Signal nr " << signum << " caught\n";
    keep_running = 0;
}


TelegramGenerator::TelegramGenerator() 
{
    std::cout << "Opening tty.\n";

    signal(SIGINT, signal_handler);

    serial_port_ = open("/dev/ttyUSB0", O_RDONLY);
    if (serial_port_ <= 0) {
        throw std::runtime_error("something went wrong: "); // TODO: add return value in seriap_port 
    }

    // Read in existing settings, and handle any error
    struct termios tty = {};
    if (tcgetattr(serial_port_, &tty) != 0) {
        throw std::runtime_error("Error from tcgetattr"); // TODO: printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port_, TCSANOW, &tty) != 0) {
        throw std::runtime_error("Error from tcsetattr"); 
        //TODO: printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
}


std::string TelegramGenerator::new_telegram()
{
    keep_running = 1;

    // Reading
    std::cout << "Start reading from serial port\n";
    size_t j = 0;
    bool save_line = false;
    char buffer[1092] = {};  // the data block is max 1024 bytes, but we also have the prefix and the crc16 code.
    while (keep_running) {
        int n_chars_read = read(serial_port_, &buffer[j], sizeof(buffer) - j);
        std::cout << " --- " << n_chars_read << " bytes read -- " << j << "\n";
        if (n_chars_read > 0) {
            if (buffer[j] == '/') {
                save_line = true;
            }
            else if (buffer[j] == '!') {
                buffer[n_chars_read + j] = '\0';
                std::cout << "complete telegram received:\n";
                //std::cout << buffer << "\n";
                return std::string(buffer);
            }
            if (save_line) {
                j += n_chars_read;
            }
        }
    }

    return "";
}


TelegramGenerator::~TelegramGenerator()
{
    // Finishing
    int result = close(serial_port_);
    if (result != 0) {
        std::runtime_error("Something went wrong during close(): "); //TODO: << result << "\n";
    }
    std::cout << "Closed ttyUSB0\n";
}

