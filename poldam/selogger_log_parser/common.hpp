#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <utility>
#include <map>
#include <cassert>

#include <poldam/util/utility.hpp>
#include <poldam/selogger_log_parser/event_type.hpp>


namespace POLDAM
{

    class FileReader
    {
    private:
        /**
         * @brief open the fileName and read data. this->data has these data. Please NOTE: This Strategy might be not efficient, you need to hold raw string data and transforemed data. It consumes 2 * sizeof(filedata).
         * @param std:;stri
         * @return void.
         */
        virtual void readFile(const std::string &fileName, std::vector<std::string> &data) = 0;

        /**
         * @brief parse and iterate this -> data.
         *
         */
        virtual void parseReadlines(std::vector<std::string> &data) = 0;

        /**
         * @brief delegate function. this fucntion will be called in parseReadlines function, and access to this method.
         */
        virtual void parseLine(const std::string &line) = 0;

        /**
         * @brief iterate target Directory to look up target file. Please note there may be more than one target file.
         * @param targetDir
         */
        virtual void dirTraversal(const std::string &targetDir) = 0;
    };

}