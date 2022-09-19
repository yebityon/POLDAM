#pragma once
#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct MethodsData
    {
        unsigned int classId;
        unsigned int methodId;
        std::string className;
        std::string methodName;
        std::string methodDesc;
        std::string access;
        std::string sourceFileName;
        std::string methodHash;
    };

    class methodDataParser : fileReader
    {
    public:
        methodDataParser(std::string inputDir_, std::string fileName_ = "methods.txt", bool hasHeaderData_ = false) : dirName(inputDir_),
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

        std::vector<MethodsData> getParsedData()
        {
            return this->parsedData;
        };

    private:
        void readFile(const std::string filePath, std::vector<std::string> &data) override;
        void parseReadlines(std::vector<std::string> &data) override;
        void dirTraversal(std::string fileName) override;

        /**
         * @brief delegate function. this function receive string data and paser it to shape Dataid structuo,
         * and push_back to dataid vector.
         *
         * @param line
         */
        void parseLine(const std::string line) override;

        bool isTargetFile(const std::string fileName);

        bool hasHeaderData = false;

        std::vector<std::string> targetFileNames{};
        std::string fileName{};
        std::string filePath{};
        std::string dirName{};
        std::vector<std::string> data{};
        std::vector<MethodsData> parsedData{};
    };
}
