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

        const std::string dirName;
    };
}