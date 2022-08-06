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
        // TODO: ungly, need to fix logic.

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

    void ObjectfileParser::parseLine(const std::string line)
    {
        const std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        const std::pair<unsigned int, unsigned int> rec =
            std::make_pair(static_cast<unsigned int>(std::stoi(d[0])), static_cast<unsigned int>(std::stoi(d[1])));

        this->parsedObjectTypesData.emplace_back(rec.second);
    };

    void ObjectfileParser::parseLogLine(const std::string line)
    {
        const std::vector<std::string> parsedVec = POLDAM_UTIL::split(line, ',');
        std::map<std::string, std::string> rec{};

        for (int i = 0; i < parsedVec.size(); ++i)
        {
            // dataid
            if (i == 0)
            {
                continue;
            }
            // objectType
            else if (i == 1)
            {
                rec["objecttype"] = parsedVec[i];
            }
            // loadclass
            else if (i == 2)
            {
                rec["loadlclass"] = parsedVec[i];
            }
            // typenum1
            else if (i == 3)
            {
                rec["typenum1"] = parsedVec[i];
            }
            // typenum2
            else if (i == 4)
            {
                rec["typenum2"] = parsedVec[i];
            }
            // loader
            else if (i == 5)
            {
                rec["loader"] = parsedVec[i];
            }
            else
            {
                assert(false && "Undefined key is recored in LOGTypefile Parser");
            }

            this->parsedLogTypeData.push_back(rec);
        }

        return;
    }

    void ObjectfileParser::parseStringLine(const std::string line)
    {
        const std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        unsigned int dataidIdx = static_cast<unsigned int>(std::stoi(d[0]));
        // we do not recored length of target string
        this->parsedStringData[dataidIdx] = d[2];
    }

    void ObjectfileParser::readObjectTypeData()
    {
        // read object file
        this->readFile(this->objectTypeFilePath, this->objectTypeData);
        this->setObjectfileType("object");
        this->parseReadlines(this->objectTypeData);

        // read string data
        this->readFile(this->stringFileName, this->stringData);
        this->setObjectfileType("string");
        this->parseReadlines(this->stringData);

        // read object data
        this->readFile(this->logTypeFilePath, this->logTypeData);
        this->setObjectfileType("log");
        this->parseReadlines(this->logTypeData);
    };

    void ObjectfileParser::accumulateObjectFile()
    {
        for (unsigned int dataid = 0; dataid < this->parsedLogTypeData.size(); ++dataid)
        {
            ObjectData obj{};
            auto &logData = parsedLogTypeData[dataid];
            obj.dataidx = dataid;
            obj.objecttype = logData["objecttype"];

            this->accumulatedData.emplace_back(obj);
        }
    };
}
