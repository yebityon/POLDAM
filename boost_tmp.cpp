#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graphviz.hpp>

#include <bits/stdc++.h>

using namespace std;

int main()
{
    struct GraphEdge
    {
    public:
        int cost;
        std::string edgeHash;
        std::string from;
        std::string to;
        std::string outputFormat;
    };

    struct GraphVertex
    {
    public:
        // method hash is the hash of method
        std::string methodStr{};
        std::string methodHash{};

        // param str has param value, the hash value of object
        std::string paramStr{};
        std::size_t paramHash{};

        // flow hash is the flow of inside of the method
        std::string flowStr{};
        std::size_t flowHash{};

        // control flow hash is the method-call-chain of the hash

        std::size_t childFlowHash{};
        std::size_t childParamHash{};

        std::string controlFlowStr{};
        std::size_t controlFlowHash{};

        std::string controlParamStr{};
        std::size_t controlParamHash{};

        /*
         * context hash is the flow hash of the parent hash.
         */
        std::size_t contextHash{};

        std::string outputFormat{};

        size_t id{}, edgesSize{};
        std::vector<std::string> paramList = {};
    };
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                        GraphVertex,
                                        GraphEdge>;

    Graph g;

    std::vector<boost::graph_traits<Graph>::vertex_descriptor> vSet;

    for (int i = 0; i < 10; ++i)
    {
        boost::graph_traits<Graph>::vertex_descriptor v = boost::add_vertex(g);
        g[v].outputFormat = g[v].methodStr = ::to_string(i);
        vSet.push_back(v);
    }
    for (int id = 0; id * 2 + 1 < vSet.size(); ++id)
    {
        boost::graph_traits<Graph>::vertex_descriptor v = vSet[id];
        boost::graph_traits<Graph>::edge_descriptor e;
        bool isInserted = false;
        boost::tie(e, isInserted) = boost::add_edge(vSet[id], vSet[id * 2 + 1], g);
        if (vSet.size() > id * 2 + 2)
            boost::tie(e, isInserted) = boost::add_edge(vSet[id], vSet[id * 2 + 2], g);
    }

    try
    {
        std::ofstream outputDotFile("tmp_boost.dot");
        boost::write_graphviz(outputDotFile, g,
                              boost::make_label_writer(get(&GraphVertex::outputFormat, g)),
                              boost::make_label_writer(get(&GraphEdge::outputFormat, g)));
    }
    catch (...)
    {
        std::cerr << "Can not export as graphviz." << std::endl;
        return 0;
    }

    for (boost::graph_traits<Graph>::vertex_descriptor v : vSet)
    {
        Graph::out_edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = out_edges(v, g); ei != ei_end; ++ei)
        {
            std::cout << "=================== " + g[v].methodStr << " ==================\n";
            std::cout << g[v].methodStr << " -> " << g[boost::target(*ei, g)].methodStr << std::endl;
            std::cout
                << "===================\n";
        }
    }
    return 0;
}
