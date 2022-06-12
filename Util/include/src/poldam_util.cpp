#pragma once

#include "poldam_util.h"

namespace POLDAM_UTIL
{

    std::vector<std::string> parse(std::string str, const char terminator)
    {
        std::vector<std::string> res;

        std::string tmp;

        for (int i = 0; i < str.size(); ++i)
        {
            if (str[i] == terminator)
            {
                res.push_back(tmp);
                // clear buffer
                std::string().swap(tmp);
            }
            else
            {
                tmp.push_back(terminator);
            }
        }
        if (not tmp.empty())
            res.push_back(tmp);

        return res;
    }
}