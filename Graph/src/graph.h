#pragma once
#include <iostream>
#include <stack>
#include <functional>
#include <iterator>
// for construct a filterd graph
#include <regex>

#include "../../Util/include/src/poldam_config.hpp"
#include "../../Util/include/src/poldam_util.h"

#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

namespace POLDAM
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
        std::string classStr{};
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
        /**
         *  For the graph view
         */
        bool isTargetVertex = false;
        bool isFilreViewRoot = false;

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
    struct VertexPredicate
    {
        VertexPredicate(){};
        VertexPredicate(Graph const *g) : _graph(g){};
        bool operator()(Graph::vertex_descriptor const &v) const
        {
            return (*_graph)[v].isTargetVertex;
        }

        Graph const *_graph;
    };

    class PoldamGraph
    {
    public:
        POLDAM::poldamConfig config;
        // constructor
        PoldamGraph(const poldamConfig c) : config(c){};
        PoldamGraph() : config(poldamConfig{}){};
        /**
         * @brief Create vertex Descriptor from given Graph Vertex, and Create edge if vStack is not empty.\
         *
         * @param v GraphVertex,
         * @param threadId
         * @return true  return true if succedd to add vertex to graph.
         * @return false return false if failed to add vertex to graph.
         */
        bool addOmniVertex(GraphVertex v, const size_t threadId);

        /**
         * @brief
         *
         * @param log string, add log context to vStack top vertex.
         * @param threadId
         * @return true
         * @return false
         */
        bool updateStackTopVertex(const std::string log, const unsigned int threadId);

        bool updateStackTopVertexParamInfo(const std::string &paramValue, unsigned int threadId);
        bool popStackVertex(const unsigned int threadId);

        bool computeHash(const unsigned int threadId);

        bool isStackEmpty(const unsigned int threadId);

        Graph getGraphCopy();

        boost::graph_traits<Graph>::vertex_descriptor getStackTopVertex(const unsigned int threadId);

        std::vector<boost::graph_traits<Graph>::vertex_descriptor> copyGraphPath()
        {
            return this->path;
        };

        const boost::graph_traits<Graph>::vertex_descriptor getRoot()
        {
            // throw exception if Root has not been initialized.
            assert(this->root.size() > 0);
            return this->Root;
        }

        PoldamGraph computeDiffGraph(PoldamGraph &targetGraph);
        // need to move Graph
        Graph computeDiffGraphBeta(PoldamGraph targetGraph, const std::function<bool(const GraphVertex &, const GraphVertex &)> &isSameVertex);

    private:
        bool computeFlowHash(const unsigned int threadId);
        bool computeParamHash(const unsigned int threadId);
        /**
         * @brief the implementation when vertex Stack is popped. the function describe the relationship between caller and callee.
         *
         * @param threadId the threadId of target log
         * @return true implementation has succesfully finished.
         * @return false implementation has not finished succesfully.
         */
        bool popVertex(const unsigned int threadId);

        /**
         * @brief return true if given vertex descriptor is leaf in graph.
         *
         * @param vDesc boost::graph_traits<Graph>::vertex_descriptor
         * @return true
         * @return false
         */
        bool isLeaf(boost::graph_traits<Graph>::vertex_descriptor vDesc);

        /**
         * @brief return true if given vertex descriptor is leaf in graph.
         *
         * @param vDesc boost::graph_traits<Graph>::vertex_descriptor
         * @return true
         * @return false
         */
        bool pushStackVertex(const boost::graph_traits<Graph>::vertex_descriptor vDesc, const unsigned int threadId);

        /**
         * @brief Create VertexDescriptor. Note this function DO NOT add to graph.\
         * NOTE: this function do not update vStack, Therefore, you need to handle it carefully.
         *
         * @param v
         * @param threadId
         * @return boost::graph_traits<Graph>::vertex_descriptor return vertex_descriptor if succedd to add
         */
        boost::graph_traits<Graph>::vertex_descriptor addOmniVertexDesc(GraphVertex v, const size_t threadId);

        /**
         * @brief Add edge between two given vertices.
         *
         * @param u_
         * @param v_
         * @param threadId
         * @return true
         * @return false
         */
        bool addOmniEdge(
            boost::graph_traits<Graph>::vertex_descriptor u_,
            boost::graph_traits<Graph>::vertex_descriptor v_, const size_t threadId);

        void traverseDiffVertices(
            const Graph &o, const Graph &t,
            const boost::graph_traits<Graph>::vertex_descriptor ovd,
            const boost::graph_traits<Graph>::vertex_descriptor tvd,
            const boost::graph_traits<Graph>::vertex_descriptor parDiffVerDesc,
            const std::function<bool(const GraphVertex &, const GraphVertex &)> &isSameVertex

        );

        unsigned int incrementCounter ()
        {
            return this->counter++;
        };
        
        Graph g{};
        Graph diffGraph{};
        unsigned int counter = 0;
        
        boost::graph_traits<Graph>::vertex_descriptor Root;

        std::map<unsigned int, boost::graph_traits<Graph>::vertex_descriptor> root{};

        std::vector<boost::graph_traits<Graph>::vertex_descriptor> path;
        // stack for vertexies

        // {key: thareadId, value:stack<>}
        std::map<unsigned int, std::stack<boost::graph_traits<Graph>::vertex_descriptor>> vStack;
        // {key: threadId, value:stack<>}
        std::map<unsigned int, std::stack<std::map<std::string, std::string>>> callStack;
        // {key: threadId, value:stack<>}
        std::map<unsigned int, std::stack<std::map<std::string, std::string>>> callInstStack;
    };
}
