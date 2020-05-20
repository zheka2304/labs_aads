#include <iostream>
#include <fstream>
#include <string>

#include "huffman.h"


int main() {
    std::string input;
    std::cout << "input string or filename to encode: ";
    std::getline(std::cin, input);

    std::ifstream file;
    file.open(input);
    if (file) {
        input = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    bit_buffer buff = huffman_codec::encode(input, true);
    std::cout << "\n\ndecoder output: " << huffman_codec::decode(buff);

    return 0;
}