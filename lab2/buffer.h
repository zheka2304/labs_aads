
#include <string>
#include "array.h"


#ifndef M_BIT_BUFFER_H
#define M_BIT_BUFFER_H

typedef unsigned char byte;
typedef unsigned char bit;

class bit_buffer {
    byte* buffer_memory = nullptr;
    int size = 0;
    int allocated_size = 0;
    int position = 0;

    void ensure_size(int size);
public:

    bit_buffer();
    bit_buffer(bit_buffer const& other);
    bit_buffer(bit_buffer&& other);
    bit_buffer(array<bit> const&);
    bit_buffer& operator=(bit_buffer const&);
    bit_buffer& operator=(bit_buffer&&);

    int length_bytes() const;
    int length_bits() const;
    byte* get_buffer() const;
    void write_bytes(byte const *buffer, int size);

    void write_byte(byte b);
    void write_bit(bit b);

    bit get_bit() const;
    byte get_byte() const;
    bit next_bit();
    byte next_byte();
    void rewind();

    void clear();
    void print() const;

    ~bit_buffer();
};

#endif
