
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

bit_buffer::bit_buffer() = default;

bit_buffer::bit_buffer(bit_buffer const& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    position = other.position;
    buffer_memory = (byte*) malloc(allocated_size);
    memcpy(buffer_memory, other.buffer_memory, size);
}

bit_buffer::bit_buffer(bit_buffer&& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    buffer_memory = other.buffer_memory;
    position = other.position;
    other.size = other.allocated_size = 0;
    other.buffer_memory = nullptr;
}

bit_buffer::bit_buffer(array<bit> const& bits) {
    int len = bits.length();
    for (int i = 0; i < len; i++) {
        write_bit(bits[i]);
    }
}

bit_buffer& bit_buffer::operator=(bit_buffer const& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    position = other.position;
    buffer_memory = (byte*) malloc(allocated_size);
    memcpy(buffer_memory, other.buffer_memory, size);
    return *this;
}

bit_buffer& bit_buffer::operator=(bit_buffer&& other) {
    allocated_size = other.allocated_size;
    size = other.size;
    buffer_memory = other.buffer_memory;
    position = other.position;
    other.size = other.allocated_size = 0;
    other.buffer_memory = nullptr;
    return *this;
}

void bit_buffer::ensure_size(int size) {
    if (size > allocated_size * 8) {
        while (size > allocated_size * 8) {
            if (allocated_size < 2048) {
                allocated_size += 1024;
            } else {
                allocated_size *= 2;
            }
        }
        if (buffer_memory != nullptr) {
            buffer_memory = (byte*) realloc(buffer_memory, allocated_size);
        } else {
            buffer_memory = (byte*) malloc(allocated_size);
        }
    }
}

byte* bit_buffer::get_buffer() const {
    return buffer_memory;
}

int bit_buffer::length_bits() const {
    return size;
}

int bit_buffer::length_bytes() const {
    return (size + 7) / 8;
}

void bit_buffer::write_bytes(byte const *buffer, int size) {
    ensure_size(this->size + size);
    this->size = ((this->size + 7) / 8) * 8;
    memcpy(this->buffer_memory + this->size / 8, buffer, size);
    this->size += size * 8;
}

void bit_buffer::write_bit(bit b) {
    ensure_size(size + 1);
    if (length_bits() * 8 == length_bytes()) {
        write_byte((bit) b);
        size -= 7;
    } else {
        int last = size / 8;
        int pos = size % 8;
        buffer_memory[last] = (byte) ((buffer_memory[last] & ~(1 << pos)) | (b << pos));
        size++;
    }
}

void bit_buffer::clear() {
    if (buffer_memory != nullptr) {
        free(buffer_memory);
        buffer_memory = nullptr;
    }
    size = allocated_size = 0;
}

void bit_buffer::write_byte(byte b) {
    this->size = ((this->size + 7) / 8) * 8;
    ensure_size(size + 8);
    buffer_memory[size / 8] = b;
    size += 8;
}

bit bit_buffer::get_bit() const {
    int byte_pos = position / 8;
    int bit_pos = position % 8;
    return (bit) (((buffer_memory[byte_pos]) >> bit_pos) & 1);
}

byte bit_buffer::get_byte() const {
    return buffer_memory[position / 8];
}

bit bit_buffer::next_bit() {
    bit b = get_bit();
    position++;
    return b;
}

byte bit_buffer::next_byte() {
    position = ((position + 7) / 8) * 8;
    byte b = get_byte();
    position = ((position + 8) / 8) * 8;
    return  b;
}

void bit_buffer::rewind() {
    position = 0;
}

void bit_buffer::print() const {
    if (buffer_memory == nullptr) {
        return;
    }

    for (int i = 0; i < size / 8; i++) {
        byte b = buffer_memory[i];
        for (int j = 0; j < 8; j++) {
            std::cout << ((b >> j) & 1);
        }
    }
    if (size % 8 != 0) {
        byte last = buffer_memory[size / 8];
        for (int j = 0; j < size % 8; j++) {
            std::cout << ((last >> j) & 1);
        }
    }
}

bit_buffer::~bit_buffer() {
    clear();
}