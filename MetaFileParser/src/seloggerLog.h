#pragma once
#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct SeloggerLog
    {
        std::string log;
    };

    class seloggerLogParser : fileReader
    {
    public:
        seloggerLogParser(std::string inputDir_) : dirName(inputDir_),
                                                   filePath(),
                                                   // fileName(),
                                                   data(),
                                                   parsedData(){};

        std::string getDirName()
        {
            return dirName;
        };

        std::vector<std::string> getData()
        {
            this->dirTraversal(this->dirName);
            this->readFile(this->filePath, this->data);
            this->parseReadlines();
            return this->data;
        };

        /**
         * @brief read next stream and store the data to this -> currentBuffer. Please note seloggerLog might contain large data. You should use this fucntion instead of readFile().
         *
         * @return std::string return this-> current bugfer
         */
        std::string readNextStream();

    private:
        void readFile(const std::string fileName, std::vector<std::string> &data) override;
        void parseReadlines() override;
        void dirTraversal(std::string fileName) override;

        /**
         * @brief parseLine() return the SELOGGER_LOG that is interpreted data of given argument. this function is called in void paserLine()
         * @param std::string line. One of the elem of this -> data.
         * @return SELOGGER_LOG
         */

        SeloggerLog parseLine(const std::string line);

        bool isTargetFile(const std::string fileName);

        const std::string fileName = "main_log.txt";
        std::string filePath;
        std::string dirName;

        std::vector<std::string> data;
        std::vector<SeloggerLog> parsedData;
    };
}