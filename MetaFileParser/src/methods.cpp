#include "methods.h"

namespace POLDAM
{
    void methodDataParser::readFile(const std::string filePath, std::vector<std::string> &data)
    {
        std::ifstream fileStream;

        fileStream.open(filePath, std::ios::in);

        if (!fileStream)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "methodData file is not found\n";
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

    void methodDataParser::parseReadlines(std::vector<std::string> &data)
    {
        std::vector<MethodsData> parsedData;
        for (const auto &str : data)
        {
            this->parseLine(str);
        }
    }

    void methodDataParser::parseLine(std::string line)
    {
        MethodsData m;
        const auto parsedline = POLDAM_UTIL::parse(line);
        assert(parsedline.size() == 8);

        m.classId = static_cast<unsigned int>(std::stoi(parsedline[0]));
        m.methodId = static_cast<unsigned int>(std::stoi(parsedline[1]));
        m.className = parsedline[2];
        m.methodName = parsedline[3];
        m.methodDesc = parsedline[4];
        m.access = parsedline[5];
        m.sourceFileName = parsedline[6];
        m.methodHash = parsedline[7];

        this->parsedData.push_back(m);
    }

    // TODO: move this fucntion to base class, all you need to do is just desginate fileName in base class.
    void methodDataParser::dirTraversal(std::string dirName)
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