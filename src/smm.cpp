// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#include "tty_telegram_generator.hpp"

#include <iostream>


void print_telegrams(TelegramGenerator& gen)
{
    std::string telegram;
    while ( ! (telegram = gen.new_telegram() ).empty() ) {
        std::cout << "new telegram:\n" << telegram;
    }
}


int main(int argc, char* argv[])
{
    std::cout << "Starting Smart Meter Monitor.\n";

    ttyTelegramGenerator gen;
    print_telegrams(gen);

    std::cout << "Finished Smart Meter Monitor.\n";

    return 0;
}

