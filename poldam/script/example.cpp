#include <poldam/util/arg_config.hpp>

#include <poldam/selogger_log_parser/common.hpp>
#include <poldam/selogger_log_parser/factory.hpp>
#include <poldam/selogger_log_parser/dataids.hpp>

#include <poldam/helper/graph_builder.hpp>
#include <poldam/helper/poldamGraph_writer.hpp>

#include <functional>

int main(int argc, char *argv[])
{
    // configに用意されたフラグによって、GraphBuilderの出力が変わる
    POLDAM::poldamConfig config = POLDAM::generateConfig(argc, argv);
    
    if (config == POLDAM::poldamConfig{})
    {
        exit(1);
    }
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Configuration\n";
    POLDAM::printConfig(config);

    POLDAM_UTIL::Timer t("Total Time");
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Reading Metafiles...";
    
    // SELoggerから作成されたファイルを読み込む
    POLDAM::metafileFactory factory(config.originDir);
    auto dataids = factory.createInstance<POLDAM::DataIdsParser>("dataids.txt", true);
    auto seloggerParser = factory.createInstance<POLDAM::SeloggerLogParser>("log-00001.txt", "^log-.*.txt");
    auto objectFileParser = factory.createInstance<POLDAM::ObjectfileParser>();
    auto methodParser = factory.createInstance<POLDAM::MethodDataParser>("methods.txt", true);
    auto classesParser = factory.createInstance<POLDAM::ClassesDataParser>("classes.txt", true);
    std::cout << "OK\n";

    // 実際にログからグラフを構築するBuilder, configと合わせてログを構築する
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Constructing Builder...";
    POLDAM::GraphBuilder builder(config,
                                 seloggerParser, dataids, objectFileParser, methodParser, classesParser);
    std::cout << "OK\n";

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Parsing Logs...";
    builder.parseLogs();
    std::cout << "OK\n";

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Constructing Markle Tree...";
    auto g = builder.build();
    std::cout << "OK\n";

    POLDAM::OmniWriter writer(g);
    writer.writePoldamGraph("sample_origin.dot");
    if (config.hasEntryMethodName)
    {
        writer.writePoldamGraph<POLDAM::VertexPredicate>("sample_filterd_output.dot");
    }
}
