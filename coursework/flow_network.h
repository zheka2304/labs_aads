#include <fstream>
#include <cstring>

#include "graph.h"
#include "queue.h"

#ifndef M_FLOW_NETWORK_H
#define M_FLOW_NETWORK_H

class FlowNetwork {
public:
    class NodeData;
    class EdgeData;
    typedef Graph<NodeData, EdgeData>::Vertex Vertex;
    typedef Graph<NodeData, EdgeData>::Edge Edge;


    class NodeData {
    public:
        std::string name;

        bool visited = false;
        Vertex* path_next = nullptr;
        Vertex* path_last = nullptr;
        int print_position = 0;

        NodeData() = default;
        NodeData(std::string const& name) : name(name) {};

        void reset() {
            visited = false;
            path_next = path_last = nullptr;
        }

        bool operator==(NodeData const& other) {
            return name == other.name;
        }

        bool operator==(std::string const& str) {
            return name == str;
        }
    };

    class EdgeData {
    public:
        int max_flow = 0;
        int flow = 0;

        bool print_flag = false;

        EdgeData() = default;
        EdgeData(int max_flow) : max_flow(max_flow) {};

        void reset() {
            flow = 0;
        }

        int remaining_flow() {
            return max_flow - flow;
        }
    };


    Graph<NodeData, EdgeData> mGraph;
    Vertex* source = nullptr;
    Vertex* target = nullptr;

    void clear() {
        mGraph.clear();
        source = target = nullptr;
    }

    void reset_vertices() {
        mGraph.for_each([] (Vertex& vertex) -> void {
            vertex.data.reset();
        });
    }

    void reset_edges() {
        mGraph.for_each([] (Edge& edge) -> void {
            edge.data.reset();
        });
    }

    enum ValidationResult : int {
        RESULT_OK = 0,
        RESULT_CYCLE = 1,
        RESULT_INVALID_PATH = 2,
        RESULT_NO_TERMINATIONS = 3
    };

    // check if graph has no cycles and all paths from source are leading to target
    // this function will reset vertices
    ValidationResult validate(Vertex* vertex = nullptr, Vertex* last = nullptr) {
        if (vertex == nullptr) {
            if (source == nullptr || target == nullptr) {
                return RESULT_NO_TERMINATIONS;
            }
            vertex = source;
            reset_vertices();
        }
        if (vertex->data.visited) {
            return RESULT_CYCLE;
        }
        vertex->data.visited = true;
        bool any = false;
        for (auto it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
            if (it->connected != last) {
                any = true;
                ValidationResult result = validate(it->connected, vertex);
                if (result != RESULT_OK) {
                    return result;
                }
            }
        }
        if (!any && target != vertex) {
            return RESULT_INVALID_PATH;
        }
        vertex->data.visited = false;
        return RESULT_OK;
    }

    array<Vertex*> bfs() {
        reset_vertices();

        Queue<Vertex*> queue;
        queue.enqueue(source);
        source->data.visited = true;

        while (!queue.empty()) {
            Vertex* vertex = queue.dequeue();
            if (vertex == target) {
                array<Vertex*> path;
                while(vertex != nullptr) {
                    path.add(vertex);
                    vertex = vertex->data.path_last;
                }
                return path;
            }

            for (auto it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
                if (!it->connected->data.visited && it->data.remaining_flow() > 0) {
                    it->connected->data.visited = true;
                    it->connected->data.path_last = vertex;
                    queue.enqueue(it->connected);
                }
            }
        }

        return array<Vertex*>();
    }

    // build max flow through network
    void build_max_flow() {
        reset_edges();

        while (true) {
            array<Vertex*> path = bfs();
            if (path.length() == 0) {
                break;
            }

            array<Edge*> path_edges;
            for (int i = 0; i < path.length() - 1; i++) {
                path_edges.add(mGraph.get_edge(*path[i + 1], *path[i]));
            }

            int min_remaining_flow = path_edges[0]->data.remaining_flow();
            for (int i = 1; i < path_edges.length(); i++) {
                int remaining_flow = path_edges[i]->data.remaining_flow();
                if (remaining_flow < min_remaining_flow) {
                    min_remaining_flow = remaining_flow;
                }
            }

            for (int i = 0; i < path_edges.length(); i++) {
                path_edges[i]->data.flow += min_remaining_flow;
            }
        }
    }

    void print() {
        if (mGraph.get_vertices().length() == 0) {
            std::cout << "network graph is empty\n";
            return;
        }

        int string_length = 1;
        int arrow_length = 13;

        mGraph.for_each([] (Edge& edge) -> void {
            edge.data.print_flag = false;
        });

        array<Vertex>& vertices = mGraph.get_vertices();
        for (int i = 0; i < vertices.length(); i++) {
            vertices[i].data.print_position = i;
            if (vertices[i].data.name.length() > string_length) {
                string_length = (int) vertices[i].data.name.length();
            }
        }

        for (int i = 0; i < vertices.length(); i++) {
            Vertex& vertex = vertices[i];
            std::cout << vertex.data.name;
            int len = string_length + arrow_length - (int) vertex.data.name.length();
            for (int j = 0; j < len; j++) {
                std::cout << " ";
            }
        }
        std::cout << "\n";

        int counter = 0;
        bool all_printed = false;
        while (!all_printed) {
            int line_length = (string_length + arrow_length) * vertices.length();
            char* line = new char[line_length + 1];
            for (int i = 0; i < line_length; i++) {
                line[i] = i % (string_length + arrow_length) == 0 ? '|' : ' ';
            }
            line[line_length] = 0;

            int end_position = -1;
            while (!all_printed) {
                Edge *min_edge = nullptr;
                int cur_min_edge_end = -1;
                all_printed = true;
                mGraph.for_each([&](Edge &edge) -> void {
                    if (!edge.data.print_flag) {
                        all_printed = false;
                        int start_pos = std::min(edge.from->data.print_position, edge.connected->data.print_position);
                        int end_pos = std::max(edge.from->data.print_position, edge.connected->data.print_position);
                        if (std::min(start_pos, end_pos) >= end_position) {
                            if (min_edge == nullptr || end_pos < cur_min_edge_end) {
                                min_edge = &edge;
                                cur_min_edge_end = end_pos;
                            }
                        }
                    }
                });

                if (min_edge == nullptr) {
                    break;
                }
                min_edge->data.print_flag = true;
                end_position = cur_min_edge_end;

                int edge_start = 1 + std::min(min_edge->from->data.print_position, min_edge->connected->data.print_position) * (string_length + arrow_length);
                int edge_end = std::max(min_edge->from->data.print_position, min_edge->connected->data.print_position) * (string_length + arrow_length);
                for (int i = edge_start; i < edge_end; i++) {
                    line[i] = '-';
                }

                int mid = (edge_end + edge_start) / 2 - 3;
                sprintf(&line[mid], " %2i/%-2i ", min_edge->data.flow, min_edge->data.max_flow);
                line[strlen(line)] = '-';

                if (min_edge->from->data.print_position < min_edge->connected->data.print_position) {
                    line[edge_end - 1] = '>';
                } else {
                    line[edge_start] = '<';
                }
            }

            std::cout << line << "\n";
            delete[] line;
        }

    };

    Vertex& get_or_add_vertex(std::string const& name) {
        Vertex* vertex = mGraph.get_vertex(name);
        if (vertex == nullptr) {
            return mGraph.add_vertex(NodeData(name));
        }
        return *vertex;
    }

    bool add_edge_from_string(std::string line) {
        array<std::string> tokens;

        char* token = strtok(line.data(), " ");
        while (token != nullptr) {
            tokens.add(std::string(token));
            token = strtok(nullptr, " ");
        }

        if (tokens.length() == 3) {
            Vertex& vertex1 = get_or_add_vertex(tokens[0]);
            Vertex& vertex2 = get_or_add_vertex(tokens[1]);
            mGraph.connect(vertex1, vertex2, EdgeData(std::strtol(tokens[2].data(), nullptr, 10)));
            if (vertex1.data.name == "S") {
                source = &vertex1;
            }
            if (vertex2.data.name == "T") {
                target = &vertex2;
            }
            return true;
        } else if (!line.empty()) {
            std::cout << "invalid formatted edge: " << line << "\n";
        }
        return false;
    }

    bool read(std::string filename) {
        std::ifstream stream(filename);
        if (stream) {
            mGraph.clear();
            std::string line;
            while (std::getline(stream, line)) {
                add_edge_from_string(line);
            }
            return true;
        } else {
            return false;
        }
    }

    bool save(std::string filename) {
        std::ofstream stream(filename);
        if (stream) {
            mGraph.for_each([&] (Edge& edge) -> void {
                stream << edge.from->data.name << " " << edge.connected->data.name << " " << edge.data.max_flow << "\n";
            });
            return true;
        } else {
            return false;
        }
    }

};

#endif
