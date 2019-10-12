// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#ifndef TELEGRAM_GENERATOR_HPP
#define TELEGRAM_GENERATOR_HPP

#include <string>


class TelegramGenerator
{
public:
    virtual ~TelegramGenerator() {};
    virtual std::string new_telegram() = 0;
};


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


class FileTelegramGenerator : public TelegramGenerator
{
public:
    FileTelegramGenerator();
    ~FileTelegramGenerator() final;
    FileTelegramGenerator(const FileTelegramGenerator&) = delete;

    virtual std::string new_telegram() final;

private:
    int serial_port_;
};

#endif  // TELEGRAM_GENERATOR_HPP

