#pragma once
#include <poldam/selogger_log_parser/common.hpp>
#include <poldam/selogger_log_parser/dataids.hpp>
#include <poldam/selogger_log_parser/classes.hpp>
#include <poldam/selogger_log_parser/methods.hpp>
#include <poldam/selogger_log_parser/object.hpp>
#include <poldam/selogger_log_parser/log.hpp>

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