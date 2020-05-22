#include "graph.h"
#include "flow_network.h"

void await_user() {
    std::cout << "press enter to continue...";
    getchar();
}

int main() {
    FlowNetwork network;
    network.read("../../coursework/data.txt");

    while (true) {
        std::cout << "FLOW NETWORK:\n";
        network.print();

        bool is_validated = false;
        switch (network.validate()) {
            case FlowNetwork::RESULT_OK:
                is_validated = true;
                std::cout << "validation result: RESULT_OK: network is valid\n";
                break;
            case FlowNetwork::RESULT_CYCLE:
                std::cout << "validation result: RESULT_CYCLE: graph contains cycles\n";
                break;
            case FlowNetwork::RESULT_INVALID_PATH:
                std::cout << "validation result: INVALID_PATH: some path does not end at target\n";
                break;
            case FlowNetwork::RESULT_NO_TERMINATIONS:
                std::cout << "validation result: RESULT_NO_TERMINATIONS: no source or target\n";
                break;
        }

        std::cout << "\n\n";
        std::cout << "options:" << "\n";
        std::cout << "type 1 to load network from file" << "\n";
        std::cout << "type 2 to build max flow through network" << "\n";
        std::cout << "type 3 to add vertices or edges" << "\n";
        std::cout << "type 4 to disconnect vertices" << "\n";
        std::cout << "type 5 to clear network" << "\n";
        std::cout << "type 6 to reset flow" << "\n";
        std::cout << "type 7 to save network" << "\n";
        std::cout << "type other number to exit" << "\n";
        std::cout << "input command: ";

        int command;
        std::cin >> command;
        getchar(); // consume \n

        switch (command) {
            case 1: {
                std::string name;
                std::cout << "input filename: ";
                std::cin >> name;
                getchar();
                std::cout << (network.read(name) ? "success" : "fail") << "\n";
                await_user();
                break;
            }
            case 2: {
                if (is_validated) {
                    network.build_max_flow();
                } else {
                    std::cout << "flow network is not valid\n";
                    await_user();
                }
                break;
            }
            case 3: {
                std::string line;
                std::cout << "input edge (START_NAME END_NAME MAX_FLOW, S = start, T = target): ";
                std::getline(std::cin, line);
                if (!network.add_edge_from_string(line)) {
                    await_user();
                }
                break;
            }
            case 4: {
                std::string start, end;
                std::cout << "input edge to remove (START_NAME END_NAME): ";
                std::cin >> start;
                std::cin >> end;
                getchar(); // consume \n
                FlowNetwork::Vertex* v1 = network.mGraph.get_vertex(start);
                FlowNetwork::Vertex* v2 = network.mGraph.get_vertex(end);
                if (v1 != nullptr && v2 != nullptr) {
                    FlowNetwork::Edge* edge = network.mGraph.get_edge(*v1, *v2);
                    if (edge != nullptr) {
                        network.mGraph.disconnect(*v1, *v2);
                    } else {
                        std::cout << "no such edge exists\n";
                        await_user();
                    }
                } else {
                    std::cout << "no such vertices\n";
                }

                break;
            }
            case 5: {
                network.clear();
                break;
            }
            case 6: {
                network.reset_edges();
                break;
            }
            case 7: {
                std::string name;
                std::cout << "input filename: ";
                std::cin >> name;
                getchar();
                std::cout << (network.save(name) ? "success" : "fail") << "\n";
                await_user();
                break;
                break;
            }
            default:
                return 0;
        }

        std::cout << "\n\n\n\n----------------------------------------------\n";

    }

}