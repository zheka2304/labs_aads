#include "huffman.h"


void print_readable_character(char c) {
    if (c == '\n') {
        std::cout << "'\\n'";
    } else if (c == '\t') {
        std::cout << "'\\t'";
    } else {
        std::cout << c;
    }
}

void print_readable_characters(array<char> const& chars) {
    std::cout << "'";
    for (int i = 0; i < chars.length(); i++) {
        char c = chars[i];
        if (c == '\n') {
            std::cout << "\\n";
        } else if (c == '\t') {
            std::cout << "\\t";
        } else {
            std::cout << c;
        }
    }
    std::cout << "'";
}


// huffman_tree::tree_node

void huffman_tree::tree_node::build_codes(rb_map<char, bit_buffer> &code_map, array<bit> &code) {
    if (character != 0) {
        code_map[character] = code;
    }

    int code_length = code.length();
    if (left != nullptr) {
        code.add(0);
        left->build_codes(code_map, code);
        code.resize(code_length);
    }
    if (right != nullptr) {
        code.add(1);
        right->build_codes(code_map, code);
        code.resize(code_length);
    }
}

huffman_tree::tree_node::~tree_node() {
    delete(left);
    delete(right);
}

void huffman_tree::tree_node::print_tree(int depth) {
    if (left != nullptr) {
        left->print_tree(depth + 1);
    }
    for (int i = 0; i < depth; i++) {
        std::cout << "    ";
    }
    print_readable_characters(characters);
    std::cout << " weight=" << weight << "\n";

    if (right != nullptr) {
        right->print_tree(depth + 1);
    }
}

huffman_tree::tree_node* huffman_tree::build_tree_from_nodes(list<huffman_tree::tree_node*> &nodes) {
    if (nodes.get_length() == 0) {
        return nullptr;
    }
    while (nodes.get_length() > 1) {
        auto min_node_it1 = nodes.end();
        auto min_node_it2 = nodes.end();
        for (auto i = nodes.begin(); i != nodes.end(); i++) {
            tree_node* node = *i;
            if (min_node_it1 == nodes.end() || (*min_node_it1)->weight > node->weight) {
                min_node_it2 = min_node_it1;
                min_node_it1 = i;
            } else if (min_node_it2 == nodes.end() || (*min_node_it2)->weight > node->weight) {
                min_node_it2 = i;
            }
        }
        tree_node* node1 = *min_node_it1;
        tree_node* node2 = *min_node_it2;
        nodes.erase(min_node_it1);
        nodes.erase(min_node_it2);

        tree_node* new_node = *nodes.add(new tree_node());
        new_node->characters.add_all(node1->characters);
        new_node->characters.add_all(node2->characters);
        new_node->weight = node1->weight + node2->weight;
        new_node->left = node1;
        new_node->right = node2;
    }

    return *nodes.begin();
}


// huffman_tree::char_weights

huffman_tree::char_weights::char_weights() = default;

huffman_tree::char_weights::char_weights(const huffman_tree::char_weights &other) {
    memcpy(weights, other.weights, sizeof(int) * 256);
}

huffman_tree::char_weights::char_weights(std::string const &string) {
    for (char c : string) {
        weights[(unsigned char) c]++;
    }
}

void huffman_tree::char_weights::read(bit_buffer &buffer) {
    for (int i = 0; i < 256; i++) {
        weights[i] = (buffer.next_byte() << 24) | (buffer.next_byte() << 16) | (buffer.next_byte() << 8) | buffer.next_byte();
    }
}

void huffman_tree::char_weights::write(bit_buffer &buffer) {
    for (int i = 0; i < 256; i++) {
        int w = weights[i];
        buffer.write_byte(byte((w >> 24) & 0xFF));
        buffer.write_byte(byte((w >> 16) & 0xFF));
        buffer.write_byte(byte((w >> 8) & 0xFF));
        buffer.write_byte(byte(w & 0xFF));
    }
}

huffman_tree::char_weights::char_weights(bit_buffer &buffer) {
    read(buffer);
}


// huffman_tree

huffman_tree::huffman_tree(const huffman_tree::char_weights &weights) : weights(weights) {
    list<tree_node*> nodes;
    for (int i = 0; i < 256; i++) {
        if (weights.weights[i] > 0) {
            tree_node* new_node = *nodes.add(new tree_node());
            new_node->weight = weights.weights[i];
            new_node->character = char(i);
            new_node->characters.add(new_node->character);
        }
    }

    root = build_tree_from_nodes(nodes);
    if (root != nullptr) {
        array<bit> code;
        root->build_codes(char_codes, code);
    }
}

void huffman_tree::print_codes() {
    list<char>& keys = char_codes.keys();
    for (auto i = keys.begin(); i != keys.end(); i++) {
        std::cout << "code for ";
        print_readable_character(*i);
        std::cout << ": ";
        char_codes[*i].print();
        std::cout << "\n";
    }
}

void huffman_tree::write(bit_buffer &buffer) {
    weights.write(buffer);
}

void huffman_tree::encode_string(std::string const &str, bit_buffer &buff) {
    unsigned int length = (unsigned int) str.length();
    buff.write_byte((byte) ((length >> 24) & 0xFF));
    buff.write_byte((byte) ((length >> 16) & 0xFF));
    buff.write_byte((byte) ((length >> 8) & 0xFF));
    buff.write_byte((byte) ((length >> 0) & 0xFF));

    const char* c_str = str.data();
    for (int i = 0; c_str[i]; i++) {
        bit_buffer& code = char_codes[c_str[i]];
        code.rewind();
        for (int j = 0; j < code.length_bits(); j++) {
            buff.write_bit(code.next_bit());
        }
    }
}

std::string huffman_tree::decode_string(bit_buffer &buff) {
    bit_buffer output_string;
    unsigned int length = ((unsigned int) buff.next_byte() << 24) | ((unsigned int) buff.next_byte() << 16) | ((unsigned int) buff.next_byte() << 8) | (unsigned int) buff.next_byte();
    for (int i = 0; i < length; i++) {
        tree_node* node = root;
        while (node->character == 0) {
            if (buff.next_bit()) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        output_string.write_byte(node->character);
    }
    output_string.write_byte(0);
    return std::string((char*) output_string.get_buffer());
}

void huffman_tree::print_tree() {
    root->print_tree();
}

huffman_tree::~huffman_tree() {
    delete(root);
}

int huffman_tree::calculate_size() {
    int size = 0;
    for (int i = 0 ; i < 256; i++) {
        if (char_codes.has(char(i))) {
            size += weights.weights[i] * char_codes[char(i)].length_bits();
        }
    }
    return size;
}


// huffman_codec

bit_buffer huffman_codec::encode(std::string const &str, bool print_stats) {
    int str_size = (int) str.size() * 8;
    if (print_stats) {
        std::cout << "encoder input: " << str << "\n";
        std::cout << "input size (bits): " << str_size << "\n";
    }

    bit_buffer buffer;
    if (str.empty()) {
        buffer.write_bit(0);
        if (print_stats) {
            std::cout << "input is empty" << "\n";
        }
        return buffer;
    } else {
        buffer.write_bit(1);
    }

    huffman_tree tree(str);
    if (print_stats) {
        std::cout << "huffman tree: \n";
        tree.print_tree();
        std::cout << "\n";
        std::cout << "character codes: \n";
        tree.print_codes();
        std::cout << "\n";
        int calc_size = tree.calculate_size();
        std::cout << "theory size (without dictionary): " << calc_size << " " << (calc_size / (double) str_size) * 100 << "%\n";
    }

    tree.write(buffer);
    if (print_stats) {
        std::cout << "leading byte (8) + dictionary size (256 * int32 = 8192): " << buffer.length_bits() << "\n";
    }

    tree.encode_string(str, buffer);
    if (print_stats) {
        std::cout << "total size: " << buffer.length_bits() << " " << (buffer.length_bits() / (double) str_size) * 100 << "%\n";
    }

    return buffer;
}

std::string huffman_codec::decode(bit_buffer &buffer) {
    buffer.rewind();
    if (!buffer.next_bit()) {
        return "";
    }
    huffman_tree tree(buffer);
    return tree.decode_string(buffer);
}
