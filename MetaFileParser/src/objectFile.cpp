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
        std::cout << filePath << std::endl;

        fileStream.open(filePath, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << filePath << " is not found\n";
            exit(1);
        }

        while (not fileStream.eof())
        {
            std::string buffer;
            std::getline(fileStream, buffer);
            std::cout << buffer << std::endl;

            if (buffer.empty())
                continue;

            data.push_back(std::move(buffer));
        }

        fileStream.close();
        std::cout << "\nfileStream is closed" << std::endl;
    };

    void ObjectfileParser::parseLine(std::string line)
    {
        std::cout << "DEBUG -> " << line << std::endl;
        std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        std::cout << d[0] << " YEBITYON  " << d[1] << std::endl;
        unsigned int objectTypeId = static_cast<unsigned int>(std::stoi(d[1]));
        this->parsedObjectTypesData.emplace_back(objectTypeId);
    };

    void ObjectfileParser::parseLogLine(std::string line)
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
        }
        this->parsedLogTypeData.push_back(rec);
        return;
    }

    void ObjectfileParser::parseStringLine(std::string line)
    {
        std::cout << "parseStringLine()" << std::endl;
        std::cout << "line: " << line << std::endl;

        std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        unsigned int dataidIdx = static_cast<unsigned int>(std::stoi(d[0]));
        // we do not recored length of target string
        this->parsedStringData[dataidIdx] = d[1];
    }

    void ObjectfileParser::readObjectTypeData()
    {
        // read object Types
        this->readFile(this->objectTypeFilePath, this->objectTypeData);
        this->setObjectfileType("object");
        std::cout << "call parseReadLines" << std::endl;
        this->parseReadlines(this->objectTypeData);

        // read string data
        this->readFile(this->stringFilePath, this->stringData);
        this->setObjectfileType("string");
        this->parseReadlines(this->stringData);

        // read object data
        this->readFile(this->logTypeFilePath, this->logTypeData);
        this->setObjectfileType("log");
        this->parseReadlines(this->logTypeData);
    };

    void ObjectfileParser::parseReadlines(std::vector<std::string> &data)
    {
        // TODO: ungly, need to fix logic.

        for (auto line : data)
        {
            if (this->fileType == "objcet")
            {
                std::cout << "[parse Object] " << line << std::endl;
                this->parseLine(line);
            }
            else if (this->fileType == "string")
            {
                std::cout << "[parse String] " << line << std::endl;
                parseStringLine(line);
            }
            else
            {
                std::cout << "[parse Log line] " << line << std::endl;
                parseLogLine(line);
            }
        }
    }

    void ObjectfileParser::accumulateObjectFile()
    {
        for (unsigned int id = 0; id < parsedLogTypeData.size(); ++id)
        {
            std::cout << id << std::endl;
            for (const auto [a, b] : parsedLogTypeData[id])
            {
                std::cout << "key:" << a << " value: " << b << std::endl;
            }
            ObjectData obj{};
            // dataids.txt
            auto &logData = parsedLogTypeData[id];
            obj.objectId = id + 1;
            // map dataid to objectid
            obj.objectTypesId = parsedObjectTypesData[id];
            obj.objecttype = logData["objecttype"];
            obj.loadclasss = logData["loadclass"];

            obj.typenum1 = std::stoi(logData["typenum1"]);
            obj.typenum2 = std::stoi(logData["typenum2"]);
            obj.loader = logData["loader"];
            if (obj.objecttype == "string")
            {
                obj.value = parsedStringData[obj.objectId];
            }

            this->accumulatedData.emplace_back(obj);
        }
    };
}
