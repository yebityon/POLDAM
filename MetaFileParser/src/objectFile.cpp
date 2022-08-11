#pragma once
#include "objectFile.h"

namespace POLDAM
{

    void ObjectfileParser::dirTraversal(const std::string dirName)
    {
        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(dirName))
        {
            const auto filePath = i.path();

            if (filePath.filename() == this->ojbectTypesFileName)
            {
                this->objectTypesFilePath = std::string(i.path().c_str());
            }
            else if (filePath.filename() == this->stringFileName)
            {
                this->stringFilePath = std::string(filePath.c_str());
            }
            else if (filePath.filename() == this->typesFileName)
            {
                this->typesFileName = std::string(filePath.c_str());
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
        std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        unsigned int objectTypeId = static_cast<unsigned int>(std::stoi(d[1]));
        this->parsedObjectTypesDatas.emplace_back(objectTypeId);
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
        this->parsedTypesDatas.push_back(rec);
        return;
    }

    void ObjectfileParser::parseStringLine(std::string line)
    {
        std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        unsigned int dataidIdx = static_cast<unsigned int>(std::stoi(d[0]));
        // we do not recored length of target string
        this->parsedStringDatas[dataidIdx] = d[1];
    }

    void ObjectfileParser::readObjectTypeData()
    {
        // read object Types
        this->readFile(this->objectTypesFilePath, this->objectTypesDatas);
        this->setObjectfileType("object");
        std::cout << "call parseReadLines" << std::endl;
        this->parseReadlines(this->objectTypesDatas);

        // read string data
        this->readFile(this->stringFilePath, this->stringDatas);
        this->setObjectfileType("string");
        this->parseReadlines(this->stringDatas);

        // read object data
        this->readFile(this->typesFilePath, this->typesDatas);
        this->setObjectfileType("log");
        this->parseReadlines(this->typesDatas);
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
        for (unsigned int id = 0; id < this->parsedObjectTypesDatas.size(); ++id)
        {
            std::cout << id << std::endl;

            ObjectData obj{};
            // dataids.txt
            auto &logData = parsedObjectTypesDatas[id];
            obj.objectId = id + 1;
            // map dataid to objectid
            obj.objectTypesId = parsedObjectTypesDatas[id];
            obj.objecttype = logData["objecttype"];
            obj.loadclasss = logData["loadclass"];

            obj.typenum1 = logData["typenum1"];
            obj.typenum2 = logData["typenum2"];
            obj.loader = logData["loader"];
            if (obj.objecttype == "string")
            {
                obj.value = parsedStringDatas[obj.objectId];
            }

            this->objectDatas.emplace_back(obj);
        }
    };
}
