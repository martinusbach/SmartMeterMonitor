// Copyright (c) 2019 Martien Oppeneer.
// This software is MIT licensed; see LICENSE.MIT.

#include "file_telegram_generator.hpp"

#include <catch2/catch.hpp>

#include <iostream>
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


TEST_CASE("First parser test", "[parser]")
{
    REQUIRE(true);

    FileTelegramGenerator gen;
    std::string telegram = gen.new_telegram();


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
    }

}

