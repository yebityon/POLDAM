#include "seloggerLog.h"

namespace POLDAM
{

    void seloggerLogParser::readFile()
    {
        std::ifstream fileStream;
        std::vector<std::string> data;

        fileStream.open(this->filePath, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "selogger log is not found\n";
            exit(1);
        }

        while (not fileStream.eof())
        {
            std::string buffer;
            std::getline(fileStream, buffer);

            // if you want to hold interpreted data, call parseLine()
            //  buffer = this.parseLine(buffer);
            //  otherwise, this class hold all data
            data.push_back(buffer);
        }

        // TODO: check whether you need to move this file,
        this->data = std::move(data);
    }

    void seloggerLogParser::parseReadlines()
    {
        for (const auto &buffer : this->data)
        {
            this->parsedData.push_back(this->parseLine(buffer));
        }
    }

    SELLOGER_LOG seloggerLogParser::parseLine(const std::string line)
    {
        SELLOGER_LOG rec{};
        return rec;
    }
    // TODO: move this fucntion to base class, all you need to do is just desginate fileName in base class.
    void seloggerLogParser::dirTraversal(const std::string dirName)
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

}
