#include "classes.h"

namespace POLDAM
{
    void ClassesDataParser::readFile(const std::string filePath, std::vector<std::string> &data)
    {
        std::ifstream fileStream;

        fileStream.open(filePath, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "classesData file is not found\n";
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

    void ClassesDataParser::parseReadlines(std::vector<std::string> &data)
    {
        std::vector<ClassesData> parsedData;
        for (const auto &str : data)
        {
            this->parseLine(str);
        }
    }

    void ClassesDataParser::parseLine(std::string line)
    {
        ClassesData m;
        const auto parsedline = POLDAM_UTIL::parse(line);

        assert(parsedline.size() == 7);

        m.classId = static_cast<unsigned int>(std::stoi(parsedline[0]));
        m.loadedFrom = parsedline[1];
        m.fileName = parsedline[2];
        m.className = parsedline[3];
        m.logLevel = parsedline[4];
        m.classHash = parsedline[5];
        m.classLoaderId = parsedline[6];

        this->parsedData.push_back(m);
    }

    // TODO: move this fucntion to base class, all you need to do is just desginate fileName in base class.
    void ClassesDataParser::dirTraversal(std::string dirName)
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
