#pragma once
#include "graph.hpp"
#include <iostream>

namespace POLDAM
{

    bool OmniGraph::addOmniVertex(GraphVertex v_, const size_t threadId)
    {
        boost::graph_traits<Graph>::vertex_descriptor v = boost::add_vertex(this->g);

        this->g[v] = v_;
        if (not this->vStack[threadId].empty())
        {
            const auto &prev = this->vStack[threadId].top();
            OmniGraph::addOmniEdge(prev, v, threadId);
        }
        this->vStack[threadId].push(v);

        return v;
    }
    bool OmniGraph::updateStackTopVertex(const std::string log, const unsigned int threadId)
    {
        const auto &prevVertex = this->vStack[threadId].top();
        this->g[prevVertex].flowStr += log;
        return true;
    }

    bool OmniGraph::computeHash(const unsigned int threadId)
    {
        computeFlowHash(threadId);
        computeParamHash(threadId);

        return true;
    }

    bool OmniGraph::moveNextVertex(const unsigned int threadId)
    {
        computeHash(threadId);
        popVertex(threadId);
    }

    bool OmniGraph::addOmniEdge(
        boost::graph_traits<Graph>::vertex_descriptor u_,
        boost::graph_traits<Graph>::vertex_descriptor v_, const size_t threadId)
    {
        boost::graph_traits<Graph>::edge_descriptor e;

        bool isInserted = false;
        boost::tie(e, isInserted) = boost::add_edge(u_, v_, this->g);

        return isInserted;
    }
    bool OmniGraph::computeFlowHash(const unsigned int threadId)
    {
        const auto &topVertex = this->vStack[threadId].top();
        this->g[topVertex].flowHash = std::hash<std::string>()(this->g[topVertex].flowStr);
        return true;
    }
    bool OmniGraph::computeParamHash(const unsigned int threadId)
    {
        const auto &topVertex = this->vStack[threadId].top();
        this->g[topVertex].paramHash = std::hash<std::string>()(this->g[topVertex].paramStr);
        return true;
    }
    /**
     * @brief the implementation when vertex Stack is popped. the function describe the relationship between caller and callee.
     *
     * @param threadId the threadId of target log
     * @return true implementation has finished without problem
     * @return false  implementation has finished with problem
     */
    bool OmniGraph::popVertex(const unsigned int threadId)
    {
        const auto crtVertex = this->vStack[threadId].top();
        this->vStack[threadId].pop();
        if (not this->vStack[threadId].empty())
        {
            const auto &callerVertex = this->vStack[threadId].top();
            this->g[callerVertex].controlFlowHash = std::hash<size_t>()(
                this->g[callerVertex].childFlowHash + g[crtVertex].flowHash);
            this->g[callerVertex].controlParamHash = std::hash<size_t>()(
                this->g[callerVertex].childParamHash + g[crtVertex].paramHash);
        }
        g[crtVertex].outputFormat += "\nCFH=" + std::to_string(g[crtVertex].controlFlowHash) + "\nCPH=" + std::to_string(g[crtVertex].controlParamHash);

        return true;
    }
    Graph OmniGraph::getGraphCopy()
    {
        return this->g;
    }

}
