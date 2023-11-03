#include <poldam/util/utility.hpp>
#include <poldam/util/arg_config.hpp>

int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = POLDAM::generateConfig(argc, argv);
    if (config == POLDAM::poldamConfig{})
    {
        // something wrong
        exit(1);
    }
    POLDAM_UTIL::Timer t("main");

    // POLDAM::PoldamGraph originGraph = buildGraph(config, config.originDir, config.outputFileName + "_origin.dot");
    // if (config.isDebugMode)
    // {
    //     std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "DEBUG MODE: ONLY PRINT ORIGINAL GRAPH\n";
    //     std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully Finished." << std::endl;
    //     return 0;
    // }
    // POLDAM::PoldamGraph targetGraph = buildGraph(config, config.targetDir, config.outputFileName + "_target.dot");

    // // Phase3. Compute Diff Graph
    // bool useFlowHash = config.useFlowHash, useParamHash = config.useParamHash;

    // const auto isSameVertex = [useFlowHash, useParamHash](const POLDAM::GraphVertex &v, const POLDAM::GraphVertex &u) -> bool
    // {
    //     if (useFlowHash && useParamHash)
    //     {
    //         return v.controlFlowHash == u.controlFlowHash && v.controlParamHash == u.controlParamHash;
    //     }
    //     else if (useParamHash)
    //     {
    //         return v.controlParamHash == u.controlParamHash;
    //     }
    //     else
    //     {
    //         return v.controlFlowHash == u.controlFlowHash;
    //     }
    //     return true;
    // };

    // std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Computhing DiffGraph..." << std::endl;
    // POLDAM::Graph diffGraph = originGraph.computeDiffGraphBeta(std::move(targetGraph), isSameVertex);

    // // Phase4. Write DiffGraph Result
    // std::ofstream outputDotFile(config.outputFileName + "_diff.dot");
    // boost::write_graphviz(outputDotFile, diffGraph,
    //                       boost::make_label_writer(get(&POLDAM::GraphVertex::outputFormat, diffGraph)),
    //                       boost::make_label_writer(get(&POLDAM::GraphEdge::outputFormat, diffGraph)));

    // std::cout
    //     << POLDAM_UTIL::POLDAM_PRINT_SUFFIX
    //     << "\033[1m\033[32m"
    //     << "Successfully Finished."
    //     << "\033[0m"
    //     << std::endl;
}
