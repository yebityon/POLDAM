#pragma once

#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct ObjectData
    {
        // recored in  LOG$ObjectTypes
        unsigned int dataidx;
        unsigned int objectid;
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
        ObjectfileParser(std::string inputDir){};

        std::vector<ObjectData> getParsedData()
        {
            return this->accumulatedData;
        }

    private:
        void readFile(const std::string filePath, std::vector<std::string> &data) override;
        void parseReadlines(const std::vector<std::string> &data) override;
        void dirTraversal(std::string fileName) override;

        void setObjectfileType(const std::string fileType)
        {
            this->fileType = fileType;
        };

        void parseLine(const std::string line) override;
        void parseLogLine(const std::string line);
        void parseStringLine(const std::string line);

        void accumulateObjectFile();
        void readObjectTypeData();

        std::pair<int, int> parseObjectData(const std::string line);

        std::string fileType = "object";

        std::string objectFileName{"LOG$ObjectTypes00001.txt"};
        std::string stringFileName{"LOG$String00001.txt"};
        std::string logTypeFileName{"LOG$Types.txt"};

        std::string objectTypeFilePath;
        std::string stringFilePath;
        std::string logTypeFilePath;

        std::vector<std::string> objectTypeData;
        std::vector<std::string> stringData;
        std::vector<std::string> logTypeData;

        std::vector<unsigned int> parsedObjectTypesData;
        std::vector<std::map<std::string, std::string>> parsedLogTypeData;
        std::map<unsigned int, std::string> parsedStringData;

        std::vector<ObjectData> accumulatedData;
    };
}
