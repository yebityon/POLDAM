#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
namespace POLDAM_UTIL
{
    const char *const POLDAM_PRINT_SUFFIX = "\033[1m[POLDAM]\033[0m:";
    const char *const POLDAM_ERROR_PRINT_SUFFIX = "\033[31m\033[1m[POLDAM ERROR]\033[0m";
    const char *const POLDAM_WARNING_PRINT_SUFFIX = "\033[35m\033[1m[POLDAM WARNING]\033[0m:";
    const char *const POLDAM_DEBUG_PRINT_SUFFIX = "\033[38;5;208m\033[1m[POLDAM DEBUG]\033[0m:";

    std::vector<std::string> split(const std::string &, const char & = ',');

    struct Timer
    {
        Timer(const std::string &timerName_) : timerName(std::move(timerName_))
        {
            start = std::chrono::system_clock::now();
        }
        ~Timer()
        {
            end = std::chrono::system_clock::now();
            const auto elaps = end - start;
            const auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elaps).count();
            std::cout << POLDAM_PRINT_SUFFIX << timerName << ": " << msec << " msec\n";
        }

    private:
        std::string timerName;
        std::chrono::system_clock::time_point start, end;
    };
}
