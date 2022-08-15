#pragma once
#include "graph.hpp"

namespace POLDAM
{
    bool OmniGraph::addOmniEdge(boost::graph_traits<Graph>::edge_descriptor e, GraphVertex u, GraphVertex v, const size_t threadId)
    {
        return true;
    }

    boost::graph_traits<Graph>::vertex_descriptor OmniGraph::createOmniVertex(GraphVertex v_, const size_t threadId)
    {
        boost::graph_traits<Graph>::vertex_descriptor v = boost::add_vertex(this->g);
        this->g[v] = v_;
        this->vStack[threadId].push(v);

        return v;
    }

    bool OmniGraph::addOmniEdge(boost::graph_traits<Graph>::edge_descriptor e, boost::graph_traits<Graph>::vertex_descriptor v_, const size_t threadId)
    {
        if (this->vStack[threadId].empty())
        {
            return false;
        }
        const auto &prevVertex = vStack[threadId].top();

        bool isInserted = boost::add_edge(prevVertex, v_, this->g);
    }

}
