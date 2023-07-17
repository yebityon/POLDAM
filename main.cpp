#include <iostream>
#include <chrono>
#include <cassert>

#include "Util/include/src/poldam_util.h"
#include "Util/include/src/poldam_config.hpp"
#include "Util/include/src/cmd.cpp"

// Provide function for Metafile of SELogger
#include "MetaFileParser/src/factory.h"
#include "MetaFileParser/src/metafile_handler_common.h"

// Provide Graph fucntion, Build MarkleTree and Compute Diff Tree of Target Program.
#include "Graph/src/graph.h"

#include <boost/graph/graphviz.hpp>

#define DEBUG_OUT(Var) std::cout << #Var << " " << Var << "\n";

namespace POLDAM
{
    class OmniWriter
    {
    public:
        OmniWriter(POLDAM::PoldamGraph &G_) : G(G_){};

        void writePoldamGraph(const std::string outputFileName)
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
        POLDAM::PoldamGraph &G;
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

POLDAM::PoldamGraph buildGraph(POLDAM::poldamConfig config, const std::string inputDir, const std::string outputFileName)
{
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles\n";
    POLDAM::metafileFactory factory(inputDir);

    // TODO: fileNameをregexpで指定できるようにする
    auto dataids = factory.createInstance<POLDAM::DataIdsParser>("dataids.txt", true);
    auto seloggerParser = factory.createInstance<POLDAM::SeloggerLogParser>("log-00001.txt", "^log-.*.txt");
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
    POLDAM::PoldamGraph PoldamGraph{config};

    for (const POLDAM::SeloggerData log : seloggerParser.getParserdData())
    {
        POLDAM::DataId dataId = parsedDataIds[log.dataId];
        const POLDAM::MethodsData m = parsedMethodsData[dataId.methodId];
        const POLDAM::ClassesData c = parsedClassesData[dataId.classId];

        if (config.isDebugMode)
        {
            std::cout << dataId.raw_value << "\n";
        }

        if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_ENTRY)
        {
            POLDAM::GraphVertex v;
            const unsigned int classId = dataId.classId;

            v.methodStr = m.methodName;
            v.methodHash = std::to_string(std::hash<std::string>()(m.className + m.methodName));
            v.outputFormat = m.className + ":" + m.methodName;

            bool result = PoldamGraph.addOmniVertex(v, log.threadId);
        }
        else if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_PARAM)
        {
            const std::string argValue = log.value;
        }
        else if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::METHOD_NORMAL_EXIT)
        {
            PoldamGraph.computeHash(log.threadId);
            PoldamGraph.popStackVertex(log.threadId);
        }
        else if (dataId.eventType == POLDAM::SELOGGER_EVENT_TYPE::CALL_PARAM)
        {
            // Use these values to compute param hash.
            const unsigned int paramIdx = static_cast<unsigned int>(std::stoi(dataId.attr["index"]));
            const std::string paramType = dataId.attr["type"];
            std::string paramConcreateValue = "";

            if (paramType.find("String") != std::string::npos)
            {
                // see also: https://github.com/takashi-ishio/selogger/blob/924f3d961344bf32cddc93709ed9609f27905191/src/selogger/logging/util/StringContentFile.java#L32
                // fixed for 0-index
                const int argValueIdx = std::stoi(log.value) - 1;
                if (argValueIdx < 0)
                {
                    if (config.isDebugMode)
                    {
                        std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << dataId.raw_value + " is Invalid Data, Skitp to compute hash\n";
                    }
                    continue;
                }
                const POLDAM::ObjectData o = parsedObjectData[argValueIdx];
                PoldamGraph.updateStackTopVertexParamInfo(o.stringValue, log.threadId);
            }
            // branch for paramType that paramValue is directly recored in SELogger.
            else if (paramType.find("Ljava") == std::string::npos)
            {
                // std::cout << "paramType: " << paramType << ",Value: " << log.value << std::endl;
                PoldamGraph.updateStackTopVertexParamInfo(log.value, log.threadId);
            }
            else
            {
                // TODO: Add new condition for Integer
                const int argValueIdx = std::stoi(log.value) - 1;
                if (argValueIdx < 0)
                {
                    if (config.isDebugMode)
                    {
                        std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << dataId.raw_value + " is Invalid Data, Skitp to compute hash\n";
                    }
                    continue;
                }
                const POLDAM::ObjectData o = parsedObjectData[argValueIdx];
            }
        }
        else
        {
            const std::string logString = shapeLogString(dataId, m, c);
            PoldamGraph.updateStackTopVertex(logString, log.threadId);
        }
    }
    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "successfully build PoldamGraph!\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "writing result..." << std::endl;
    if (config.hasEntryMethodName)
    {
        std::cout << "Nekochan\n";
        POLDAM::Graph g = PoldamGraph.getGraphCopy();
        boost::filtered_graph<POLDAM::Graph, boost::keep_all, POLDAM::VertexPredicate> fg(
            g, boost::keep_all(), POLDAM::VertexPredicate(&g));
        std::ofstream outputDotFile(outputFileName);
        boost::write_graphviz(outputDotFile, fg,
                              boost::make_label_writer(get(&POLDAM::GraphVertex::outputFormat, fg)),
                              boost::make_label_writer(get(&POLDAM::GraphEdge::outputFormat, fg)));
    }
    else
    {
        POLDAM::OmniWriter writer(PoldamGraph);
        writer.writePoldamGraph(outputFileName);
    }

    return PoldamGraph;
}

/**
 * ================================= PODLAM MAIN =================================
 */

int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = POLDAM::generateConfig(argc, argv);
    if (config == POLDAM::poldamConfig{})
    {
        // something wrong
        exit(1);
    }
    POLDAM::printConfig(config);
    POLDAM_UTIL::Timer t("main");

    POLDAM::PoldamGraph originGraph = buildGraph(config, config.originDir, config.outputFileName + "_origin.dot");
    if (config.isDebugMode)
    {
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "DEBUG MODE: ONLY PRINT ORIGINAL GRAPH\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Finished." << std::endl;
        return 0;
    }
    POLDAM::PoldamGraph targetGraph = buildGraph(config, config.targetDir, config.outputFileName + "_target.dot");

    // Phase3. Compute Diff Graph
    bool useFlowHash = config.useFlowHash, useParamHash = config.useParamHash;

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

    // Phase4. Write DiffGraph Result
    std::ofstream outputDotFile(config.outputFileName + "_diff.dot");
    boost::write_graphviz(outputDotFile, diffGraph,
                          boost::make_label_writer(get(&POLDAM::GraphVertex::outputFormat, diffGraph)),
                          boost::make_label_writer(get(&POLDAM::GraphEdge::outputFormat, diffGraph)));

    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Finished." << std::endl;
}
