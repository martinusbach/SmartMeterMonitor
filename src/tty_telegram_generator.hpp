// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#ifndef TTY_TELEGRAM_GENERATOR_HPP
#define TTY_TELEGRAM_GENERATOR_HPP

#include "telegram_generator.hpp"

#include <fstream>
#include <string>


class ttyTelegramGenerator : public TelegramGenerator
{
public:
    ttyTelegramGenerator();
    ~ttyTelegramGenerator() final;
    ttyTelegramGenerator(const ttyTelegramGenerator&) = delete;

    virtual std::string new_telegram() final;

private:
    int serial_port_;
};


#endif  // TTY_TELEGRAM_GENERATOR_HPP

