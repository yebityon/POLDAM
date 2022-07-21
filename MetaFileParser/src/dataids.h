#pragma once
#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

// TODO: Interface should be speciallized for each struct type.

namespace POLDAM
{
    struct DATAIDS
    {
        std::string hogehogemaru;
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
            this->readFile();
            this->parseReadlines();
            return this->data;
        };
        std::string getDirName()
        {
            return dirName;
        };

        bool setTargetFileName(const std::string targetFileName)
        {
            this->targetFileNames.push_back(targetFileName);
        };

    private:
        void readFile() override;
        void parseReadlines() override;
        void dirTraversal(std::string fileName) override;

        /**
         * @brief parseLine() return the DATAIDS that is interpreted data of given argument.this function is called in void parseReadLines().
         * @param std::string line. One of the elem of this -> data.
         * @return DATAIDS
         */
        DATAIDS parseLine(const std::string line);

        bool isTargetFile(const std::string fileName);

        std::vector<std::string> targetFileNames;
        const std::string fileName = "dataids.txt";
        std::string filePath;
        std::string dirName;

        std::vector<std::string> data;
        std::vector<DATAIDS> parsedData;
    };
}
