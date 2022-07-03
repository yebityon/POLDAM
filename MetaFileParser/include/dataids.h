#pragma once
#include "metafileHandlerCommon.h"

namespace POLDAM
{

    class dataidsParser : fileReader
    {
    public:
        dataidsParser(std::string fileName_) : fileName(fileName_){};

    private:
        void radFile() override;
        void parseReadlines() override;
        void parseLine() override;

        void const std::string fileName;

        std::vector<std::string> data;
    }
}