#include <poldam/util/arg_config.hpp>
#include <poldam/selogger_log_parser/common.hpp>
#include <poldam/selogger_log_parser/factory.hpp>
#include <poldam/graph/graph.hpp>
#include <boost/graph/graphviz.hpp>

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

        template <typename F>
        void writePoldamGraph(const std::string outputFileName, F& filterd_proc)
        {
            
            try
            {
                const auto& g = G.getGraphCopy();
                auto filteredGraph = filterd_proc(g);   
                std::ofstream outputDotFile(outputFileName);
                boost::write_graphviz(outputDotFile, filteredGraph,
                                      boost::make_label_writer(get(&POLDAM::GraphVertex::outputFormat, filteredGraph)),
                                      boost::make_label_writer(get(&POLDAM::GraphEdge::outputFormat, filteredGraph)));
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
