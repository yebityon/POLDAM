#pragma once
#include "metafileHandlerCommon.h"
#include "dataids.h"

namespace POLDAM
{
    class metafileFactory
    {
    public:
        metafileFactory(std::string dirPath_) : dirPath(dirPath_){};
        template <typename T, typename... Args>
        std::vector<T> createInstance(Args &&...args);

    private:
        const std::string dirPath;
    };
}