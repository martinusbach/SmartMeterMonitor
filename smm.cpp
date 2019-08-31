#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[]) 
{ 
    std::cout << "Test" << O_RDWR << "\n";

    int fd = open("/dev/ttyUSB0", O_RDONLY);

    std::cout << fd << "\n";

    if (fd <= 0) {
        std::cout << "Something went wrong: " << fd << "\n";
        return -1;
    }
    
    int result = close(fd);

    if (result != 0) {
        std::cout << "Something went wrong during close(): " << result << "\n";
        return -1;
    }

    return 0;
}
