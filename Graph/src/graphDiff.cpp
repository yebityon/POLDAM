#include "graph.hpp"

/**
 * https://www.researchgate.net/publication/220618233_Simple_Fast_Algorithms_for_the_Editing_Distance_Between_Trees_and_Related_Problems
 *
 */
namespace POLDAM
{
    bool isSameVertex(GraphVertex v, GraphVertex u)
    {
        return true;
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
                // no need to show
            }
            else
            {
                // different vertex, append two vertex
                boost::graph_traits<Graph>::vertex_descriptor prevVertexDesc = diffGraph.getStackTopVertex(defaultThreadId);

                if (originVertex.methodHash == targetVertex.methodHash)
                {
                    // called same vertices
                    boost::graph_traits<Graph>::vertex_descriptor diffOriginVertexDesc = diffGraph.addOmniVertex(originVertex, defaultThreadId);
                    diffGraph.addOmniEdge(prevVertexDesc, diffOriginVertexDesc, defaultThreadId);
                }
                else
                {
                    // different method is called

                    boost::graph_traits<Graph>::vertex_descriptor diffOriginVertexDesc = diffGraph.addOmniVertex(originVertex, defaultThreadId);
                    boost::graph_traits<Graph>::vertex_descriptor diffTargetVertexDesc = diffGraph.addOmniVertex(targetVertex, defaultThreadId);

                    diffGraph.addOmniEdge(prevVertexDesc, diffOriginVertexDesc, defaultThreadId);
                    diffGraph.addOmniEdge(prevVertexDesc, diffTargetVertexDesc, defaultThreadId);

                    // try to append vertex until its leaf
                    originDescIdx++;

                    boost::graph_traits<Graph>::vertex_descriptor branchDesc{};
                    GraphVertex branchVertex{};

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