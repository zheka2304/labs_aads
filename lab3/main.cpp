#include "graph.h"
#include "pathfinding.h"

int main() {
    Graph mGraph;
    mGraph.read("../../lab3/data.txt");

    while (true) {
        std::cout << "\n\ngraph: ";
        mGraph.print();
        std::cout << "\n\ntype 1 to find shortest path between cities\ntype 2 to show all shortest paths from city\ntype 3 to read file\ntype any other number to exit\ncommand: ";

        int choice = 1;
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::string name;
                std::cout << "input departure city: ";
                std::cin >> name;
                auto vertex1 = mGraph.get_vertex(name);
                std::cout << "input arrival city: ";
                std::cin >> name;
                auto vertex2 = mGraph.get_vertex(name);
                if (vertex1 != nullptr && vertex2 != nullptr) {
                    auto path = bellman_floid_algorithm::get_shortest_paths(mGraph, *vertex1)[vertex2->index];
                    if (path.weight != Graph::INF) {
                        std::cout << "path from " << vertex1->name << " to " << vertex2->name << " (weight=" << path.weight << "): ";
                        for (int j = 0; j < path.path.length(); j++) {
                            std::cout << path.path[j]->name << " ";
                        }
                        std::cout << "\n";
                    } else {
                        std::cout << "no path found\n";
                    }
                } else {
                    std::cout << "invalid city name(s)\n";
                }
                std::cout << "press enter to continue...";
                getchar();
                getchar();
                break;
            }
            case 2: {
                std::string name;
                std::cout << "input departure city: ";
                std::cin >> name;
                Graph::Vertex* start_vertex = mGraph.get_vertex(name);
                if (start_vertex != nullptr) {
                    auto paths = bellman_floid_algorithm::get_shortest_paths(mGraph, *start_vertex);
                    for (int i = 0; i < paths.length(); i++) {
                        auto path = paths[i];
                        std::cout << "path from " << name << " to " << mGraph.get_vertex(i)->name << " (weight=" << path.weight << "): ";
                        for (int j = 0; j < path.path.length(); j++) {
                            std::cout << path.path[j]->name << " ";
                        }
                        std::cout << "\n";
                    }
                } else {
                    std::cout << "invalid city name(s)\n";
                }
                std::cout << "press enter to continue...";
                getchar();
                getchar();
                break;
            }
            case 3: {
                std::string name;
                std::cout << "input filename: ";
                std::cin >> name;
                std::cout << (mGraph.read(name) ? "success" : "fail") << "\n";
                std::cout << "press enter to continue...";
                getchar();
                getchar();
                break;
            }
            default:
                return 0;
        }
    }
}