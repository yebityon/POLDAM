#pragma once
#include "metafileHandlerCommon.h"
#include "dataids.h"
#include "seloggerLog.h"
#include "objectFile.h"
#include "methods.h"
#include "classes.h"

namespace POLDAM
{
    // metafileFactory should be static class
    class metafileFactory
    {
    public:
        metafileFactory(std::string dirPath_) : dirPath(dirPath_){};
        template <typename T, typename... Args>
        bool createInstance(T *&outputPtr, Args &&...args);

        template <typename T, typename... Args>
        T createInstance(Args &&...args)
        {
            return T(dirPath, std::forward<Args>(args)...);
        }

        std::string getDirPath()
        {
            return dirPath;
        }

    private:
        const std::string dirPath;
    };
}