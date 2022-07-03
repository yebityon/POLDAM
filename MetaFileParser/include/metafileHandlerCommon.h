#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace POLDAM
{

    class fileReader
    {
    private:
        virtual void readFile() = 0;
        virtual void parseReadlines() = 0;
        virtual void parserLine() = 0;
    };

}