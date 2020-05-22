#include "gtest/gtest.h"
#include "rb_map.h"
#include "array.h"
#include "queue.h"
#include "graph.h"

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
}
