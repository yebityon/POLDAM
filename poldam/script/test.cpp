#include <poldam/util/arg_config.hpp>

#include <poldam/selogger_log_parser/common.hpp>
#include <poldam/selogger_log_parser/factory.hpp>
#include <poldam/selogger_log_parser/dataids.hpp>

#include <poldam/helper/graph_builder.hpp>
#include <poldam/helper/poldamGraph_writer.hpp>

int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = POLDAM::generateConfig(argc, argv);
    
    if (config == POLDAM::poldamConfig{})
    {
        exit(1);
    }
    POLDAM_UTIL::Timer t("main");
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles\n";
    POLDAM::metafileFactory factory(config.originDir);
    auto dataids = factory.createInstance<POLDAM::DataIdsParser>("dataids.txt", true);
    auto seloggerParser = factory.createInstance<POLDAM::SeloggerLogParser>("log-00001.txt", "^log-.*.txt");
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>();
    auto methodParser = factory.createInstance<POLDAM::MethodDataParser>("methods.txt", true);
    auto classesParser = factory.createInstance<POLDAM::ClassesDataParser>("classes.txt", true);
    std::cout
        << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Successfully reading Metafiles\n";
    
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Constructing Builder...\n";
    POLDAM::GraphBuilder builder(config, 
    seloggerParser, dataids, objectFileParser, methodParser, classesParser);
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Builder is Ready\n";
    
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Parsing Logs...\n";
    builder.parseLogs();
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "OK\n";
    
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Constructing Markle Tree...\n";
    auto g = builder.build();
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "OK...\n";
    
    POLDAM::OmniWriter writer(g);
    writer.writePoldamGraph("test.dot");
    
}
