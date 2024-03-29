#pragma once
#include <iostream>
#include <string>
#include <regex>

namespace POLDAM
{

    struct poldamConfig
    {

        std::string originDir{};
        std::string targetDir{};
        std::string outputFileName{};
        std::string entryClassName{};
        std::string entryMethodName{};
        std::string filterdVertexRegexStr{};
        std::string filterdhashRegexStr{};

        std::regex filterdVertexRegex{};
        std::regex filterdhashRegex{};

        bool useFastIO = false;
        bool isDebugMode = false;
        bool useFlowHash = false;
        bool useParamHash = false;
        bool hasEntryClassName = false;
        bool hasEntryMethodName = false;
        bool hasFilterdRegex = false;
        bool isFilterdHash = false;

        bool operator==(const poldamConfig &c)
        {
            return (
                c.originDir == originDir &&
                c.targetDir == originDir &&
                c.outputFileName == outputFileName &&
                c.entryMethodName == entryMethodName &&
                c.useFastIO == useFastIO &&
                c.filterdVertexRegexStr == filterdVertexRegexStr &&
                c.isDebugMode == isDebugMode &&
                c.useFlowHash == useFlowHash &&
                c.useParamHash == useParamHash &&
                c.hasEntryClassName == hasEntryClassName &&
                c.hasEntryMethodName == hasEntryMethodName &&
                c.hasFilterdRegex == hasFilterdRegex
                );
        }

        bool setFilterdRegExp(const std::string &regexStr)
        {
            try
            {
                filterdVertexRegex = std::regex(regexStr);
            }
            catch (const std::regex &error)
            {
                return false;
            }
            filterdVertexRegexStr = regexStr;
            hasFilterdRegex = true;
            return true;
        }

        bool setFilterdHashRegExp(const std::string &regexStr)
        {
            try
            {
                filterdhashRegex = std::regex(regexStr);
            }
            catch (const std::regex &error)
            {
                return false;
            }
            filterdhashRegexStr = regexStr;
            isFilterdHash = true;
            return true;
        }
    };

    poldamConfig generateConfig(int argc, char *argv[]);
    void printConfig(const poldamConfig &config);
} // namespace
