#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace POLDAM_UTIL
{
    std::vector<std::string> parse(std::string, const char = ',');
    std::vector<std::string> split(std::string, const char = '=');

}