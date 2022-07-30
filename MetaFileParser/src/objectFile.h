#pragma once

#include "metafileHandlerCommon.h"
#include "../../Util/include/src/poldamUtil.h"

namespace POLDAM
{
    struct ObjectData
    {
        unsigned int dataidx;
    };

    class ObjectfileParser : fileReader
    {
    public:
        ObjectfileParser(std::string inputDir){};

        std::vector<std::string> getData()
        {
            return this->data;
        };

    private:
        std::vector<std::string> data;
    };
}