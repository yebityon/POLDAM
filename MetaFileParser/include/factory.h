#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace POLDAM
{
    class metafileFactory
    {
    public:
        metafileFactory(std::string dirPath_) : dirPath(dirPath_){};
        std::vector<std::string> createLogInstance();

    private:
        const std::string dirPath;
    };
}