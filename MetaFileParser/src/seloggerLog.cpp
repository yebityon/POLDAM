#include "seloggerLog.h"

namespace POLDAM
{
    void
    seloggerLogParser::readFile(const std::string filePath, std::vector<std::string> &data)
    {
        std::ifstream fileStream;

        fileStream.open(filePath, std::ios::in);

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
            if (buffer.empty())
                continue;
            data.push_back(buffer);
        }
        fileStream.close();
    }

    void seloggerLogParser::parseReadlines(std::vector<std::string> &data)
    {
        for (const auto &buffer : this->data)
        {
            parseLine(buffer);
        }
    }

    void seloggerLogParser::parseLine(const std::string line)
    {
        SeloggerData rec{};
        const std::vector<std::string> parsedLog = POLDAM_UTIL::parse(line, ',');

        assert(parsedLog.size() == 4);

        rec.eventId = static_cast<unsigned int>(std::stoi(parsedLog[0]));
        rec.dataId = static_cast<unsigned int>(std::stoi(parsedLog[1]));
        rec.threadId = static_cast<unsigned int>(std::stoi(parsedLog[2]));
        rec.value = parsedLog[3];

        this->parsedData.push_back(rec);
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
