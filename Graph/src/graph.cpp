#pragma once
#include "graph.hpp"

namespace POLDAM
{
    bool OmniGraph::addOmniEdge(GraphVertex u, GraphVertex v, const size_t threadId)
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

    bool OmniGraph::addOmniEdge(boost::graph_traits<Graph>::vertex_descriptor v_, const size_t threadId)
    {
        boost::graph_traits<Graph>::edge_descriptor e;

        if (this->vStack[threadId].empty())
        {
            return false;
        }
        const auto &prevVertex = vStack[threadId].top();

        bool isInserted = false;
        boost::tie(e, isInserted) = boost::add_edge(prevVertex, v_, this->g);

        return isInserted;
    }
    bool OmniGraph::updateStackTopVertex(const std::string log, const unsigned int threadId)
    {
        const auto &prevVertex = this->vStack[threadId].top();
        this->g[prevVertex].flowStr += log;
        return true;
    }
    bool OmniGraph::computeFlowHash(const unsigned int threadId)
    {
        const auto &prevVertex = this->vStack[threadId].top();
        this->g[prevVertex].flowHash = std::hash<std::string>()(this->g[prevVertex].flowStr);
        return true;
    }
    bool OmniGraph::computeParamHash(const unsigned int threadId)
    {
        const auto &prevVertex = this->vStack[threadId].top();
        this->g[prevVertex].paramHash = std::hash<std::string>()(this->g[prevVertex].paramStr);
        return true;
    }
}
