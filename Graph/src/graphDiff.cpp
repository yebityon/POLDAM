#include "graph.hpp"

/**
 * GraphDiff algorithm is beta, There might be good way to compute the diff between two graphs, like following URL
 * https://www.researchgate.net/publication/220618233_Simple_Fast_Algorithms_for_the_Editing_Distance_Between_Trees_and_Related_Problems
 *
 */
namespace POLDAM
{
    bool isSameVertex(GraphVertex v, GraphVertex u)
    {
        return v.flowHash == u.flowHash;
    }

    /**
     * @brief compute the tree diff between this graph and given OmniGraoh.
     * @param target
     * @return OmniGraph the diffGraph
     */
    OmniGraph OmniGraph::computeDiffGraph(OmniGraph &target)
    {
        OmniGraph diffGraph;

        const Graph originGraph = this->g;
        const Graph targetGraph = target.getGraphCopy();

        std::vector<boost::graph_traits<Graph>::vertex_descriptor> originPath, targetPath;

        originPath = this->path;
        targetPath = target.copyGraphPath();

        unsigned int originDescIdx = 0, targetDescIdx = 0;

        // For the diffgraph, you do not need to desginate threadId
        const unsigned int defaultThreadId = 0;

        while (originDescIdx < originPath.size() && targetDescIdx < targetPath.size())
        {
            boost::graph_traits<Graph>::vertex_descriptor originDesc, targetDesc;

            originDesc = originPath[originDescIdx];
            targetDesc = targetPath[targetDescIdx];

            GraphVertex originVertex = originGraph[originDesc];
            GraphVertex targetVertex = targetGraph[targetDesc];

            if (isSameVertex(originVertex, targetVertex))
            {
                ++originDescIdx;
                ++targetDescIdx;
            }
            else
            { // Different flow is detected.
                boost::graph_traits<Graph>::vertex_descriptor prevVertexDesc = -1;

                if (not diffGraph.isStackEmpty(defaultThreadId))
                {
                    prevVertexDesc = diffGraph.getStackTopVertex(defaultThreadId);
                }

                if (originVertex.methodHash == targetVertex.methodHash)
                {
                    // Different Execution is detected in same method.
                    // Proceed Diff algorithm until a differnt method call is detected or method on the execution path becomes a leaf in this graph.
                    boost::graph_traits<Graph>::vertex_descriptor diffOriginVertexDesc = diffGraph.addOmniVertexDesc(originVertex, defaultThreadId);

                    // if diffOriginVertexDesc is root vertex, no need to create edge.
                    if (diffOriginVertexDesc != diffGraph.root[defaultThreadId])
                    {
                        assert(prevVertexDesc != -1);
                        diffGraph.addOmniEdge(prevVertexDesc, diffOriginVertexDesc, defaultThreadId);
                    }
                    else
                    {
                        diffGraph.pushStackVertex(diffOriginVertexDesc, defaultThreadId);
                    }
                    ++originDescIdx;
                    ++targetDescIdx;

                    // if the diff between two graph is too big, should use early return.
                    if (OmniGraph::isLeaf(originDesc))
                    {
                        return diffGraph;
                    }
                }
                else /*if originVertex.methodHash != targetVertex.methodHash : DIFFERENT METHOD IS CALLED*/
                {
                    /**
                     *  if different method call is detected, it's impossible to compare the two method callds
                     */

                    boost::graph_traits<Graph>::vertex_descriptor diffOriginVertexDesc = diffGraph.addOmniVertexDesc(originVertex, defaultThreadId);
                    boost::graph_traits<Graph>::vertex_descriptor diffTargetVertexDesc = diffGraph.addOmniVertexDesc(targetVertex, defaultThreadId);

                    if (diffOriginVertexDesc != diffGraph.root[defaultThreadId])
                    {
                        assert(prevVertexDesc != -1);
                        diffGraph.addOmniEdge(prevVertexDesc, diffOriginVertexDesc, defaultThreadId);
                        diffGraph.addOmniEdge(prevVertexDesc, diffTargetVertexDesc, defaultThreadId);
                    }
                    else
                    {
                        // In this case, the root vertices are different
                        return POLDAM::OmniGraph();
                    }

                    // try to append vertex until its leaf
                    originDescIdx++;

                    boost::graph_traits<Graph>::vertex_descriptor branchDesc{};
                    GraphVertex branchVertex{};
                    std::cout << "never reach here\n";
                    while (originDescIdx < originPath.size() && !isLeaf(originDescIdx))
                    {
                        // copy from branch vertex Info
                        branchDesc = originPath[originDescIdx];
                        branchVertex = originGraph[branchDesc];

                        boost::graph_traits<Graph>::vertex_descriptor newDiffVertexDesc = diffGraph.addOmniVertexDesc(branchVertex, defaultThreadId);

                        diffGraph.addOmniEdge(diffOriginVertexDesc, newDiffVertexDesc, defaultThreadId);

                        diffOriginVertexDesc = newDiffVertexDesc;
                        originDescIdx++;
                    }

                    while (targetDescIdx < targetPath.size() && !isLeaf(targetDescIdx))
                    {
                        // copy from branch vertex Info
                        branchDesc = targetPath[targetDescIdx];
                        branchVertex = targetGraph[branchDesc];

                        boost::graph_traits<Graph>::vertex_descriptor newDiffVertexDesc = diffGraph.addOmniVertexDesc(branchVertex, defaultThreadId);

                        diffGraph.addOmniEdge(diffTargetVertexDesc, newDiffVertexDesc, defaultThreadId);

                        diffOriginVertexDesc = newDiffVertexDesc;
                        targetDescIdx++;
                    }

                    return diffGraph;
                }
            }
        }

        if (originDescIdx >= originPath.size() && targetDescIdx >= targetPath.size())
        {
            // Itereate ALL vertices , return Immediately
            return diffGraph;
        }
        else if (originDescIdx >= originPath.size())
        {
        }
        else if (targetDescIdx >= targetPath.size())
        {
        }

        return diffGraph;
    }

    void OmniGraph::traverseDiffVertices(const Graph &originGraph, const Graph &targetGraph,
                                         const boost::graph_traits<Graph>::vertex_descriptor originVerDesc,
                                         const boost::graph_traits<Graph>::vertex_descriptor targetVerDesc,
                                         const boost::graph_traits<Graph>::vertex_descriptor parDiffVerDesc)
    {

        assert(not isSameVertex(originGraph[originVerDesc], targetGraph[targetVerDesc]));
        boost::graph_traits<Graph>::out_edge_iterator oEdgeItr,
            oEdgeItrEnd, tEdgeItr, tEdgeItrEnd;

        const unsigned int oOutEdgeSize = boost::out_degree(originVerDesc, originGraph);
        const unsigned int tOutEdgeSize = boost::out_degree(targetVerDesc, targetGraph);

        // if oOutEdgeSize  != tOutEdgeSize, it indicated there is a different method call

        if (oOutEdgeSize != tOutEdgeSize)
        {
            std::cout << "DIFFERENT NUMBER OF METHOD CALL IS DETECTED\n";

            boost::tie(oEdgeItr, oEdgeItrEnd) = boost::out_edges(originVerDesc, originGraph);
            boost::tie(tEdgeItr, tEdgeItrEnd) = boost::out_edges(targetVerDesc, targetGraph);

            for (unsigned int i = 0; i < std::min(oOutEdgeSize, tOutEdgeSize); ++i)
            {
                const boost::graph_traits<Graph>::vertex_descriptor nOriginVerDesc = boost::target(*oEdgeItr, originGraph);
                const boost::graph_traits<Graph>::vertex_descriptor nTragetVerDesc = boost::target(*tEdgeItr, targetGraph);

                boost::graph_traits<Graph>::edge_descriptor edgeDesc;
                boost::graph_traits<Graph>::vertex_descriptor verDesc;

                verDesc = boost::add_vertex(this->diffGraph);
                diffGraph[verDesc] = originGraph[nOriginVerDesc];
                bool isInserted = false;
                boost::tie(edgeDesc, isInserted) = boost::add_edge(parDiffVerDesc, verDesc, this->diffGraph);
                ++oEdgeItr;
                ++tEdgeItr;
            }

            boost::graph_traits<Graph>::vertex_descriptor extraMethodCallDesc = boost::add_vertex(this->diffGraph);

            if (oEdgeItr != oEdgeItrEnd)
            {
                this->diffGraph[extraMethodCallDesc] = originGraph[boost::target(*oEdgeItr, originGraph)];
            }
            else
            {
                assert(tEdgeItr != tEdgeItrEnd);

                this->diffGraph[extraMethodCallDesc] = targetGraph[boost::target(*tEdgeItr, targetGraph)];
            }

            this->diffGraph[extraMethodCallDesc].outputFormat += "\nDIFFEREMT METHOD CALL";

            return;
        }

        // oOutEdgeSize ==  tOutEdgeSize --> True

        boost::tie(oEdgeItr, oEdgeItrEnd) = boost::out_edges(originVerDesc, originGraph);
        boost::tie(tEdgeItr, tEdgeItrEnd) = boost::out_edges(targetVerDesc, targetGraph);

        for (unsigned int i = 0; i < oOutEdgeSize; ++i)
        {
            if (oEdgeItr != oEdgeItrEnd && tEdgeItr != oEdgeItrEnd)
            {
                const boost::graph_traits<Graph>::vertex_descriptor nOriginVerDesc = boost::target(*oEdgeItr, originGraph);
                const boost::graph_traits<Graph>::vertex_descriptor nTragetVerDesc = boost::target(*tEdgeItr, targetGraph);

                boost::graph_traits<Graph>::edge_descriptor edgeDesc;
                boost::graph_traits<Graph>::vertex_descriptor verDesc;

                verDesc = boost::add_vertex(this->diffGraph);
                diffGraph[verDesc] = originGraph[nOriginVerDesc];
                bool isInserted = false;
                boost::tie(edgeDesc, isInserted) = boost::add_edge(parDiffVerDesc, verDesc, this->diffGraph);

                if (isSameVertex(originGraph[nOriginVerDesc], targetGraph[nTragetVerDesc]))
                {
                    // no need to traversal anymore.
                }
                else
                {
                    traverseDiffVertices(originGraph, targetGraph, nOriginVerDesc, nTragetVerDesc, verDesc);
                    break;
                }

                ++oEdgeItr;
                ++tEdgeItr;
            }
        }

        return;
    }

    OmniGraph OmniGraph::computeDiffGraphBeta(OmniGraph &&target)
    {
        const Graph &originGraph = this->g;
        const Graph &targetGraph = target.g;

        unsigned int originDescIdx = 0, targetDescIdx = 0;
        const unsigned int defaultThreadId = 0;

        // create root vertices
        boost::graph_traits<Graph>::vertex_descriptor diffGraphRootDesc = boost::add_vertex(this->diffGraph);

        this->diffGraph[diffGraphRootDesc].outputFormat = "DIFF GRAPH";

        traverseDiffVertices(originGraph, targetGraph,
                             this->getRoot(),
                             target.getRoot(),
                             diffGraphRootDesc);
    }
}