// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#ifndef FILE_TELEGRAM_GENERATOR_HPP
#define FILE_TELEGRAM_GENERATOR_HPP

#include "../src/telegram_generator.hpp"

#include <fstream>
#include <string>


class FileTelegramGenerator : public TelegramGenerator
{
public:
    FileTelegramGenerator();
    ~FileTelegramGenerator() final;
    FileTelegramGenerator(const FileTelegramGenerator&) = delete;

    virtual std::string new_telegram() final;

private:
    std::ifstream telegram_file_;
};


#endif  // FILE_TELEGRAM_GENERATOR_HPP

