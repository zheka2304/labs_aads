#include <fstream>
#include <string.h>

#include "array.h"
#include "rb_map.h"


#ifndef M_GRAPH_H
#define M_GRAPH_H

class Graph {
public:
    static const double constexpr INF = -1;

    class Vertex {
    public:
        int index;
        std::string name;

        Vertex() = default;
        Vertex(std::string const& name, int index) : name(name), index(index) {};
    };

    array<Vertex> vertices;
    rb_map<std::string, Vertex*> vertex_map; // map for faster access
    array<array<double>> weights;

    void connect(int index1, int index2, double weight) {
        weights[index1][index2] = weight;
    }

    void connect(Vertex& v1, Vertex& v2, double weight) {
        connect(v1.index, v2.index, weight);
    }

    double get_weight(int index1, int index2) {
        if (index1 == index2) {
            return 0;
        }
        return weights[index1][index2];
    }

    double get_weight(Vertex& v1, Vertex& v2) {
        return get_weight(v1.index, v2.index);
    }

    Vertex* get_vertex(int index) {
        if (index >= 0 && index < vertices.length()) {
            return &vertices[index];
        } else {
            return nullptr;
        }
    }

    Vertex* get_vertex(std::string const& name) {
        auto found = vertex_map.find(name);
        if (found != nullptr) {
            return **found;
        }
        return nullptr;
    }

    Vertex& get_or_make_vertex(std::string const& name) {
        Vertex* existing = get_vertex(name);
        if (existing != nullptr) {
            return *existing;
        }
        Vertex vertex(name, vertices.length());
        vertices.add(vertex);
        vertex_map[name] = &(vertices[vertex.index]);
        weights.add(array<double>());
        for (int i = 0; i < weights.length(); i++) {
            array<double>& row = weights[i];
            while (row.length() < weights.length()) {
                row.add(INF);
            }
        }
        return vertices[vertex.index];
    }

    int size() {
        return vertices.length();
    }

    void clear() {
        weights.clear();
        vertices.clear();
        vertex_map.clear();
    }

    bool read(std::string filename) {
        std::ifstream stream(filename);
        if (stream) {
            clear();

            std::string line;
            while (std::getline(stream, line)) {
                array<std::string> tokens;

                char* token = strtok(line.data(), ";");
                while (token != nullptr) {
                    tokens.add(std::string(token));
                    token = strtok(nullptr, ";");
                }

                if (tokens.length() == 4) {
                    Vertex& vertex1 = get_or_make_vertex(tokens[0]);
                    Vertex& vertex2 = get_or_make_vertex(tokens[1]);
                    if (tokens[2] != "N/A") {
                        connect(vertex1, vertex2, std::strtod(tokens[2].data(), nullptr));
                    }
                    if (tokens[3] != "N/A") {
                        connect(vertex2, vertex1, std::strtod(tokens[3].data(), nullptr));
                    }
                } else if (!line.empty()) {
                    std::cout << "error in reading file, line skipped: " << line << "\n";
                }
            }
            return true;
        } else {
            return false;
        }
    }

    void print() {
        for (int i = 0; i < size(); i++) {
            std::cout << "Adjacent to " << get_vertex(i)->name << ": ";
            for (int j = 0; j < size(); j++) {
                double weight = get_weight(i, j);
                if (i != j && weight != Graph::INF) {
                    Graph::Vertex* vertex = get_vertex(j);
                    std::cout << vertex->name << "=" << weight << " ";
                }
            }
            std::cout << "\n";
        }
    }

};

#endif
