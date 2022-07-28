#pragma once
#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct SELLOGER_LOG
    {
        std::string log;
    };

    class seloggerLogParser : fileReader
    {
    public:
        seloggerLogParser(std::string inputDir_){};

        std::string getDirNam()
        {
            return dirName;
        };
        /**
         * @brief read next stream and store the data to this -> currentBuffer. Please note seloggerLog might contain large data. You should use this fucntion instead of readFile().
         *
         * @return std::string return this-> current bugfer
         */
        std::string readNextStream();

    private:
        void readFile() override;
        void parseReadlines() override;
        void dirTraversal(std::string fileName) override;

        /**
         * @brief parseLine() return the SELOGGER_LOG that is interpreted data of given argument. this function is called in void paserLine()
         * @param std::string line. One of the elem of this -> data.
         * @return SELOGGER_LOG
         */

        SELLOGER_LOG parseLine(const std::string line);

        bool isTargetFile(const std::string fileName);

        const std::string fileName = "log.txt";
        std::string filePath;
        std::string dirName;

        std::vector<std::string> data;
        std::vector<SELLOGER_LOG> parsedData;
        };
}