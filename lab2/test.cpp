#include "gtest/gtest.h"
#include "rb_map.h"
#include "array.h"
#include "huffman.h"


// encoder test
TEST (codec, encode_and_decode) {
    std::string encode_string;
    for (int i = 0; i < 1000; i++) {
        bit_buffer buff = huffman_codec::encode(encode_string);
        ASSERT_EQ(huffman_codec::decode(buff), encode_string);
        char new_chars[] = {i % 2 == 0 ? 'a' : char(rand() % 254 + 1), '\0'};
        encode_string += new_chars;
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