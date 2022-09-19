#pragma once
#include <iostream>
#include <chrono>
#include <cassert>

#include "Util/include/src/poldamUtil.h"
#include "Util/include/src/poldamConfig.h"

// Provide function for Metafile of SELogger
#include "MetafileParser/src/factory.h"
#include "Metafileparser/src/metafileHandlerCommon.h"

// Provide Graph fucntion, Build MarkleTree and Compute Diff Tree of Target Program.
#include "Graph/src/graph.hpp"

#include <boost/graph/graphviz.hpp>

bool DEBUG = true;

void printHelp()
{
    std::cout << "Check your code" << std::endl;
}

namespace POLDAM
{
    class OmniWriter
    {
    public:
        OmniWriter(POLDAM::OmniGraph &G_) : G(G_){};

        void writeOmniGraph(const std::string outputFileName)
        {
            const auto &g = G.getGraphCopy();

            try
            {
                std::ofstream outputDotFile(outputFileName);
                boost::write_graphviz(outputDotFile, g,
                                      boost::make_label_writer(get(&POLDAM::GraphVertex::outputFormat, g)),
                                      boost::make_label_writer(get(&POLDAM::GraphEdge::outputFormat, g)));
            }
            catch (...)
            {
                std::cerr << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX
                          << "Can not export as graphviz." << std::endl;
                return;
            }

            return;
        }

    private:
        POLDAM::OmniGraph &G;
    };

}

void buildGraph(POLDAM::poldamConfig config, const std::string inputDir, const std::string outputFileName)
{
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles\n";
    POLDAM::metafileFactory factory(inputDir);

    auto dataids = factory.createInstance<POLDAM::dataidsParser>(inputDir, "dataids.txt", true);
    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(inputDir, "log-00001.txt");
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>(inputDir);
    auto methodParser = factory.createInstance<POLDAM::methodDataParser>(inputDir, "methods.txt", true);
    auto classesParser = factory.createInstance<POLDAM::classesDataParser>(inputDir, "classes.txt", true);

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully reading Metafiles\n";
    // Phase2. Create Graph.
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Create Graph\n";

    const std::vector<std::string> omniLog = seloggerParser.getData();

    const std::vector<std::string> dataidsData = dataids.getData();
    const std::vector<POLDAM::DataId> parsedDataIds = dataids.getParsedData();

    const std::vector<std::string> methodData = methodParser.getData();
    const std::vector<POLDAM::MethodsData> parsedMethodsData = methodParser.getParsedData();

    const std::vector<std::string> classesData = classesParser.getData();
    const std::vector<POLDAM::ClassesData> parsedClassesData = classesParser.getParsedData();

    const std::vector<POLDAM::ObjectData> parsedObjectData = objectFileParser.getParsedData();

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Parsed\n";
    POLDAM::OmniGraph targetGraph{};

    int id = 0;
    for (const POLDAM::SeloggerData log : seloggerParser.getParserdData())
    {
        //  std::cout << id << std::endl;
        POLDAM::DataId dataId = parsedDataIds[log.dataid];
        const POLDAM::MethodsData m = parsedMethodsData[dataId.methodid];
        const POLDAM::ClassesData c = parsedClassesData[dataId.classid];

        if (dataId.eventtype == "METHOD_ENTRY")
        {
            POLDAM::GraphVertex v;
            const unsigned int classId = dataId.classid;

            v.methodStr = m.methodName;
            v.methodHash = m.methodHash;
            v.outputFormat = m.methodName;

            bool result = targetGraph.addOmniVertex(v, log.threadid);
        }
        else if (dataId.eventtype == "METHOD_PARAM")
        {
            const std::string argValue = log.value;
        }
        else if (dataId.eventtype == "METHOD_NORMAL_EXIT")
        {
            targetGraph.computeHash(log.threadid);
            targetGraph.popStackVertex(log.threadid);
        }
        else if (dataId.eventtype == "CALL_PARAM")
        {
            // Use these values to compute param hash.
            const unsigned int paramIdx = static_cast<unsigned int>(std::stoi(dataId.attr["index"]));
            const std::string paramType = dataId.attr["type"];
            std::string paramConcreateValue = "";

            if (paramType.find("String") != std::string::npos)
            {
                // fixed for 0-index
                const int argValueIdx = std::stoi(log.value) - 1;
                const POLDAM::ObjectData o = parsedObjectData[argValueIdx];
                std::cout << "paramType: " << paramType << ",Value: " << o.stringValue << std::endl;
            }
            // branch for paramType that paramValue is directly recored in SELogger.
            // NO
            else if (paramType.find("Integer") != std::string::npos or
                     paramType.find("int") != std::string::npos or paramType == "I")
            {

                std::cout << "paramType: " << paramType << ",Value: " << log.value << std::endl;
            }
            else
            {
                // const POLDAM::ObjectData o = parsedObjectData[log.value - 1];
                // std::cout << "paramType: " << paramType << ",Value: " << o.objectType << std::endl;
            }
        }
        else
        {
            const std::string logString = dataidsData[log.dataid] /*+ classesData[dataId.classid]*/ + methodData[dataId.methodid];
            targetGraph.updateStackTopVertex(logString, log.threadid);
        }
        ++id;
    }

    // Phase3. Apply algorrithms　and Compare two Graphs.

    // Phase4. Write Graph Result.

    POLDAM::OmniWriter writer(targetGraph);
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "writing result..." << std::endl;
    writer.writeOmniGraph(outputFileName);
}
int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = {};

    if (argc <= 1)
    {
        std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "You need to designate input directory." << std::endl;
        exit(1);
    }

    for (unsigned int i = 1; i < argc; ++i)
    {

        const std::string arg = argv[i];
        if (arg == "-o" or arg == "--originDir")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Gieven\n";
                printHelp();
                exit(1);
            }
            config.originDir = argv[i + 1];
            ++i;
        }
        else if (arg == "-t" or arg == "--targetDir")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Given\n";
                printHelp();
                exit(1);
            }
            config.targetDir = argv[i + 1];
            ++i;
        }
        else if (arg == "-m" or arg == "--targetMethod")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No targetMethod is Given\n";
                printHelp();
                exit(1);
            }
            config.targetMethodName = argv[i + 1];
            ++i;
        }
        else if (arg == "-out" or arg == "--outFileName")
        {
            if (i + 1 > argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No outputfile is Given\n";
                printHelp();
                exit(1);
            }
            config.outputFileName = argv[i + 1];
            ++i;
        }
        else if (arg == "--fastIO")
        {
            std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Use fast IO\n"
                      << std::endl;
            std::cin.tie(nullptr);
            std::ios_base::sync_with_stdio(false);
            config.useFastIO = true;
        }
        else if (arg == "--debug")
        {
            config.isDebugMode = true;
        }
        else
        {

            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "Unknown Option\n";
            std::cout << argv[i] << " is Not  Valid Option\n";
            printHelp();
            exit(1);
        }
    }

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "POLDAM IS WORKING\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "originDir: {" << config.originDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetDir: {" << config.targetDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetMethod: {" << config.targetMethodName << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "outputFileName: {" << config.outputFileName << "}\n";

    buildGraph(config, config.originDir, config.outputFileName + "_origin.dot");
    buildGraph(config, config.targetDir, config.outputFileName + "_target.dot");

    // Phase 1. read and parse all metafiles

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Finished." << std::endl;
}
