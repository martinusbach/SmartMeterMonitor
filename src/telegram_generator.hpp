// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#ifndef TELEGRAM_GENERATOR_HPP
#define TELEGRAM_GENERATOR_HPP

#include <fstream>
#include <string>


class TelegramGenerator
{
public:
    virtual ~TelegramGenerator() {};
    virtual std::string new_telegram() = 0;
};


#endif  // TELEGRAM_GENERATOR_HPP

