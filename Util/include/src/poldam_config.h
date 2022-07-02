#pragma once
#include <iostream>
#include <string>

namespace POLDAM
{

    struct poldamConfig
    {

        std::string inputDir = "";
        std::string outputDir = "";
        std::string targetMethodName = "";
        bool useFastIO = false;
    };

}
