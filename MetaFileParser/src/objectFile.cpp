#pragma once
#include "objectFile.h"

namespace POLDAM
{

    void ObjectfileParser::dirTraversal(const std::string dirName)
    {
        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(dirName))
        {
            const auto filePath = i.path();

            if (filePath.filename() == this->fileName)
            {
                this->filePath = std::string(i.path().c_str());
            }
        }
    }

    void ObjectfileParser::AccumulateObjectFile()
    {
    }

}
