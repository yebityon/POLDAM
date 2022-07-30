#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <utility>

namespace POLDAM
{

    class fileReader
    {
    private:
        /**
         * @brief open the fileName and read data. this->data has these data
         * @param std:;stri
         * @return void.
         */
        virtual void readFile(const std::string fileName, std::vector<std::string> &data) = 0;

        /**
         * @brief parse and iterate this -> data.
         *
         */
        virtual void parseReadlines() = 0;

        /**
         * @brief iterate target Directory to look up target file. Please note there may be more than one target file.
         * @param targetDir
         */
        virtual void dirTraversal(std::string targetDir) = 0;
    };

}