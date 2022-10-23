#include <iostream>
#include <chrono>
#include <cassert>

#include "Util/include/src/poldam_util.h"
#include "Util/include/src/poldam_config.h"

// Provide function for Metafile of SELogger
#include "MetafileParser/src/factory.h"
#include "Metafileparser/src/metafile_handler_common.h"

// Provide Graph fucntion, Build MarkleTree and Compute Diff Tree of Target Program.
#include "Graph/src/graph.h"

#include <boost/graph/graphviz.hpp>

bool DEBUG = true;

void printHelp()
{
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Following Options are available now.\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-o : selogger_outputDir,Directory containing selogger output of the program before modification.\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-t : selogger_outputDir,Directory containing selogger output of the program after modification.\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-m : method that you want to observe, default is the first executed method.\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "--debug : enable debug output.\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "--flow, --param : evaluate the equivalence of method execution using the given hash. \n";
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
                          << "Can not export to graphviz." << std::endl;
                return;
            }

            return;
        }

    private:
        POLDAM::OmniGraph &G;
    };

}

std::string shapeLogString(const POLDAM::DataId d, const POLDAM::MethodsData m, const POLDAM::ClassesData c)
{
    std::string res;
    res += std::to_string(static_cast<int>(d.eventType)) + d.valueDesc;
    for (const auto itr : d.attr)
    {
        res += itr.first + itr.second;
    }

    res += m.className + m.methodName + m.methodDesc;
    res += c.className + c.fileName;
    return res;
}

POLDAM::OmniGraph buildGraph(POLDAM::poldamConfig config, const std::string inputDir, const std::string outputFileName)
{
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles\n";
    POLDAM::metafileFactory factory(inputDir);

    auto dataids = factory.createInstance<POLDAM::DataIdsParser>("dataids.txt", true);
    auto seloggerParser = factory.createInstance<POLDAM::SeloggerLogParser>("log-00001.txt");
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>();
    auto methodParser = factory.createInstance<POLDAM::MethodDataParser>("methods.txt", true);
    auto classesParser = factory.createInstance<POLDAM::ClassesDataParser>("classes.txt", true);

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
    POLDAM::OmniGraph omniGraph{};

    for (const POLDAM::SeloggerData log : seloggerParser.getParserdData())
    {
        POLDAM::DataId dataId = parsedDataIds[log.dataId];
        const POLDAM::MethodsData m = parsedMethodsData[dataId.methodId];
        const POLDAM::ClassesData c = parsedClassesData[dataId.classId];

        if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_ENTRY)
        {
            POLDAM::GraphVertex v;
            const unsigned int classId = dataId.classId;

            v.methodStr = m.methodName;
            v.methodHash = std::to_string(std::hash<std::string>()(m.className + m.methodName));
            v.outputFormat = m.methodName;

            bool result = omniGraph.addOmniVertex(v, log.threadId);
        }
        else if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_PARAM)
        {
            const std::string argValue = log.value;
        }
        else if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_NORMAL_EXIT)
        {
            omniGraph.computeHash(log.threadId);
            omniGraph.popStackVertex(log.threadId);
        }
        else if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::CALL_PARAM)
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
                omniGraph.updateStackTopVertexParamInfo(o.stringValue, log.threadId);
            }
            // branch for paramType that paramValue is directly recored in SELogger.
            else if (paramType.find("Ljava") == std::string::npos)
            {
                // std::cout << "paramType: " << paramType << ",Value: " << log.value << std::endl;
                omniGraph.updateStackTopVertexParamInfo(log.value, log.threadId);
            }
            else
            {
                const POLDAM::ObjectData o = parsedObjectData[std::stoi(log.value) - 1];
                // std::cout << "paramType: " << paramType << ",Value: " << o.objectType << std::endl;
            }
        }
        else
        {
            const std::string logString = shapeLogString(dataId, m, c);
            omniGraph.updateStackTopVertex(logString, log.threadId);
        }
    }

    POLDAM::OmniWriter writer(omniGraph);
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "writing result..." << std::endl;
    writer.writeOmniGraph(outputFileName);

    return omniGraph;
}

/**
 * ================================= PODLAM MAIN =================================
 */

int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = {};
    POLDAM_UTIL::Timer t("main");

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
        else if (arg == "--flow")
        {
            config.useFlowHash = true;
        }
        else if (arg == "--param")
        {
            config.usePramHash = true;
        }
        else
        {

            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "Unknown Option.\n";
            std::cout << argv[i] << " is not valid option.\n";
            printHelp();
            exit(1);
        }
    }

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "POLDAM IS WORKING\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "originDir: {" << config.originDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetDir: {" << config.targetDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetMethod: {" << config.targetMethodName << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "outputFileName: {" << config.outputFileName << "}\n";

    POLDAM::OmniGraph originGraph = buildGraph(config, config.originDir, config.outputFileName + "_origin.dot");
    POLDAM::OmniGraph targetGraph = buildGraph(config, config.targetDir, config.outputFileName + "_target.dot");

    // Phase3. Compute Diff Graph
    bool useFlowHash = config.useFlowHash, useParamHash = config.usePramHash;

    const auto isSameVertex = [useFlowHash, useParamHash](const POLDAM::GraphVertex &v, const POLDAM::GraphVertex &u) -> bool
    {
        if (useFlowHash && useParamHash)
        {
            return v.controlFlowHash == u.controlFlowHash && v.controlParamHash == u.controlParamHash;
        }
        else if (useParamHash)
        {
            return v.controlParamHash == u.controlParamHash;
        }
        else
        {
            return v.controlFlowHash == u.controlFlowHash;
        }
        return true;
    };

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Computhing DiffGraph..." << std::endl;
    POLDAM::Graph diffGraph = originGraph.computeDiffGraphBeta(std::move(targetGraph), isSameVertex);

    // Phase4. Write DiffGraph Resulf
    std::ofstream outputDotFile(config.outputFileName + "_diff.dot");
    boost::write_graphviz(outputDotFile, diffGraph,
                          boost::make_label_writer(get(&POLDAM::GraphVertex::outputFormat, diffGraph)),
                          boost::make_label_writer(get(&POLDAM::GraphEdge::outputFormat, diffGraph)));

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Finished." << std::endl;
}
