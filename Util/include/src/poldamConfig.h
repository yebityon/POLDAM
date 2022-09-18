#pragma once
#include <iostream>
#include <string>

namespace POLDAM
{

    struct poldamConfig
    {

        std::string inputDir{};
        std::string outputDir{};
        std::string targetMethodName{};
        std::string outputFileName{};

        bool useFastIO = false;
        bool isDebugMode = false;
    };

}
