#pragma once
#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"
#include "../../Interpreter/src/InterpreterCommon.h"
#include <map>

// TODO: Interface should be speciallized for each struct type.

namespace POLDAM
{
    struct DataId
    {
        unsigned int dataidx;
        unsigned int classid;
        unsigned int methodid;
        // -1 is undefined or N/A
        int linenumber;
        int ordernumber;
        std::string eventtype;
        std::string descriptor;
        std::map<std::string, std::string> eventinfo;
    };

    class dataidsParser : fileReader
    {
    public:
        dataidsParser(std::string inputDir_) : dirName(inputDir_),
                                               filePath(),
                                               // fileName(),
                                               data(),
                                               parsedData(),
                                               targetFileNames(){};

        std::vector<std::string> getData()
        {
            this->dirTraversal(this->dirName);
            this->readFile(this->filePath, this->data);

            this->parseReadlines();
            return this->data;
        };
        std::string getDirName()
        {
            return dirName;
        };

        std::vector<DataId> getParsedData()
        {
            return this->parsedData;
        }

        bool setTargetFileName(const std::string targetFileName)
        {
            this->targetFileNames.push_back(targetFileName);
        };

    private:
        void readFile(const std::string filePath, std::vector<std::string> &data) override;
        void parseReadlines() override;
        void dirTraversal(std::string fileName) override;

        /**
         * @brief parseLine() return the DataIds that is interpreted data of given argument.this function is called in void parseReadLines().
         * @param std::string line. One of the elem of this -> data.
         * @return DataId
         */
        DataId parseLine(const std::string line);

        bool isTargetFile(const std::string fileName);

        std::vector<std::string> targetFileNames;
        const std::string fileName = "dataids.txt";
        std::string filePath;
        std::string dirName;

        std::vector<std::string> data;
        std::vector<DataId> parsedData;
    };
}
