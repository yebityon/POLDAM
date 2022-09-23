#include "dataids.h"

namespace POLDAM
{
    void dataidsParser::readFile(const std::string filePath, std::vector<std::string> &data)
    {
        std::ifstream fileStream;

        fileStream.open(filePath, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "dataids file is not found\n";
            exit(1);
        }

        while (not fileStream.eof())
        {
            std::string buffer;
            std::getline(fileStream, buffer);

            if (buffer.empty())
                continue;
            // if you want to hold interpreted data, call parseLine()
            // buffer = this.parseLine(buffer);
            this->data.push_back(buffer);
        }

        if (this->hasHeaderData)
        {
            this->data.erase(this->data.begin());
        }
    }

    void dataidsParser::parseReadlines(std::vector<std::string> &data)
    {
        std::vector<DataId> parsedData;
        for (const auto &str : data)
        {
            this->parseLine(str);
        }
    }

    void dataidsParser::parseLine(std::string line)
    {
        DataId dataid;

        const auto &&parsedline = POLDAM_UTIL::parse(line);

        dataid.dataIdx = static_cast<unsigned int>(std::stoi(parsedline[0]));
        dataid.classId = static_cast<unsigned int>(std::stoi(parsedline[1]));
        dataid.methodId = static_cast<unsigned int>(std::stoi(parsedline[2]));
        dataid.line = std::stoi(parsedline[3]);
        dataid.instructionId = std::stoi(parsedline[4]);
        dataid.eventType = POLDAM::getEventType(parsedline[5]);
        dataid.valueDesc = parsedline[6];

        for (int i = 7; i < parsedline.size(); ++i)
        {
            if (parsedline[i].find("=") == std::string::npos)
            {
                // if attribute does not contain '='
                std::string fixedOne = parsedline[i];

                if (fixedOne.front() == '\"')
                {
                    fixedOne.erase(fixedOne.begin());
                }
                if (fixedOne.back() == '\"')
                {
                    fixedOne.pop_back();
                }
                dataid.attr[fixedOne] = fixedOne;
            }
            else
            {
                std::vector<std::string> vec = POLDAM_UTIL::split(parsedline[i], '=');
                if (vec[0].front() == '\"')
                {
                    vec[0].erase(vec[0].begin());
                }

                if (vec[1].back() == '\"')
                {
                    vec[1].pop_back();
                }

                dataid.attr[vec[0]] = vec[1];
            }
        }
        this->parsedData.push_back(dataid);
    }

    // TODO: move this fucntion to base class, all you need to do is just desginate fileName in base class.
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
