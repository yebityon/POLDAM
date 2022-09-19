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
        else
        {
            root[threadId] = v;
        }
        this->vStack[threadId].push(v);
        this->path.push_back(v);

        return v;
    }

    boost::graph_traits<Graph>::vertex_descriptor OmniGraph::addOmniVertexDesc(GraphVertex v_, const size_t threadId)
    {
        boost::graph_traits<Graph>::vertex_descriptor v = boost::add_vertex(this->g);
        this->g[v] = v_;
        return v;
    }

    bool OmniGraph::updateStackTopVertex(const std::string log, const unsigned int threadId)
    {
        const auto &prevVertex = this->vStack[threadId].top();
        this->g[prevVertex].flowStr += log;
        return true;
    }

    bool OmniGraph::popStackVertex(const unsigned int threadId)
    {
        popVertex(threadId);
        return true;
    }

    bool OmniGraph::computeHash(const unsigned int threadId)
    {
        computeFlowHash(threadId);
        computeParamHash(threadId);

        return true;
    }

    bool OmniGraph::isStackEmpty(const unsigned int threadId)
    {
        return this->vStack[threadId].empty();
    }

    bool OmniGraph::isLeaf(boost::graph_traits<Graph>::vertex_descriptor vDesc)
    {
        return boost::out_degree(vDesc, this->g);
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

    boost::graph_traits<Graph>::vertex_descriptor OmniGraph::getStackTopVertex(const unsigned int threadId)
    {
        // TODO: handle error exception
        try
        {
            return this->vStack[threadId].top();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return boost::graph_traits<Graph>::vertex_descriptor();
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
     * @return true implementation has succesfully finished.
     * @return false implementation has not finished succesfully.
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

        this->g[crtVertex].controlFlowHash = std::hash<size_t>()(
            this->g[crtVertex].childFlowHash +
            this->g[crtVertex].flowHash);

        this->g[crtVertex].controlParamHash = std::hash<size_t>()(
            this->g[crtVertex].childParamHash +
            this->g[crtVertex].paramHash);

        g[crtVertex].outputFormat += "\nCFH=" + std::to_string(g[crtVertex].controlFlowHash) + "\nCPH=" + std::to_string(g[crtVertex].controlParamHash);

        return true;
    }

    Graph OmniGraph::getGraphCopy()
    {
        return this->g;
    }

}
