#pragma once
#include <poldam/selogger_log_parser/common.hpp>

namespace POLDAM
{
    struct DataId
    {
        unsigned int dataIdx;
        unsigned int classId;
        unsigned int methodId;
        // -1 is undefined or N/A
        int line;
        int instructionId;
        POLDAM::SELOGGER_EVENT_TYPE eventType;
        std::string valueDesc;
        std::map<std::string, std::string> attr;
        std::string raw_value;
    };
    class DataIdsParser : FileReader
    {
    public:
        DataIdsParser(const std::string &inputDir_, const std::string &fileName_ = "dataids.txt", const bool &hasHeaderData_ = false)
            : dirName(inputDir_),
              filePath(),
              fileName(fileName_),
              hasHeaderData(hasHeaderData_),
              data(),
              parsedData(),
              targetFileNames(){};


        std::vector<std::string> getData()
        {
            this->dirTraversal(this->dirName);
            this->readFile(this->filePath, this->data);

            this->parseReadlines(this->data);
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
            return true;
        };

    private:
        void readFile(const std::string &filePath, std::vector<std::string> &data) override;
        void parseReadlines(std::vector<std::string> &data) override;
        void dirTraversal(const std::string &dirName) override;

        /**
         * @brief delegate function. this function receive string data and paser it to shape Dataid structuo,
         * and push_back to dataid vector.
         *
         * @param line
         */
        void parseLine(const std::string &line) override;

        bool isTargetFile(const std::string fileName);

        bool hasHeaderData = true;

        std::vector<std::string> targetFileNames{};
        std::string fileName{};
        std::string filePath{};
        std::string dirName{};
        std::vector<std::string> data{};
        std::vector<DataId> parsedData{};
    };
}
