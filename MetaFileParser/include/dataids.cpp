#include "dataids.h"

namespace POLDAM
{
    void dataidsParser::readFile()
    {
        std::ifstream fileStream;
        std::vector<std::string> data;

        fileStream.open(this->filePath, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "dataids file is not found\n";
            exit(1);
        }

        while (not fileStream.eof())
        {
            std::string buffer;
            std::getline(fileStream, buffer);

            // if you want to hold interpreted data, call parseLine()
            // buffer = this.parseLine(buffer);
            data.push_back(buffer);
        }

        this->data = std::move(data);
    }

    void dataidsParser::parseReadlines()
    {
        std::vector<DATAIDS> parsedData;
        for (const auto &str : this->data)
        {
            parsedData.push_back(this->parseLine(str));
        }
        this->parsedData = std::move(parsedData);
    }

    DATAIDS dataidsParser::parseLine(const std::string line)
    {
        DATAIDS rec{};
        return rec;
    }

    void dataidsParser::dirTraversal(std::string dirName)
    {
        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(dirName))
        {
            const auto filePath = i.path();
            if (filePath.filename() == this->fileName)
            {
                this->filePath = std::string(i.path().c_str());
            }
        }
    };

}
