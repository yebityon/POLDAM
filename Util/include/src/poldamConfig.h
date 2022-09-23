#pragma once
#include <iostream>
#include <string>

namespace POLDAM
{

    struct poldamConfig
    {

        std::string originDir{};
        std::string targetDir{};
        std::string targetMethodName{};
        std::string outputFileName{};

        bool useFastIO = false;
        bool isDebugMode = false;
        bool useFlowHash = false;
        bool usePramHash = false;
    };

}
