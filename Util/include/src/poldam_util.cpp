#include "poldam_util.h"
namespace POLDAM_UTIL
{
    std::vector<std::string> split(const std::string &str, const char &terminator)
    {
        std::vector<std::string> rec{};
        std::string buffer{};

        for (auto c : str)
        {
            if (c == terminator)
            {
                rec.push_back(buffer);
                std::string().swap(buffer);
            }
            else
            {
                buffer.push_back(c);
            }
        }

        if (not buffer.empty())
        {
            rec.push_back(buffer);
        }

        return rec;
    }

}