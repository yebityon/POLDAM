#pragma once

#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct ObjectData
    {
        // recored in  LOG$ObjectTypes
        unsigned int objectId;
        // map to LOG$Types file
        unsigned int objectTypesId;
        // typenum is recored in LOG$Types.txt
        int typenum1;
        int typenum2;

        std::string loadclasss;
        std::string objecttype;
        std::string loader;
        //  if type is sstring, concreate value is available
        std::string value;
    };

    class ObjectfileParser : fileReader
    {
    public:
        ObjectfileParser(std::string inputDir_)
        {
            this->dirTraversal(inputDir_);
            this->readObjectTypeData();
            this->accumulateObjectFile();
        };

        std::vector<ObjectData> getParsedData()
        {
            return this->accumulatedData;
        }

    private:
        void readFile(const std::string filePath, std::vector<std::string> &data) override;
        void parseReadlines(std::vector<std::string> &data) override;
        void dirTraversal(std::string fileName) override;

        void setObjectfileType(std::string fileType)
        {
            std::cout << "Try to set fileType" << std::endl;
            this->fileType = fileType;
            std::cout << "succesfully set fileType --->" << this->fileType << std::endl;
        };

        void parseLine(std::string line) override;
        void parseLogLine(std::string line);
        void parseStringLine(std::string line);

        void accumulateObjectFile();
        void readObjectTypeData();

        std::pair<int, int> parseObjectData(const std::string line);

        std::string fileType{};

        std::string objectFileName{"LOG$ObjectTypes00001.txt"};
        std::string stringFileName{"LOG$String00001.txt"};
        std::string logTypeFileName{"LOG$Types.txt"};

        std::string objectTypeFilePath{};
        std::string stringFilePath{};
        std::string logTypeFilePath{};

        std::vector<std::string> objectTypeData{};
        std::vector<std::string> stringData{};
        std::vector<std::string> logTypeData{};

        std::vector<unsigned int> parsedObjectTypesData{};
        std::vector<std::map<std::string, std::string>> parsedLogTypeData{};
        std::map<unsigned int, std::string> parsedStringData{};

        std::vector<ObjectData> accumulatedData{};
    };
}
