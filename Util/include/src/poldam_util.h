#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace POLDAM_UTIL
{
    const char *const POLDAM_PRINT_SUFFIX = "[POLDAM]: ";
    const char *const POLDAM_ERROR_PRINT_SUFFIX = "[POLDAM ERROR]: ";

    std::vector<std::string> parse(std::string, const char = ',');
    std::vector<std::string> split(std::string, const char = '=');

}
