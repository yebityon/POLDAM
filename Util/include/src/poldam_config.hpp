#pragma once
#include <iostream>
#include <string>

namespace POLDAM
{

    struct poldamConfig
    {

        std::string originDir{};
        std::string targetDir{};
        std::string outputFileName{};
        std::string entryMethodName{};

        bool useFastIO = false;
        bool isDebugMode = false;
        bool useFlowHash = false;
        bool useParamHash = false;
        bool hasEntryMethodName = false;

        bool operator==(const poldamConfig &c)
        {
            return (
                c.originDir == originDir &&
                c.targetDir == originDir &&
                c.outputFileName == outputFileName &&
                c.entryMethodName == entryMethodName &&
                c.useFastIO == useFastIO &&
                c.isDebugMode == isDebugMode &&
                c.useFlowHash == useFlowHash &&
                c.useParamHash == useParamHash &&
                c.hasEntryMethodName == hasEntryMethodName);
        }
    };

}
