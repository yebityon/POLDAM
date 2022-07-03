#pragma once

#include "poldamUtil.h"

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
                tmp.push_back(str[i]);
            }
        }
        if (not tmp.empty())
            res.push_back(tmp);

        return res;
    }

    std::vector<std::string> split(std::string str, const char terminator)
    {
        std::vector<std::string> rec{};
        std::string buffer;

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