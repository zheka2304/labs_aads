#include "flow_network.h"

FlowNetwork::NodeData::NodeData() {}

FlowNetwork::NodeData::NodeData(std::string const &name) : name(name) {}

void FlowNetwork::NodeData::reset() {
    visited = false;
    path_next = path_last = nullptr;
}

bool FlowNetwork::NodeData::operator==(const FlowNetwork::NodeData &other) {
    return name == other.name;
}

bool FlowNetwork::NodeData::operator==(std::string const &str) {
    return name == str;
}

FlowNetwork::EdgeData::EdgeData() {}

FlowNetwork::EdgeData::EdgeData(int max_flow) : max_flow(max_flow) {}

void FlowNetwork::EdgeData::reset() {
    flow = 0;
}

int FlowNetwork::EdgeData::remaining_flow() {
    return max_flow - flow;
}



void FlowNetwork::clear() {
    mGraph.clear();
    source = target = nullptr;
}

void FlowNetwork::reset_vertices() {
    mGraph.for_each([] (Vertex& vertex) -> void {
        vertex.data.reset();
    });
}

void FlowNetwork::reset_edges() {
    mGraph.for_each([] (Edge& edge) -> void {
        edge.data.reset();
    }, true);
}

FlowNetwork::ValidationResult FlowNetwork::validate(FlowNetwork::Vertex *vertex, FlowNetwork::Vertex *last) {
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

    for (int i = 0; i < mGraph.size(); i++) {
        Edge& edge = mGraph.get_edge(vertex->index, i);
        if (edge.active && edge.connected != last) {
            any = true;
            ValidationResult result = validate(edge.connected, vertex);
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

array<FlowNetwork::Vertex *> FlowNetwork::bfs() {
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

        vertex->for_each([&] (Edge& edge) -> void {
            if (!edge.connected->data.visited && edge.data.remaining_flow() > 0) {
                edge.connected->data.visited = true;
                edge.connected->data.path_last = vertex;
                queue.enqueue(edge.connected);
            }
        }, true);
    }

    return array<Vertex*>();
}

void FlowNetwork::build_max_flow() {
    reset_edges();

    while (true) {
        array<Vertex*> path = bfs();
        if (path.length() == 0) {
            break;
        }

        array<Edge*> path_edges;
        for (int i = 0; i < path.length() - 1; i++) {
            path_edges.add(&mGraph.get_edge(*path[i + 1], *path[i]));
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
            path_edges[i]->opposite().data.flow -= min_remaining_flow;
        }
    }
}

int FlowNetwork::get_flow_at(Vertex& vertex) {
    int flow = 0;
    mGraph.for_each([&] (Edge& edge) -> void {
        if (edge.connected == &vertex) {
            flow += edge.data.flow;
        }
        if (edge.from == &vertex) {
            flow -= edge.data.flow;
        }
    });
    return flow;
}

int FlowNetwork::get_flow_at_source() {
    return source != nullptr ? -get_flow_at(*source) : 0;
}

int FlowNetwork::get_flow_at_target() {
    return target != nullptr ? get_flow_at(*target) : 0;
}

FlowNetwork::Vertex &FlowNetwork::get_or_add_vertex(std::string const &name) {
    Vertex* vertex = mGraph.get_vertex(name);
    if (vertex == nullptr) {
        return mGraph.add_vertex(NodeData(name));
    }
    return *vertex;
}

bool FlowNetwork::add_edge_from_string(std::string line) {
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
        if (vertex1.data.name == "T") {
            target = &vertex1;
        }
        if (vertex2.data.name == "S") {
            source = &vertex2;
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

bool FlowNetwork::read(std::string filename) {
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

bool FlowNetwork::save(std::string filename) {
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

void FlowNetwork::print() {
    if (mGraph.size() == 0) {
        std::cout << "flow network is emtpy\n";
        return;
    }
    mGraph.print(9,
                 [] (Vertex& v) -> std::string {
                     return v.data.name;
                 },
                 [] (Edge& edge) -> std::string {
                     char line[64];
                     sprintf(line, "%s->%s %2i/%-2i", edge.from->data.name.data(), edge.connected->data.name.data(), edge.data.flow, edge.data.max_flow);
                     return line;
                 });
}
