// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#include "telegram_generator.hpp"

#include <fstream>
#include <iostream>


FileTelegramGenerator::FileTelegramGenerator()
    : telegram_file_("telegrams.log")
{
    if (!telegram_file_.is_open()) {
        throw std::runtime_error("Failed to open telegrams file.");
    }
}


std::string FileTelegramGenerator::new_telegram()
{
    // Reading
    std::cout << "Start reading example telegram from file\n";
    
    bool save_line = false;
    std::string telegram;
    std::string line;
    while (std::getline(telegram_file_, line)) {
        if (line.size() == 0) {
            continue;
        }
        if (line[0] == '/') {
            save_line = true;
        }
        if (save_line) {
            telegram += line + "\r\n";
        }
        if (line[0] == '!') {
            return telegram;
        }
    }

    return "";
}


FileTelegramGenerator::~FileTelegramGenerator()
{
}

