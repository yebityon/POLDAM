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
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "outputDir: {" << config.outputDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "inputDir: {" << config.inputDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetMethod: {" << config.targetMethodName << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "outputFileName: {" << config.outputFileName << "}\n";

    // Phase 1. read and parse all metafiles
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles\n";
    POLDAM::metafileFactory factory(config.inputDir);

    auto dataids = factory.createInstance<POLDAM::dataidsParser>(config.inputDir, "dataids.txt", true);
    auto seloggerParser = factory.createInstance<POLDAM::seloggerLogParser>(config.inputDir, "log-00001.txt");
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>(config.inputDir);
    auto methodParser = factory.createInstance<POLDAM::methodDataParser>(config.inputDir, "methods.txt", true);
    auto classesParser = factory.createInstance<POLDAM::classesDataParser>(config.inputDir, "classes.txt", true);

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

    for (const POLDAM::SeloggerData log : seloggerParser.getParserdData())
    {

        POLDAM::DataId dataId = parsedDataIds[log.dataid];
        const POLDAM::MethodsData m = parsedMethodsData[dataId.methodid];
        const POLDAM::ClassesData c = parsedClassesData[dataId.classid];
        const POLDAM::ObjectData o = parsedObjectData[log.value];

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
            const int argValue = log.value;
        }
        else if (dataId.eventtype == "METHOD_NORMAL_EXIT")
        {
            targetGraph.computeHash(log.threadid);
            targetGraph.popStackVertex(log.threadid);
        }
        else if (dataId.eventtype == "CALL_PARAM")
        {
            const unsigned int paramIdx = static_cast<unsigned int>(std::stoi(dataId.attr["index"]));
            const std::string paramType = dataId.attr["type"];
            std::string paramConcreateValue = "";

            if (paramType.find("String") != std::string::npos)
            {
                std::cout << "paramType: " << paramType << " Value: " << o.value << std::endl;
            }
            // branch for paramType that paramValue is directly recored in SELogger.
            else if (paramType.find("Interger") != std::string::npos or
                     paramType.find("int") != std::string::npos)
            {
                std::cout << "paramType: " << paramType << " Value: " << log.value << std::endl;
            }
            else
            {
                std::cout << "paramType: " << paramType << " Value: " << o.objecttype << std::endl;
            }
        }
        else
        {
            const std::string logString = dataidsData[log.dataid] + classesData[dataId.classid] + methodData[dataId.methodid];
            targetGraph.updateStackTopVertex(logString, log.threadid);
        }
    }

    // Phase3. Apply algorrithms　and Compare two Graphs.

    // Phase4. Write Graph Result.

    POLDAM::OmniWriter writer(targetGraph);
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "writing result..." << std::endl;
    writer.writeOmniGraph(config.outputFileName);

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Finished...!!" << std::endl;
}
