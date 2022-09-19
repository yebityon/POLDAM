#pragma once

#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct ObjectData
    {
        // recored in  LOG$ObjectTypes
        unsigned int fileIdx_; // internal use only
        unsigned int objectId;
        // typenum is recored in LOG$Types.txt
        std::string objectType;
        std::string jarFile;
        int typenum1;
        int typenum2;
        std::string loadFrom;
        //  if type is sstring, concreate value is available
        std::string stringValue;
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
            return this->objectDatas;
        }

    private:
        void readFile(const std::string filePath, std::vector<std::string> &data) override;
        void parseReadlines(std::vector<std::string> &data) override;
        void dirTraversal(std::string fileName) override;

        void setObjectfileType(std::string fileType)
        {
            this->fileType = fileType;
        };

        void parseLine(std::string line) override;
        void parseLogTypesLine(std::string line);
        void parseStringLine(std::string line);

        void accumulateObjectFile();
        /**
         * @brief raed Object metafile. (i.e., objectTypesData, StringData and  Types Data)
         *
         */
        void readObjectTypeData();

        std::pair<int, int> parseObjectData(const std::string line);

        std::string fileType{};

        std::string ojbectTypesFileName{"LOG$ObjectTypes00001.txt"};
        std::string stringFileName{"LOG$String00001.txt"};
        std::string typesFileName{"LOG$Types.txt"};

        std::string objectTypesFilePath{};
        std::string stringFilePath{};
        std::string typesFilePath{};

        std::vector<std::string> objectTypesDatas{};
        std::vector<std::string> stringDatas{};
        std::vector<std::string> typesDatas{};

        std::vector<unsigned int> parsedObjectTypesDatas{};
        std::map<unsigned int, std::string> parsedStringDatas{};
        std::vector<std::map<std::string, std::string>> parsedTypesDatas{};

        std::vector<ObjectData> objectDatas{};
    };
}
