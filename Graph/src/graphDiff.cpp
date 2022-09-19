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

        std::cout << "GRAPHDIFF WORKING\n";

        originPath = this->path;
        targetPath = target.copyGraphPath();

        std::cout << "==================================== PATH DEBUG ===================================\n";
        for (const auto &v : originPath)
        {
            std::cout << this->g[v].methodStr << '\n';
        }
        std::cout << '\n';
        std::cout << "==================================== PATH DEBUG ===================================\n";

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
                // no need to show
                ++originDescIdx;
                ++targetDescIdx;
            }
            else
            {
                boost::graph_traits<Graph>::vertex_descriptor prevVertexDesc = -1;

                if (not diffGraph.isStackEmpty(defaultThreadId))
                {
                    prevVertexDesc = diffGraph.getStackTopVertex(defaultThreadId);
                }

                // TODO: methodHash and
                if (originVertex.methodHash == targetVertex.methodHash)
                {
                    std::cout << "originDescIdx: " << originDescIdx << " targetDescIdx: " << targetDescIdx << '\n';
                    std::cout << ">> origin: " << this->g[originDesc].methodStr << " target: " << target.g[targetDesc].methodStr << '\n';
                    // Based on Diff difinition, there are vertex that it has same method hash, but different flow/param hash.
                    // called same vertices
                    boost::graph_traits<Graph>::vertex_descriptor diffOriginVertexDesc = diffGraph.addOmniVertex(originVertex, defaultThreadId);

                    // if diffOriginVertexDesc is root vertex, no need to create edge.
                    if (diffOriginVertexDesc != diffGraph.root[defaultThreadId])
                    {
                        assert(prevVertexDesc != -1);
                        diffGraph.addOmniEdge(prevVertexDesc, diffOriginVertexDesc, defaultThreadId);
                    }

                    ++originDescIdx;
                    ++targetDescIdx;
                }
                else
                {
                    std::cout << "Different Method is called" << std::endl;
                    std::cout << "originDescIdx: " << originDescIdx << " targetDescIdx: " << targetDescIdx << '\n';
                    std::cout << ">> origin: " << this->g[originDesc].methodStr << " target: " << target.g[targetDesc].methodStr << '\n';
                    boost::graph_traits<Graph>::vertex_descriptor diffOriginVertexDesc = diffGraph.addOmniVertexDesc(originVertex, defaultThreadId);

                    boost::graph_traits<Graph>::vertex_descriptor diffTargetVertexDesc = diffGraph.addOmniVertexDesc(targetVertex, defaultThreadId);

                    if (diffOriginVertexDesc != diffGraph.root[defaultThreadId])
                    {
                        assert(prevVertexDesc != -1);
                        std::cout << "--> " << diffGraph.g[prevVertexDesc].methodStr << std::endl;
                        diffGraph.addOmniEdge(prevVertexDesc, diffOriginVertexDesc, defaultThreadId);
                        diffGraph.addOmniEdge(prevVertexDesc, diffTargetVertexDesc, defaultThreadId);
                        return diffGraph;
                    }
                    else
                    {
                        // In this case, the root vertices are different
                        std::cout << "TOP VERTICES ARE DIFFERENT" << std::endl;
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

                        boost::graph_traits<Graph>::vertex_descriptor newDiffVertexDesc = diffGraph.addOmniVertex(branchVertex, defaultThreadId);

                        diffGraph.addOmniEdge(diffOriginVertexDesc, newDiffVertexDesc, defaultThreadId);

                        diffOriginVertexDesc = newDiffVertexDesc;
                        originDescIdx++;
                    }

                    while (targetDescIdx < targetPath.size() && !isLeaf(targetDescIdx))
                    {
                        // copy from branch vertex Info
                        branchDesc = targetPath[targetDescIdx];
                        branchVertex = targetGraph[branchDesc];

                        boost::graph_traits<Graph>::vertex_descriptor newDiffVertexDesc = diffGraph.addOmniVertex(branchVertex, defaultThreadId);

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

}