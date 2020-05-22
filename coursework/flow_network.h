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

        NodeData();
        NodeData(std::string const& name);

        void reset();
        bool operator==(NodeData const& other);
        bool operator==(std::string const& str);
    };

    class EdgeData {
    public:
        int max_flow = 0;
        int flow = 0;

        EdgeData();
        EdgeData(int max_flow);
        void reset();
        int remaining_flow();
    };

    enum ValidationResult : int {
        RESULT_OK = 0,
        RESULT_CYCLE = 1,
        RESULT_INVALID_PATH = 2,
        RESULT_NO_TERMINATIONS = 3
    };


    Graph<NodeData, EdgeData> mGraph;
    Vertex* source = nullptr;
    Vertex* target = nullptr;

    void clear();
    void reset_vertices();
    void reset_edges();
    Vertex& get_or_add_vertex(std::string const& name);

    // check if graph has no cycles and all paths from source are leading to target
    // this function will reset vertices
    ValidationResult validate(Vertex* vertex = nullptr, Vertex* last = nullptr);
    array<Vertex*> bfs();
    void build_max_flow();
    int get_flow_at(Vertex& vertex);
    int get_flow_at_source();
    int get_flow_at_target();

    bool add_edge_from_string(std::string line);
    bool read(std::string filename);
    bool save(std::string filename);
    void print();

};



#endif
