#pragma once
#include "objectFile.h"

namespace POLDAM
{

    void ObjectfileParser::dirTraversal(const std::string dirName)
    {
        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(dirName))
        {
            const auto filePath = i.path();

            if (filePath.filename() == this->objectFileName)
            {
                this->objectTypeFilePath = std::string(i.path().c_str());
            }
            else if (filePath.filename() == this->stringFileName)
            {
                this->stringFilePath = std::string(filePath.c_str());
            }
            else if (filePath.filename() == this->logTypeFileName)
            {
                this->logTypeFilePath = std::string(filePath.c_str());
            }
        }
    };

    void ObjectfileParser::readFile(const std::string filePath, std::vector<std::string> &data)
    {
        std::ifstream fileStream;

        fileStream.open(this->objectFileName, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "objectfile is nott found\n";
            exit(1);
        }

        while (not fileStream.eof())
        {
        }
    };

    // void ObjectfileParser::readObjectTypeData()
    // {

    //     for (const auto &line : this->objectTypeData)
    //     {
    //     }
    // };

    // void ObjectfileParser::accumulateObjectFile(){};
}
