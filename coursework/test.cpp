#include <string>

#include "gtest/gtest.h"
#include "rb_map.h"
#include "array.h"
#include "queue.h"
#include "graph.h"
#include "flow_network.h"


// array tests
TEST (array, massive_add_and_check) {
    array<std::string> arr;
    for (int i = 0; i < 1000000; i++) {
        arr.add(i % 2 ? "abc" : "edf");
    }
    ASSERT_EQ(arr.length(), 1000000);
    for (int i = 0; i < 1000000; i++) {
        ASSERT_EQ(arr[i], i % 2 ? "abc" : "edf");
    }
    arr.clear();
    ASSERT_EQ(arr.length(), 0);
}

// keep hardest test for rb_map
TEST (rb_map, massive_random_load) {
    rb_map<int, int> map;
    for (int i = 0; i < 1000000; i++) {
        map[rand() % 1000000] = rand();
    }
    ASSERT_EQ(map.length(), map.tree_size());

    int remaining_length = map.length();
    for (int i = 0; i < 1000000; i++) {
        if (map.remove(rand() % 1000000)) {
            remaining_length--;
        }
    }

    ASSERT_EQ(map.length(), map.tree_size());
    ASSERT_EQ(map.length(), remaining_length);
}

// queue tests
TEST (queue, queue_and_dequeue) {
    Queue<std::string> queue;
    queue.enqueue("a");
    queue.enqueue("b");
    ASSERT_EQ(queue.size(), 2);
    ASSERT_EQ(queue.dequeue(), "a");
    ASSERT_EQ(queue.dequeue(), "b");
}

// test basic graph functionality
TEST (graph, build_test) {
    Graph<std::string, std::string> graph;
    Graph<std::string, std::string>::Vertex& v1 = graph.add_vertex("A");
    ASSERT_EQ(graph.size(), 1);
    Graph<std::string, std::string>::Vertex& v2 = graph.add_vertex("B");
    ASSERT_EQ(graph.size(), 2);
    ASSERT_EQ(&v1, graph.get_vertex("A"));
    ASSERT_EQ(&v2, graph.get_vertex("B"));
    graph.connect(v1, v2, "Edge");
    ASSERT_EQ(graph.get_edge(v1, v2)->data, "Edge");
    graph.clear();
    ASSERT_EQ(graph.size(), 0);
}

TEST (graph, iteration_test) {
    Graph<int, int> graph;
    for (int i = 0; i < 10; i++) {
        Graph<int, int>::Vertex& v1 = graph.add_vertex(i);
        for (int j = 0; j < i; j++) {
            auto v2 = graph.get_vertex(j);
            ASSERT_NE(v2, nullptr);
            graph.connect(*v2, v1, i * j);
        }
    }

    ASSERT_EQ(graph.size(), 10);
    graph.for_each([] (Graph<int, int>::Edge& edge) -> void {
        ASSERT_LE(edge.from->data, edge.connected->data);
        ASSERT_EQ(edge.data, edge.from->data * edge.connected->data);
    });

//    graph.print(7,
//            [] (int& i) -> std::string {return std::to_string(i);},
//            [] (int& i) -> std::string {return std::to_string(i);});
}


// flow networks test
TEST(flow_network, build) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S T 10"));
    ASSERT_FALSE(network.add_edge_from_string("A 10"));
    ASSERT_EQ(network.mGraph.size(), 2);
    FlowNetwork::Vertex* v1 = network.mGraph.get_vertex("S");
    FlowNetwork::Vertex* v2 = network.mGraph.get_vertex("T");
    ASSERT_NE(v1, nullptr);
    ASSERT_NE(v2, nullptr);
    ASSERT_EQ(v1, network.source);
    ASSERT_EQ(v2, network.target);
    FlowNetwork::Edge* edge1 = network.mGraph.get_edge(*v1, *v2);;
    FlowNetwork::Edge* edge2 = network.mGraph.get_edge(*v2, *v1);
    ASSERT_NE(edge1, nullptr);
    ASSERT_EQ(edge2, nullptr);
    ASSERT_EQ(edge1->data.max_flow, 10);
}

TEST(flow_network, validate_ok) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("S B 2"));
    ASSERT_TRUE(network.add_edge_from_string("A B 3"));
    ASSERT_TRUE(network.add_edge_from_string("B T 4"));
    ASSERT_EQ(network.validate(), FlowNetwork::RESULT_OK);
}

TEST(flow_network, validate_no_terminators) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("S B 2"));
    ASSERT_TRUE(network.add_edge_from_string("A B 3"));
    ASSERT_EQ(network.validate(), FlowNetwork::RESULT_NO_TERMINATIONS);

    network.clear();
    ASSERT_TRUE(network.add_edge_from_string("A T 1"));
    ASSERT_TRUE(network.add_edge_from_string("T B 2"));
    ASSERT_TRUE(network.add_edge_from_string("A B 3"));
    ASSERT_EQ(network.validate(), FlowNetwork::RESULT_NO_TERMINATIONS);
}

TEST(flow_network, validate_cycle) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("T S 2"));
    ASSERT_TRUE(network.add_edge_from_string("A T 3"));
    ASSERT_EQ(network.validate(), FlowNetwork::RESULT_CYCLE);
}

TEST(flow_network, validate_invalid_path) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("T B 3"));
    ASSERT_TRUE(network.add_edge_from_string("A B 2"));
    ASSERT_EQ(network.validate(), FlowNetwork::RESULT_INVALID_PATH);
}

TEST(flow_network, bfs_success) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("A B 1"));
    ASSERT_TRUE(network.add_edge_from_string("B C 1"));
    ASSERT_TRUE(network.add_edge_from_string("B T 1"));
    ASSERT_TRUE(network.add_edge_from_string("C T 1"));
    ASSERT_EQ(network.bfs().length(), 4);

    FlowNetwork::Edge* edge = network.mGraph.get_edge(*network.mGraph.get_vertex("B"), *network.mGraph.get_vertex("T"));
    ASSERT_NE(edge, nullptr);
    edge->data.flow = edge->data.max_flow;
    ASSERT_EQ(network.bfs().length(), 5);
}

TEST(flow_network, bfs_fail) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("A B 1"));
    ASSERT_TRUE(network.add_edge_from_string("C T 1"));
    ASSERT_EQ(network.bfs().length(), 0);
}

TEST(flow_network, max_flow_build) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("A B 1"));
    ASSERT_TRUE(network.add_edge_from_string("B T 1"));
    ASSERT_TRUE(network.add_edge_from_string("S T 2"));
    ASSERT_EQ(network.validate(), FlowNetwork::RESULT_OK);
    network.build_max_flow();
    ASSERT_EQ(network.get_flow_at_source(), 3);
    ASSERT_EQ(network.get_flow_at_target(), 3);
    network.mGraph.for_each([&] (FlowNetwork::Vertex& vertex) -> void {
        if (&vertex != network.source && &vertex != network.target) {
            ASSERT_EQ(network.get_flow_at(vertex), 0);
        }
    });
}

TEST(flow_network, save_and_read) {
    FlowNetwork network;
    ASSERT_TRUE(network.add_edge_from_string("S A 1"));
    ASSERT_TRUE(network.add_edge_from_string("A B 1"));
    ASSERT_TRUE(network.add_edge_from_string("B T 1"));
    ASSERT_TRUE(network.add_edge_from_string("S T 2"));

    std::string filename = "unit_test.tmp";
    if (network.save(filename)) {
        ASSERT_TRUE(network.read(filename));
        ASSERT_EQ(network.validate(), FlowNetwork::RESULT_OK);
        network.build_max_flow();
        ASSERT_EQ(network.get_flow_at_source(), 3);
        ASSERT_EQ(network.get_flow_at_target(), 3);
    } else {
        std::cout << "failed to test save & read: failed to write to file";
    }
}