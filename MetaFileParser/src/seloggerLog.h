#pragma once
#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{

    struct SeloggerData
    {
        unsigned int eventid;
        unsigned int dataid;
        std::string value;
        unsigned int threadid;
    };

    class seloggerLogParser : fileReader
    {
    public:
        seloggerLogParser(std::string inputDir_, std::string fileName_) : dirName(inputDir_),
                                                                          filePath(),
                                                                          fileName(fileName_),
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
            this->parseReadlines(this->data);
            return this->data;
        };

        std::vector<SeloggerData> getParserdData()
        {
            return this->parsedData;
        }

        /**
         * @brief read next stream and store the data to this -> currentBuffer. Please note seloggerLog might contain large data. You should use this fucntion instead of readFile().
         *
         * @return std::string return this-> current bugfer
         */
        std::string readNextStream();

    private:
        void readFile(const std::string fileName, std::vector<std::string> &data) override;
        void parseReadlines(std::vector<std::string> &data) override;
        void dirTraversal(std::string fileName) override;

        /**
         * @brief parseLine() return the SELOGGER_LOG that is interpreted data of given argument. this function is called in void paserLine()
         * @param std::string line. One of the elem of this -> data.
         * @return SELOGGER_LOG
         */
        void parseLine(const std::string line) override;

        bool isTargetFile(const std::string fileName);

        // TODO: string fileName should be disgnated from out of this file.
        const std::string fileName{};
        std::string filePath;
        std::string dirName;
        std::vector<std::string> data;
        std::vector<SeloggerData> parsedData;
    };
}