#pragma once
#include "metafileHandlerCommon.h"
#include <Util/include/src/poldamUtil.h>

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
        dataidsParser(std::string fileName_) : fileName(fileName_){};
        void getParsedData();

    private:
        void readFile() override;
        void parseReadlines() override;
        void parseLine() override;

        DATAIDS parseLine(const std::string str)
        {
            DATAIDS rec{};
            return rec;
        }

        const std::string fileName;

        std::vector<std::string> data;
        std::vector<DATAIDS> parsedData;
    };
}
