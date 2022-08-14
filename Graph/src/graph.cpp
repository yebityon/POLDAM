#pragma once
#include "graph.hpp"

namespace POLDAM
{
    bool OmniGraph::addOmniEdge(GraphVertex u, GraphVertex v)
    {
    }

    boost::graph_traits<Graph>::vertex_descriptor OmniGraph::createOmniVertex()
    {
        boost::graph_traits<Graph>::vertex_descriptor v = boost::add_vertex(this->g);
        return v;
    }

}
