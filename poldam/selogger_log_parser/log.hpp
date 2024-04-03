#pragma once
#include <regex>
#include <algorithm>
#include <poldam/selogger_log_parser/common.hpp>

namespace POLDAM
{

    struct SeloggerData
    {
        unsigned int eventId;
        unsigned int dataId;
        std::string value;
        unsigned int threadId;
    };

    class SeloggerLogParser : FileReader
    {
    public:
        SeloggerLogParser(const std::string &inputDir_, const std::string &fileName_, const std::string &filePattern_ = "") : dirName(inputDir_),
                                                                                                                              filePath(),
                                                                                                                              fileName(fileName_),
                                                                                                                              data(),
                                                                                                                              filePattern(filePattern_),
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
        void readFile(const std::string &fileName, std::vector<std::string> &data) override;
        void parseReadlines(std::vector<std::string> &data) override;
        void dirTraversal(const std::string &filePattern) override;

        /**
         * @brief parseLine() return the SELOGGER_LOG that is interpreted data of given argument. this function is called in void paserLine()
         * @param std::string line. One of the elem of this -> data.
         * @return SELOGGER_LOG
         */
        void parseLine(const std::string &line) override;

        bool isTargetFile(const std::string fileName);

        // TODO: string fileName should be disgnated from out of this file.
        const std::string fileName{};
        bool hasHeaderData = true;
        std::string filePath;
        std::vector<std::string> filePaths;
        std::string filePattern;
        std::string dirName;
        std::vector<std::string> data;
        std::vector<SeloggerData> parsedData;
    };
}