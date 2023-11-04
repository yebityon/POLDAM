#include <poldam/util/arg_config.hpp>

#include <poldam/selogger_log_parser/common.hpp>
#include <poldam/selogger_log_parser/factory.hpp>
#include <poldam/selogger_log_parser/dataids.hpp>

#include <poldam/helper/graph_builder.hpp>
#include <poldam/helper/poldamGraph_writer.hpp>

#include <functional>

int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = POLDAM::generateConfig(argc, argv);
    
    if (config == POLDAM::poldamConfig{})
    {
        exit(1);
    }
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Configuration\n";
    POLDAM::printConfig(config);

    POLDAM_UTIL::Timer t("main");
    
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles\n";
    POLDAM::metafileFactory factory(config.originDir);
    auto dataids = factory.createInstance<POLDAM::DataIdsParser>("dataids.txt", true);
    auto seloggerParser = factory.createInstance<POLDAM::SeloggerLogParser>("log-00001.txt", "^log-.*.txt");
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>();
    auto methodParser = factory.createInstance<POLDAM::MethodDataParser>("methods.txt", true);
    auto classesParser = factory.createInstance<POLDAM::ClassesDataParser>("classes.txt", true);
    std::cout<< POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully reading Metafiles\n";
    
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Constructing Builder...";
    POLDAM::GraphBuilder builder(config, 
    seloggerParser, dataids, objectFileParser, methodParser, classesParser);
    std::cout <<  "OK\n";
    
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Parsing Logs...";
    builder.parseLogs();
    std::cout << "OK\n";
    
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Constructing Markle Tree...";
    auto g = builder.build();
    std::cout << "OK\n";
    
    POLDAM::OmniWriter writer(g);
    writer.writePoldamGraph("sample_origin.dot");
    if(config.hasEntryMethodName)
    {
        // FIXME: getGraphCopy is not cost effective, 
        // using filterd_graph_type = boost::filtered_graph<POLDAM::Graph, boost::keep_all, POLDAM::VertexPredicate>;
        // const auto filter_proc = [&](POLDAM::Graph g) -> filterd_graph_type {
        //     boost::filtered_graph<POLDAM::Graph, boost::keep_all, POLDAM::VertexPredicate> fg(
        //         g, boost::keep_all(), POLDAM::VertexPredicate(&g));
        //         return fg;
        // };
        // writer.writePoldamGraph<decltype(filter_proc)>("sample_filterd_output.dot", filter_proc);
        auto gg = g.getGraphCopy();
        boost::filtered_graph<POLDAM::Graph, boost::keep_all, POLDAM::VertexPredicate> fg(
            gg, boost::keep_all(), POLDAM::VertexPredicate(&gg));
        std::ofstream outputDotFile("sample_filted_output.dot");
        boost::write_graphviz(outputDotFile, fg,
                              boost::make_label_writer(get(&POLDAM::GraphVertex::outputFormat, fg)),
                              boost::make_label_writer(get(&POLDAM::GraphEdge::outputFormat, fg)));
    }    
}
