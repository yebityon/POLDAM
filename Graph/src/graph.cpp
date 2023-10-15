#include "graph.h"

namespace POLDAM
{
    bool PoldamGraph::addOmniVertex(GraphVertex v_, const size_t threadId)
    {
        // FIXME: it should be perfect forwarding
        boost::graph_traits<Graph>::vertex_descriptor v = PoldamGraph::addOmniVertexDesc(v_, threadId);
        if (not this->vStack[threadId].empty())
        {
            const auto &prev = PoldamGraph::getStackTopVertex(threadId);
            PoldamGraph::addOmniEdge(prev, v, threadId);
        }
        else
        {
            // entryMethodNameが明示的に与えられなかった場合は、最初の頂点をthis->Rootに設定
            if (!config.hasEntryMethodName && this->root.find(threadId) == this->root.end() /*threadID拡張のため*/)
            {
                this->Root = v;
                this->root[threadId] = v;
            }
        }
        PoldamGraph::pushStackVertex(v, threadId);
        // Thread IDのことは考えないことにする。
        if (this->g[v].classStr == config.entryClassName && this->g[v].methodStr == config.entryMethodName && this->root.find(threadId) == this->root.end())
        {
            this->Root = v;
            this->g[v].isTargetVertex = true;
            this->root[threadId] = v;
        }
        if(config.hasFilterdRegex) {
            if(std::regex_match(this->g[v].methodStr, config.filterdVertexRegex)) {
                this->g[v].isFilteredVertex = true;
            }
        }
        this->path.push_back(v);

        return v;
    }

    boost::graph_traits<Graph>::vertex_descriptor PoldamGraph::addOmniVertexDesc(GraphVertex v_, const size_t threadId)
    {
        boost::graph_traits<Graph>::vertex_descriptor v = boost::add_vertex(this->g);
        this->g[v] = v_;
        return v;
    }

    bool PoldamGraph::updateStackTopVertex(const std::string log, const unsigned int threadId)
    {
        const auto &prevVertex = this->vStack[threadId].top();
        this->g[prevVertex].flowStr += log;
        return true;
    }

    bool PoldamGraph::updateStackTopVertexParamInfo(const std::string &paramValue, unsigned int threadId)
    {
        const auto &prevVertx = this->vStack[threadId].top();
        this->g[prevVertx].paramStr += paramValue;
        return true;
    }

    bool PoldamGraph::popStackVertex(const unsigned int threadId)
    {
        popVertex(threadId);
        return true;
    }

    bool PoldamGraph::pushStackVertex(const boost::graph_traits<Graph>::vertex_descriptor vDesc, const unsigned int threadId)
    {
        this->vStack[threadId].push(vDesc);
        return true;
    }

    bool PoldamGraph::computeHash(const unsigned int threadId)
    {
        computeFlowHash(threadId);
        computeParamHash(threadId);

        return true;
    }

    bool PoldamGraph::isStackEmpty(const unsigned int threadId)
    {
        return this->vStack[threadId].empty();
    }

    bool PoldamGraph::isLeaf(boost::graph_traits<Graph>::vertex_descriptor vDesc)
    {
        return boost::out_degree(vDesc, this->g) == 1;
    }

    bool PoldamGraph::addOmniEdge(
        boost::graph_traits<Graph>::vertex_descriptor u_,
        boost::graph_traits<Graph>::vertex_descriptor v_, const size_t threadId)
    {
        boost::graph_traits<Graph>::edge_descriptor e;

        bool isInserted = false;
        boost::tie(e, isInserted) = boost::add_edge(u_, v_, this->g);
        if (isInserted)
        {
            // u_ -> v_ のedgeで, u_がfilter viewであればv_の頂点も出力対象とする
            if (this->g[u_].isTargetVertex)
            {
                this->g[v_].isTargetVertex = true;
            }
            
            if(this -> g[u_].isFilteredVertex) {
                this -> g[v_].isFilteredVertex = true;
            }   
        }

        return isInserted;
    }

    boost::graph_traits<Graph>::vertex_descriptor PoldamGraph::getStackTopVertex(const unsigned int threadId)
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

    bool PoldamGraph::computeFlowHash(const unsigned int threadId)
    {
        const auto &topVertex = this->vStack[threadId].top();
        this->g[topVertex].flowHash = std::hash<std::string>()(this->g[topVertex].flowStr);

        return true;
    }

    bool PoldamGraph::computeParamHash(const unsigned int threadId)
    {
        const auto &topVertex = this->vStack[threadId].top();
        this->g[topVertex].paramHash = std::hash<std::string>()(this->g[topVertex].paramStr);
        return true;
    }

    bool PoldamGraph::popVertex(const unsigned int threadId)
    {
        const boost::graph_traits<Graph>::vertex_descriptor &crtVertex = this->vStack[threadId].top();
        this->g[crtVertex].controlFlowHash = std::hash<size_t>()(
            this->g[crtVertex].childFlowHash +
            this->g[crtVertex].flowHash);

        this->g[crtVertex].controlParamHash = std::hash<size_t>()(
            this->g[crtVertex].childParamHash +
            this->g[crtVertex].paramHash);

        g[crtVertex].outputFormat += "\nCFH=" + std::to_string(g[crtVertex].controlFlowHash) + "\nCPH=" + std::to_string(g[crtVertex].controlParamHash);

        this->vStack[threadId].pop();
        if (not isStackEmpty(threadId))
        {
            const auto &callerVertex = this->vStack[threadId].top();
            this->g[callerVertex].childFlowHash = std::hash<size_t>()(
                this->g[callerVertex].childFlowHash + g[crtVertex].controlFlowHash);

            this->g[callerVertex].childParamHash = std::hash<size_t>()(
                this->g[callerVertex].childParamHash + g[crtVertex].controlParamHash);
        }

        return true;
    }

    Graph PoldamGraph::getGraphCopy()
    {
        return this->g;
    }

}
