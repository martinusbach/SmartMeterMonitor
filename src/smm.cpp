#include "telegram_generator.hpp"

#include <iostream>


int main(int argc, char* argv[])
{
    std::cout << "Starting Smart Meter Monitor.\n";

    TelegramGenerator tg;

    std::string s; 
    while ( ! (s = tg.new_telegram() ).empty() )
    {
        std::cout << "new telegram:\n" << s;
    }

    std::cout << "Finished Smart Meter Monitor.\n";

    return 0;
}

