#include "selogger_log.h"

namespace POLDAM
{
    void
    SeloggerLogParser::readFile(const std::string &filePath, std::vector<std::string> &data)
    {
        // std::ifstream fileStream;

        // fileStream.open(filePath, std::ios::in);

        // if (!fileStream)
        // {
        //     std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "selogger log is not found\n";
        //     exit(1);
        // }

        // while (not fileStream.eof())
        // {
        //     std::string buffer;
        //     std::getline(fileStream, buffer);

        //     // if you want to hold interpreted data, call parseLine()
        //     //  buffer = this.parseLine(buffer);
        //     //  otherwise, this class hold all data
        //     if (buffer.empty())
        //         continue;
        //     data.push_back(buffer);
        // }
        // fileStream.close();

        // ugly, need to fix
        for (const std::string &filePath : this->filePaths)
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
    }

    void SeloggerLogParser::parseReadlines(std::vector<std::string> &data)
    {
        for (const auto &buffer : this->data)
        {
            parseLine(buffer);
        }
    }

    void SeloggerLogParser::parseLine(const std::string &line)
    {
        SeloggerData rec{};
        const std::vector<std::string> parsedLog = POLDAM_UTIL::split(line, ',');

        assert(parsedLog.size() == 4);

        rec.eventId = static_cast<unsigned int>(std::stoi(parsedLog[0]));
        rec.dataId = static_cast<unsigned int>(std::stoi(parsedLog[1]));
        rec.threadId = static_cast<unsigned int>(std::stoi(parsedLog[2]));
        rec.value = parsedLog[3];

        this->parsedData.push_back(rec);
    }
    // TODO: move this fucntion to base class, all you need to do is just desginate fileName in base class.
    void SeloggerLogParser::dirTraversal(const std::string &dirName)
    {
        std::regex re(this->filePattern);

        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(dirName))
        {
            const auto filePath = i.path();
            // SeloggerLogParse use regexp instead of fileName
            if (std::regex_match(i.path().filename().string(), re))
            {
                this->filePaths.emplace_back(i.path().c_str());
            }
        }
        std::sort(filePaths.begin(), filePaths.end());
    }

}
