#include <functional>
#include <string>
#include <string.h>
#include <sstream>
#include <math.h>

#include "rb_map.h"
#include "array.h"
#include "canvas.h"


#ifndef M_GRAPH_H
#define M_GRAPH_H


template <typename V, typename E>

class Graph {
public:
    class Vertex;
    class Edge {
    public:
        friend class Graph;

        bool active = false;
        Vertex* connected = nullptr;
        Vertex* from = nullptr;
        E data;

    private:
        float printX = 0, printY = 0;

    public:

        Edge() = default;
        Edge(Vertex* from, Vertex* connected, E const& data) : active(true), from(from), connected(connected), data(data) {}

        Edge& opposite() {
            return from->graph->get_edge(*connected, *from);
        }

        E& operator*() { return data; }
        E const& operator*() const { return data; }
        E* operator->() { return &data; }
        E const* operator->() const { return &data; }
    };

    class Vertex {
    public:
        friend class Graph;

        Graph* graph = nullptr;
        int index;
        V data;

    private:
        std::string print_str = "";
        float printX = 0, printY = 0;

    public:
        Vertex() = default;
        Vertex(Graph* graph, int index, V const& data) : graph(graph), index(index), data(data) {};

        V& operator*() { return data; }
        V const& operator*() const { return data; }
        V* operator->() { return &data; }
        V const* operator->() const { return &data; }

        void for_each(std::function<void(Edge&)> func, bool iterateInactive = false) {
            for (int i = 0; i < graph->vertices.length(); i++) {
                Edge& edge = graph->matrix[index][i];
                if (iterateInactive || edge.active) {
                    func(edge);
                }
            }
        }
    };

private:
    array<Vertex> vertices;
    array<array<Edge>> matrix;

public:
    array<Vertex>& get_vertices() {
        return vertices;
    }

    array<Vertex> const& get_vertices() const {
        return vertices;
    }

    int size() const {
        return vertices.length();
    }

    void connect(Vertex& v1, Vertex& v2, E const& data) {
        matrix[v1.index][v2.index].active = true;
        matrix[v1.index][v2.index].data = data;
    }

    void disconnect(Vertex& v1, Vertex& v2) {
        matrix[v1.index][v2.index].active = false;
    }

    Edge& get_edge(int index1, int index2) {
        return matrix[index1][index2];
    }

    Edge& get_edge(Vertex& v1, Vertex& v2) {
        return get_edge(v1.index, v2.index);
    }

    template <typename T>
    Vertex* get_vertex(T const& data) {
        for (int i = 0; i < vertices.length(); i++) {
            if (vertices[i].data == data) {
                return &vertices[i];
            }
        }
        return nullptr;
    }

    Vertex& add_vertex(V const& data) {
        Vertex& vertex = vertices.add(Vertex(this, vertices.length(), data));
        matrix.resize(vertices.length());
        for (int i = 0; i < matrix.length(); i++) {
            matrix[i].resize(vertices.length());
            for (int j = i == matrix.length() - 1 ? 0 : vertices.length() - 1; j < vertices.length(); j++) {
                Edge &edge = matrix[i][j];
                edge.from = &vertices[i];
                edge.connected = &vertices[j];
            }
        }
        return vertex;
    }

    void for_each(std::function<void(Vertex&)> func) {
        for (int i = 0; i < vertices.length(); i++) {
            func(vertices[i]);
        }
    }

    void for_each(std::function<void(Edge&)> func, bool iterateInactive = false) {
        for (int i = 0; i < vertices.length(); i++) {
            for (int j = 0; j < vertices.length(); j++) {
                Edge& edge = matrix[i][j];
                if (iterateInactive || edge.active) {
                    func(edge);
                }
            }
        }
    }

    void clear() {
        vertices.clear();
    }

    void print(int arrow_length, std::function<std::string(Vertex&)> vertex_to_string, std::function<std::string(Edge&)> edge_to_string) {
        if (get_vertices().length() == 0) {
            std::cout << "graph is empty\n";
            return;
        }

        Canvas canvas(80, 40);
        for_each([&] (Vertex& vertex) -> void {
            vertex.print_str = vertex_to_string(vertex);
            vertex.printX = 0.5f + std::sin(vertex.index * 6.282f / vertices.length()) * 0.425f;
            vertex.printY = 0.5f + std::cos(vertex.index * 6.282f / vertices.length()) * 0.425f;
        });

        for_each([&] (Edge& edge) -> void {
            auto point = canvas.arrow(edge.from->printX, edge.from->printY, edge.connected->printX, edge.connected->printY);
            edge.printX = point.x;
            edge.printY = point.y;
        });


        for_each([&] (Vertex& vertex) -> void {
            canvas.text(vertex.printX, vertex.printY, vertex.print_str);
        });

        for_each([&] (Edge& edge) -> void {
            canvas.text(edge.printX, edge.printY, edge_to_string(edge));
        });

        canvas.print();
    };
};

#endif