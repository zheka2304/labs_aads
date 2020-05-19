#include "gtest/gtest.h"
#include "rb_map.h"
#include "array.h"
#include "graph.h"
#include "pathfinding.h"


// graph tests
TEST (graph, fully_connected_pathfinding) {
    Graph mGraph;

    const int size = 25 * 10;
    for (int i = 0; i < size; i++) {
        char name[] = {char('A' + i % 25), char('A' + (i / 25) % 25), '\0'};
        mGraph.get_or_make_vertex(name);
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mGraph.connect(i, j, 1);
        }
    }

    auto paths = bellman_floid_algorithm::get_shortest_paths(mGraph, *mGraph.get_vertex(0));
    for (int i = 1; i < paths.length(); i++) {
        ASSERT_EQ(paths[i].weight, 1);
        ASSERT_EQ(paths[i].path.length(), 2);
    }
}

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