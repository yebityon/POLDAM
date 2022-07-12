#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <utility>

namespace POLDAM
{

    class fileReader
    {
    private:
        virtual void readFile() = 0;
        virtual void parseReadlines() = 0;
        virtual void parseLine() = 0;
        // traverse given directory name and find target file name.
        // dirTraversal call parseReadLines in directory iteration
        virtual void dirTraversal(std::string) = 0;
    };

}