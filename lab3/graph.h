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

        Vertex();
        Vertex(std::string const& name, int index);
    };

private:
    array<Vertex> vertices;
    rb_map<std::string, Vertex*> vertex_map; // map for faster access
    array<array<double>> weights;

public:
    void connect(int index1, int index2, double weight);
    void connect(Vertex& v1, Vertex& v2, double weight);
    double get_weight(int index1, int index2);
    double get_weight(Vertex& v1, Vertex& v2);
    Vertex* get_vertex(int index);
    Vertex* get_vertex(std::string const& name);
    Vertex& get_or_make_vertex(std::string const& name);
    int size();
    void clear();
    bool read(std::string filename);
    void print();
};

#endif
