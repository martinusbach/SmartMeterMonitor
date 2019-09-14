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


int main(int argc, char* argv[])
{
    std::cout << "Starting Smart Meter Monitor.\n";

    signal(SIGINT, signal_handler);

    int serial_port = open("/dev/ttyUSB0", O_RDONLY);
    if (serial_port <= 0) {
        std::cout << "Something went wrong: " << serial_port << "\n";
        return -1;
    }

    // Read in existing settings, and handle any error
    struct termios tty = {};
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
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
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }


    keep_running = 1;

    // Reading
    std::cout << "Start reading from serial port\n";
    size_t j = 0;
    bool save_line = false;
    char buffer[1092] = {};  // the data block is max 1024 bytes, but we also have the prefix and the crc16 code.
    while (keep_running) {
        int n_chars_read = read(serial_port, &buffer[j], sizeof(buffer) - j);
        std::cout << " --- " << n_chars_read << " bytes read -- " << j << "\n";
        if (n_chars_read > 0) {
            if (buffer[j] == '/') {
                save_line = true;
            }
            else if (buffer[j] == '!') {
                buffer[n_chars_read + j] = '\0';
                std::cout << "complete telegram:\n" << buffer << "\n";
                j = 0;
                save_line = false;
            }
            if (save_line) {
                j += n_chars_read;
            }
        }
    }

    // Finishing
    int result = close(serial_port);
    if (result != 0) {
        std::cout << "Something went wrong during close(): " << result << "\n";
        return -1;
    }
    std::cout << "Finished\n";

    return 0;
}

