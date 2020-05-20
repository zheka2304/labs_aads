#include "rb_map.h"
#include "array.h"
#include "buffer.h"

#ifndef H_HUFFMAN_H
#define H_HUFFMAN_H

void print_readable_character(char c);

class huffman_tree {
    struct tree_node {
        array<char> characters;
        char character = 0;
        int weight = 0;

        tree_node* left = nullptr;
        tree_node* right = nullptr;

        void build_codes(rb_map<char, bit_buffer>& code_map, array<bit>& code);
        void print_tree(int depth = 0);
        ~tree_node();
    };

    tree_node* build_tree_from_nodes(list<tree_node*>& nodes);

public:
    class char_weights {
    public:
        int weights[256] = {0};

        char_weights();
        char_weights(char_weights const& other);
        char_weights(std::string const& string);
        char_weights(bit_buffer& buffer);
        void read(bit_buffer& buffer);
        void write(bit_buffer& buffer);
    };

private:
    tree_node* root = nullptr;
    char_weights weights;
    rb_map<char, bit_buffer> char_codes;

public:
    huffman_tree(char_weights const& weights);
    ~huffman_tree();
    void encode_string(std::string const &str, bit_buffer &buff);
    std::string decode_string(bit_buffer &buff);
    void write(bit_buffer& buffer);


    int calculate_size();
    void print_codes();
    void print_tree();
};


namespace huffman_codec {
    bit_buffer encode(std::string const& str, bool print_stats = false);
    std::string decode(bit_buffer& buffer);
};

#endif
