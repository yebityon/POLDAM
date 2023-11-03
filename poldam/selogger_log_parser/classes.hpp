#pragma once

#include <poldam/selogger_log_parser/common.hpp>
#include <poldam/util/utility.hpp>

namespace POLDAM
{
    struct ClassesData
    {
        unsigned int classId;
        std::string loadedFrom;
        std::string fileName;
        std::string className;
        std::string logLevel;
        std::string classHash;
        std::string classLoaderId;
    };

    class ClassesDataParser : FileReader
    {
    public:
        ClassesDataParser(const std::string &inputDir_, const std::string &fileName_ = "methods.txt", bool hasHeaderData_ = false)
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

        std::vector<ClassesData> getParsedData()
        {
            return this->parsedData;
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
        std::vector<ClassesData> parsedData{};
    };
}
