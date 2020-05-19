#include "graph.h"
#include "pathfinding.h"

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