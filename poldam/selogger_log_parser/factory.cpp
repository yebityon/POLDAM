#include <poldam/selogger_log_parser/factory.hpp>

namespace POLDAM
{
    // TODO: typename T should be base of readFileClass
    template <typename T, typename... Args>
    bool metafileFactory::createInstance(T *&outputPtr, Args &&...args)
    {
        try
        {
            outputPtr = T(std::forward<Args>(args)...);

            return true;
        }
        catch (void *err)
        {
            // TODO:  handle the error
        }
        return false;
    }
}