#include "factory.h"

namespace POLDAM
{
    // TODO: typename T should be base of readFileClass
    template <typename T, typename... Args>
    std::vector<T> metafileFactory::createInstance(Args &&...args)
    {
        try
        {
            return T(std::forward<Args>(args)...)
        }
        catch
        {
            // TODO:  handle the error
        }
        return
    }
}