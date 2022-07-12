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
        bool createInstance(T *&outputPtr, Args &&...args);

        template <typename T, typename... Args>
        T createInstance(Args &&...args)
        {
            return T(std::forward<Args>(args)...);
        }

    private:
        const std::string dirPath;
    };
}