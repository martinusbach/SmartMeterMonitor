#ifndef TELEGRAM_GENERATOR_HPP
#define TELEGRAM_GENERATOR_HPP

#include <string>


class TelegramGenerator
{
public:
    TelegramGenerator(); 
    ~TelegramGenerator();
    TelegramGenerator(const TelegramGenerator&) = delete;

    std::string new_telegram();

private:
    int serial_port_;
};

#endif  // TELEGRAM_GENERATOR_HPP

