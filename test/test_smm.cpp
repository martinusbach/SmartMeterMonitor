// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#include "file_telegram_generator.hpp"

#include <catch2/catch.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <regex>


// Helper class for using a range-based for loop combined with regex matches.
struct sregex_range : std::pair<std::sregex_iterator, std::sregex_iterator>
{
    sregex_range(std::sregex_iterator begin, std::sregex_iterator end = std::sregex_iterator())
        : pair(begin, end)
    { }
    sregex_range(std::string::const_iterator begin, std::string::const_iterator end, const std::regex& re)
        : sregex_range(std::sregex_iterator(begin, end, re))
    { }
    std::sregex_iterator begin() { return first; }
    std::sregex_iterator end() { return second; }
};


class ObisEntry
{
public:
    ObisEntry(const std::string& name)
        : is_parsed_(false)
        , name_(name)
    { }
    virtual ~ObisEntry() = default;
    virtual void parse(const std::string& s) = 0;
protected:
    bool is_parsed_;
    std::string name_;
};

class IntObisEntry : public ObisEntry
{
public:
    IntObisEntry(const std::string& name)
        : ObisEntry(name)
        , value_()
    { }

    void parse(const std::string& s)
    {
        value_ = std::stoi(s);
        std::cout << "parsed int\n";

        is_parsed_ = true;
    }
private:
    int value_;
};

class TimestampObisEntry : public ObisEntry
{
public:
    TimestampObisEntry(const std::string& name)
        : ObisEntry(name)
        , year_()
        , month_()
        , day_()
    { }
    void parse(const std::string& s)
    {
        const std::regex re("(0[1-9]|[12][0-9]|3[01])(0[1-9]|1[012])([0-9]{2})(2[0-3]|[0-1][0-9])([0-5][0-9])([0-5][0-9])([SW])");
        std::smatch match;
        if (!std::regex_search(s, match, re)) {
            throw std::runtime_error("date not found!");
        }
        year_ = std::stoi(match.str(1));
        month_ = std::stoi(match.str(2));
        day_ = std::stoi(match.str(3));
        hour_ = std::stoi(match.str(4));
        min_ = std::stoi(match.str(5));
        sec_ = std::stoi(match.str(6));
        dst_ = match.str(7)[0];

        std::cout << "parsed date " << dst_ << "\n";
        is_parsed_ = true;
    }
private:
    int year_;
    int month_;
    int day_;
    int hour_;
    int min_;
    int sec_;
    char dst_;
};

class MeasurementObisEntry : public ObisEntry
{
public:
    MeasurementObisEntry(const std::string& name)
        : ObisEntry(name)
        , value_()
    { }

    void parse(const std::string& s)
    {
        const std::regex re(R"((\d+)(?:\.(\d+))?\*(\w+))");
        std::smatch match;
        if (!std::regex_search(s, match, re)) {
            throw std::runtime_error("date not found!");
        }
        value_ = std::stod(match.str(1));
        if (match.str(2) != "") {
            value_ += std::stod("0." + match.str(2));
        }
        unit_ = match.str(3);

        std::cout << "parsed a measurent: " << value_ << unit_ << ".\n";
        is_parsed_ = true;
    }
private:
    double value_;
    std::string unit_;
};


TEST_CASE("First parser test", "[parser]")
{
    REQUIRE(true);

    FileTelegramGenerator gen;
    std::string telegram = gen.new_telegram();


    std::map<std::string, std::unique_ptr<ObisEntry>> telegram_entries;
    telegram_entries.emplace("1-3:0.2.8", std::make_unique<IntObisEntry>("version info (S2)"));
    telegram_entries.emplace("0-0:1.0.0", std::make_unique<TimestampObisEntry>("time stamp"));
    telegram_entries.emplace("1-0:1.8.1", std::make_unique<MeasurementObisEntry>("tariff 1, to client"));
    telegram_entries.emplace("1-0:1.8.7123", std::make_unique<MeasurementObisEntry>("test entry"));


    const std::regex re_header(R"(/([A-Z]{3}\d[A-Z0-9]{16}))");
    std::smatch match_header;
    if (std::regex_search(telegram, match_header, re_header)) {
        std::cout << "header: " << match_header.str(1) << ".\n";
    }

    const std::regex re_OBISID_and_data(R"(([01]-[03]:\d{1,2}\.\d{1,2}\.\d{1,2})\(([^()]*)\)(.*))");

    for (const auto& match : sregex_range(telegram.begin(), telegram.end(), re_OBISID_and_data)) {
        const std::string& obis_id = match.str(1);
        const std::string& data = match.str(2);
        const std::string& data_extra = match.str(3);
        std::cout << "OBIS ID: `" << obis_id << "`, data: `" << data << "`";
        if (data_extra != "") {
            std::cout << ", extra data: `" << data_extra << "`";
        }
        std::cout << "\n";

        // start parsing
        try {
            auto& tg_entry = telegram_entries.at(obis_id);
            tg_entry->parse(data);
        } catch (const std::out_of_range&) {
            std::cout << "obis id " << obis_id << " not parsed\n";
        }
    }

}
