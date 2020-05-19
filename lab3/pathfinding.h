#include "array.h"
#include "graph.h"


#ifndef M_PATHFINDING_H
#define M_PATHFINDING_H

namespace bellman_floid_algorithm {
    struct graph_path {
        array<Graph::Vertex*> path;
        double weight = Graph::INF;
    };

    array<graph_path> get_shortest_paths(Graph& graph, Graph::Vertex& offset);
};


#endif
