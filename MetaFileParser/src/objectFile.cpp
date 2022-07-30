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
            std::string buffer;
            std::getline(fileStream, buffer);

            data.push_back(buffer);
        }

        fileStream.close();
    };

    void ObjectfileParser::parseReadlines(const std::vector<std::string> &data)
    {
        // TODO: ungly, you need to fix logic here.

        for (const auto &line : data)
        {
            if (this->fileType == "objcet")
            {
                parseLine(line);
            }
            else if (this->fileType == "log")
            {
                parseLogLine(line);
            }
            else
            {
                parseStringLine(line);
            }
        }
    }

    void ObjectfileParser::parseLine(const std::string line){};

    void ObjectfileParser::readObjectTypeData()
    {
        this->readFile(this->objectTypeFilePath, this->objectTypeData);
        this->readFile(this->stringFilePath, this->stringData);
        this->readFile(this->logTypeFilePath, this->logtypeData);
    };

    void ObjectfileParser::accumulateObjectFile(){

    };
}
