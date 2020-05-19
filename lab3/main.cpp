#include "graph.h"

namespace bellman_floid_algorithm {
    struct graph_path {
        array<Graph::Vertex*> path;
        double weight = Graph::INF;
    };

    array<graph_path> get_shortest_paths(Graph& graph, Graph::Vertex& offset) {
        array<graph_path> paths(graph.size());
        for (int i = 0; i < paths.length(); i++) {
            paths[i].path.add(&offset);
        }
        paths[offset.index].weight = 0;

        for (int i = 1; i < graph.size() - 1; i++) {
            for (int edge_start = 0; edge_start < graph.size(); edge_start++) {
                for (int edge_end = 0; edge_end < graph.size(); edge_end++) {
                    double weight = graph.get_weight(edge_start, edge_end);
                    if (weight != Graph::INF && paths[edge_start].weight != Graph::INF &&
                                                (paths[edge_end].weight == Graph::INF ||
                                                 paths[edge_end].weight > paths[edge_start].weight + weight)) {
                        paths[edge_end].path = paths[edge_start].path;
                        paths[edge_end].path.add(graph.get_vertex(edge_end));
                        paths[edge_end].weight = paths[edge_start].weight + weight;
                    }
                }
            }
        }
        return paths;
    };
};

int main() {
    Graph mGraph;
    mGraph.read("../../lab3/data.txt");


    std::string start_name = "D";
    auto paths = bellman_floid_algorithm::get_shortest_paths(mGraph, mGraph.get_or_make_vertex(start_name));
    for (int i = 0; i < paths.length(); i++) {
        auto path = paths[i];
        std::cout << "path from " << start_name << " to " << mGraph.get_vertex(i)->name << " (weight=" << path.weight << "): ";
        for (int j = 0; j < path.path.length(); j++) {
            std::cout << path.path[j]->name << " ";
        }
        std::cout << "\n";
    }

    return 0;
}