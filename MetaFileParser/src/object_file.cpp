#include "object_file.h"
#include <cassert>
namespace POLDAM
{

    void ObjectfileParser::dirTraversal(const std::string &dirName)
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
                this->typesFilePath = std::string(filePath.c_str());
            }
        }
    };

    void ObjectfileParser::readFile(const std::string &filePath, std::vector<std::string> &data)
    {
        std::ifstream fileStream;

        fileStream.open(filePath, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << filePath << " is not found\n";
            std::cout << "DEBUG: " << filePath << '\n';
            exit(1);
        }

        while (not fileStream.eof())
        {
            std::string buffer;
            std::getline(fileStream, buffer);

            if (buffer.empty())
                continue;

            data.push_back(std::move(buffer));
        }

        fileStream.close();
    };

    void ObjectfileParser::parseLine(const std::string &line)
    {
        std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        unsigned int objectTypeId = static_cast<unsigned int>(std::stoi(d[1]));
        this->parsedObjectTypesDatas.emplace_back(objectTypeId);
    };

    void ObjectfileParser::parseLogTypesLine(const std::string &line)
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
                rec["objectType"] = parsedVec[1];
            }
            else if (i == 2)
            {
                rec["jarFile"] = parsedVec[2];
            }
            // typenum1
            else if (i == 3)
            {
                rec["typenum1"] = parsedVec[3];
            }
            // typenum2
            else if (i == 4)
            {
                rec["typenum2"] = parsedVec[4];
            }
            // loader
            else if (i == 5)
            {
                rec["loadFrom"] = parsedVec[5];
            }
            else
            {
                assert(false && "Undefined key is recored in LOGTypefile Parser");
            }
        }
        this->parsedTypesDatas.push_back(rec);
        return;
    }

    void ObjectfileParser::parseStringLine(const std::string &line)
    {
        // see also
        // https://github.com/takashi-ishio/selogger/blob/924f3d961344bf32cddc93709ed9609f27905191/src/selogger/logging/util/StringContentFile.java#L32

        std::vector<std::string> d = POLDAM_UTIL::split(line, ',');
        unsigned int objectIdx = static_cast<unsigned int>(std::stoi(d[0]));
        // Do not recored length of target string.
        this->parsedStringDatas[objectIdx] = d[2];
    }

    void ObjectfileParser::readObjectTypeData()
    {
        // read object Types
        this->readFile(this->objectTypesFilePath, this->objectTypesDatas);
        this->setObjectfileType("object");
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
            if (this->fileType == "object")
            {
                this->parseLine(line);
            }
            else if (this->fileType == "string")
            {
                parseStringLine(line);
            }
            else
            {
                parseLogTypesLine(line);
            }
        }
    }

    void ObjectfileParser::accumulateObjectFile()
    {
        for (unsigned int id = 0; id < this->parsedObjectTypesDatas.size(); ++id)
        {
            ObjectData obj{};
            // dataids.txt
            unsigned int objectId = parsedObjectTypesDatas[id];
            obj.fileIdx_ = id + 1;

            // map dataid to objectid
            obj.objectId = objectId;

            std::map<std::string, std::string> typesData = this->parsedTypesDatas[objectId];

            obj.objectType = typesData["objectType"];
            obj.jarFile = typesData["jarFile"];

            obj.typenum1 = std::stoi(typesData["typenum1"]);
            obj.typenum2 = std::stoi(typesData["typenum2"]);

            obj.loadFrom = typesData["loadFrom"];

            if (obj.objectType == "java.lang.String")
            {
                assert(parsedStringDatas.find(obj.fileIdx_) != parsedStringDatas.end());

                obj.stringValue = parsedStringDatas[obj.fileIdx_];
            }

            this->objectDatas.emplace_back(obj);
        }
    };
}
