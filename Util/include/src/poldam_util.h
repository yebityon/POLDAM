#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
namespace POLDAM_UTIL
{
    const char *const POLDAM_PRINT_SUFFIX = "[POLDAM]: ";
    const char *const POLDAM_ERROR_PRINT_SUFFIX = "[POLDAM ERROR]: ";

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
