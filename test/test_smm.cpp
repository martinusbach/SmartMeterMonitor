// Copyright (c) 2019 Martien Oppeneer. 
// This software is MIT licensed; see LICENSE.MIT. 

#include "file_telegram_generator.hpp"

#include <catch2/catch.hpp>

#include <iostream>


TEST_CASE("First parser test", "[parser]") 
{
    REQUIRE(true);
    
    FileTelegramGenerator gen;
    std::cout << gen.new_telegram();
}

