#include <stdio.h>
#include <stack>
#include <map>

#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace POLDAM
{
    struct GraphEdge
    {
    public:
        int cost;
        std::string edgeHash;
        std::string from;
        std::string to;
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

    class OmniGraph
    {
    public:
        bool addOmniEdge(GraphVertex u, GraphVertex v);
        boost::graph_traits<Graph>::vertex_descriptor createOmniVertex(){};
        bool updateVertex(GraphVertex v, const std::string context);
        bool pushVertex(){};

    private:
        Graph g{};
        boost::graph_traits<Graph>::vertex_descriptor root;

        // stack for vertexies

        // {key: thareadId, value:stack<>}
        std::map<int, std::stack<Graph::vertex_descriptor>> vStack;
        // {key: threadId, value:stack<>}
        std::map<int, std::stack<std::map<std::string, std::string>>> callStack;
        // {key: threadId, value:stack<>}
        std::map<int, std::stack<std::map<std::string, std::string>>> callInstStack;
    };
}
