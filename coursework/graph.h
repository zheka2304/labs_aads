#include <functional>

#include "rb_map.h"
#include "array.h"


#ifndef M_GRAPH_H
#define M_GRAPH_H


template <typename V, typename E>

class Graph {
public:
    class Vertex;
    class Edge {
    public:
        Vertex* connected = nullptr;
        Vertex* from = nullptr;
        E data;

        Edge() = default;
        Edge(Vertex* from, Vertex* connected, E const& data) : from(from), connected(connected), data(data) {}

        E& operator*() { return data; }
        E const& operator*() const { return data; }
        E* operator->() { return &data; }
        E const* operator->() const { return &data; }
    };

    class Vertex {
    public:
        list<Edge> edges;
        V data;

        Vertex() = default;
        Vertex(V const& data) : data(data) {};

        V& operator*() { return data; }
        V const& operator*() const { return data; }
        V* operator->() { return &data; }
        V const* operator->() const { return &data; }

        void for_each(std::function<void(Edge&)> func) {
            for (auto it = edges.begin(); it != edges.end(); it++) {
                func(*it);
            }
        }
    };

private:
    array<Vertex> vertices;

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
        for (auto i = v1.edges.begin(); i != v1.edges.end(); i++) {
            if (i->connected == &v2) {
                i->data = data;
                return;
            }
        }
        v1.edges.add(Edge(&v1, &v2, data));
    }

    void disconnect(Vertex& v1, Vertex& v2) {
        for (auto i = v1.edges.begin(); i != v1.edges.end(); i++) {
            if (i->connected == &v2) {
                v1.edges.erase(i);
                break;
            }
        }
    }

    Edge* get_edge(Vertex& v1, Vertex& v2) const {
        for (auto i = v1.edges.begin(); i != v1.edges.end(); i++) {
            if (i->connected == &v2) {
                return &*i;
            }
        }
        return nullptr;
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
        return vertices.add(Vertex(data));
    }

    void for_each(std::function<void(Vertex&)> func) {
        for (int i = 0; i < vertices.length(); i++) {
            func(vertices[i]);
        }
    }

    void for_each(std::function<void(Edge&)> func) {
        for (int i = 0; i < vertices.length(); i++) {
            vertices[i].for_each(func);
        }
    }

    void clear() {
        vertices.clear();
    }
};

#endif