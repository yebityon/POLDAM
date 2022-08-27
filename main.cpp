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

void printHelp()
{
    std::cout << "Check your code" << std::endl;
}

class OmniWriter
{
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

    // Phase 1. read and parse all metafiles
    POLDAM::metafileFactory factory(config.inputDir);

    auto dataids = factory.createInstance<POLDAM::dataidsParser>(config.inputDir);
    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(config.inputDir);
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>(config.inputDir);

    // Phase2. Create Graph.

    const std::vector<std::string> omniLog = seloggerParser.getData();

    POLDAM::OmniGraph targetGraph{};

    for (const std::string &log : omniLog)
    {
        const POLDAM::EventType eventId = POLDAM::getEventType(log);

        if (eventId == POLDAM::EventType::METHOD_ENTRY)
        {
            POLDAM::LogInterpreter<POLDAM::METHOD_ENTRY> interpreter(log);
            interpreter.parseLog();

            POLDAM::METHOD_ENTRY res = interpreter.getParserResult();
            POLDAM::GraphVertex v;
            // update  v infor
            const size_t threadId = 1;

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
        }
        else
        {
            const unsigned int threadId = 0;
            targetGraph.updateStackTopVertex(log, threadId);
        }
    }

    // Phase3. Apply algorrithms　and Compare two Graphs.

    // Phase4. Write Graph Result.

    OmniWriter writer(targetGraph);
    writer.writeOmniGraph();

    std::cout
        << "===================== TEST PASSED ====================" << std::endl;
}
