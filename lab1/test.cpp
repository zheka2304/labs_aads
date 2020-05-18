#include "gtest/gtest.h"
#include "rb_map.h"

TEST (rb_map, fill_and_check_length) {
    rb_map<int, int> map;
    map[0] = 1;
    map[10] = 2;
    map[3] = 3;
    map[10] = -4;
    ASSERT_EQ(map.length(), 3);
    ASSERT_EQ(map.tree_size(), 3);
    ASSERT_EQ(map.keys().get_length(), 3);
    ASSERT_EQ(map.values().get_length(), 3);
}

TEST (rb_map, fill_and_check_elems) {
    rb_map<int, int> map;
    for (int i = 0; i < 10; i++) {
        map[i] = i * 2;
    }
    for (int i = 14; i >= 5; i--) {
        map[i] = i * 10;
    }
    ASSERT_EQ(map.length(), 15);
    ASSERT_EQ(map.tree_size(), 15);

    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(map[i], i * 2);
    }
    for (int i = 14; i >= 5; i--) {
        ASSERT_EQ(map[i], i * 10);
    }
}

TEST (rb_map, remove_test) {
    rb_map<int, int> map;
    for (int i = 0; i < 100; i++) {
        map[i] = i;
    }
    ASSERT_EQ(map.length(), 100);
    ASSERT_EQ(map.tree_size(), 100);
    for (int i = 0; i < 100; i++) {
        map.remove(i);
    }
    ASSERT_EQ(map.length(), 0);
    ASSERT_EQ(map.tree_size(), 0);
    for (int i = 0; i < 200; i++) {
        map[i] = i;
    }
    ASSERT_EQ(map.length(), 200);
    ASSERT_EQ(map.tree_size(), 200);
    for (int i = 199; i >= 0; i--) {
        map.remove(i);
    }
    ASSERT_EQ(map.length(), 0);
    ASSERT_EQ(map.tree_size(), 0);

}

TEST (rb_map, excess_remove_test) {
    rb_map<int, int> map;
    for (int i = 0; i < 100; i++) {
        map[i] = i;
    }
    for (int i = 50; i < 200; i++) {
        ASSERT_EQ(map.remove(i), i < 100);
    }
    ASSERT_EQ(map.length(), 50);
    ASSERT_EQ(map.tree_size(), 50);
}

TEST (rb_map, find_and_has) {
    rb_map<int, int> map;
    for (int i = 0; i < 100; i++) {
        map[i] = i;
    }
    for (int i = 0; i < 200; i++) {
        ASSERT_EQ(map.has(i), i < 100);
        ASSERT_EQ(map.find(i) != nullptr, i < 100);
    }
}

TEST (rb_map, clear) {
    rb_map<int, int> map;
    map.clear();
    ASSERT_EQ(map.length(), 0);
    ASSERT_EQ(map.tree_size(), 0);
    ASSERT_EQ(map.keys().get_length(), 0);

    for (int i = 0; i < 200; i++) {
        map[i] = i;
    }
    ASSERT_EQ(map.length(), 200);
    ASSERT_EQ(map.tree_size(), 200);
    ASSERT_EQ(map.keys().get_length(), 200);

    map.clear();
    ASSERT_EQ(map.length(), 0);
    ASSERT_EQ(map.tree_size(), 0);
    ASSERT_EQ(map.keys().get_length(), 0);
}

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