#pragma once
#include <iostream>
#include <chrono>
#include <cassert>

#include "Util/include/src/poldamUtil.h"
#include "Util/include/src/poldamConfig.h"

#include "Interpreter/src/InterpreterCommon.h"
#include "Interpreter/src/methodEntry.h"

#include "MetafileParser/src/factory.h"
#include "Metafileparser/src/metafileHandlerCommon.h"

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

    EventType getEventType(const std::string str)
    {
        const std::string evnetString = POLDAM_UTIL::split(str, ',')[1];
        const std::string eventType = POLDAM_UTIL::split(evnetString, '=')[1];

        if (eventType == "METHOD_ENTRY")
        {
            return EventType::METHOD_ENTRY;
        }
        else if (eventType == "METHOD_PARAM")
        {
            return EventType::METHOD_PARAM;
        }
        else if (eventType == "METHOD_NORMAL_EXIT")
        {
            return EventType::METHOD_NORMAL_EXIT;
        }
        else if (eventType == "METHOD_EXCEPTIONAL_EXIT")
        {
            return EventType::METHOD_EXCEPTIONAL_EXIT;
        }
        else
        {
            return EventType::UNDEFINED;
        }
    }
}

int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = {};

    if (argc <= 1)
    {
        std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "You need to disignate input directory." << std::endl;
        exit(1);
    }

    for (unsigned int i = 1; i < argc; ++i)
    {

        const std::string arg = argv[i];
        if (arg == "-d" or arg == "--outputDir")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Gieven\n";
                printHelp();
                exit(1);
            }
            const std::string outputDir = argv[i + 1];
            config.outputDir = outputDir;
            ++i;
        }
        else if (arg == "-s" or arg == "--inputDir")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Given\n";
                printHelp();
                exit(1);
            }
            const std::string inputDir = argv[i + 1];
            config.inputDir = inputDir;
            ++i;
        }
        else if (arg == "-t" or arg == "--targetMethod")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No targetMethod is Given\n";
                printHelp();
                exit(1);
            }
            const std::string targetMethod = argv[i + 1];
            config.targetMethodName = targetMethod;
            ++i;
        }
        else if (arg == "-outputFile")
        {
            if (i + 1 > argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No outputfile is Given\n";
                printHelp();
                exit(1);
            }
            const std::string outputFileName = argv[i + 1];
            config.outputFileName = outputFileName;
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
        else
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "Unknown Option\n";
            printHelp();
            exit(1);
        }
    }

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "PODLAM IS WORKING\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "outputDir: {" << config.outputDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "inputDir: {" << config.inputDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetMethod: {" << config.targetMethodName << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "outputFileName: {" << config.outputFileName << "}\n";

    // Phase 1. read and parse all metafiles
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles\n";
    POLDAM::metafileFactory factory(config.inputDir);

    auto dataids = factory.createInstance<POLDAM::dataidsParser>(config.inputDir);
    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(config.inputDir);
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>(config.inputDir);

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully reading Metafiles\n";
    // Phase2. Create Graph.
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Create Graph\n";
    const std::vector<std::string> omniLog = seloggerParser.getData();

    POLDAM::OmniGraph targetGraph{};

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "==================== Iterate Log ====================\n";

    for (const std::string log : omniLog)
    {
        if (DEBUG)
        {
            printf("%s\n", log.c_str());
        }
        const POLDAM::EventType eventId = POLDAM::getEventType(log);

        if (eventId == POLDAM::EventType::METHOD_ENTRY)
        {
            POLDAM::LogInterpreter<POLDAM::METHOD_ENTRY> interpreter(log);
            interpreter.parseLog();

            POLDAM::METHOD_ENTRY res = interpreter.getParserResult();
            POLDAM::GraphVertex v;

            v.outputFormat = res.methodName;

            const size_t threadId = 0;
            const auto &vertex = targetGraph.createOmniVertex(v, threadId);
            bool isInserted = targetGraph.addOmniEdge(vertex, threadId);
        }
        else if (eventId == POLDAM::EventType::METHOD_PARAM)
        {
            // POLDAM::LogInterpreter< POLDAM::METHOD_PARAM> interpreter(log);
            // interpreter.parseLog();
        }
        else if (eventId == POLDAM::EventType::METHOD_NORMAL_EXIT)
        {
            const unsigned int threadId = 0;
            // computeFlowHash should consider the control flow hash
            targetGraph.computeFlowHash(threadId);
            // same as FlowHash
            targetGraph.computeParamHash(threadId);

            targetGraph.popVertex(threadId);
        }
        else
        {
            const unsigned int threadId = 0;
            targetGraph.updateStackTopVertex(log, threadId);
        }
    }

    // Phase3. Apply algorrithms　and Compare two Graphs.

    // Phase4. Write Graph Result.

    POLDAM::OmniWriter writer(targetGraph);
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "writing result..." << std::endl;
    writer.writeOmniGraph(config.outputDir);

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Finished...!!" << std::endl;
}
