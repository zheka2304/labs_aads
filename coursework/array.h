#include <iostream>


#ifndef M_ARRAY_H
#define M_ARRAY_H

template <typename T>
class array {
    T* allocated_memory = nullptr;
    int allocated_memory_size = 0;

    int size = 0;

    void ensure_size(int size) {
        if (allocated_memory_size < size) {
            int old_size = allocated_memory_size;
            while (allocated_memory_size < size) {
                if (allocated_memory_size < 32) {
                    allocated_memory_size += 16;
                } else {
                    allocated_memory_size *= 2;
                }
            }

            if (allocated_memory != nullptr) {
                T* new_memory = new T[allocated_memory_size];
                for (int i = 0; i < old_size; i++) {
                    new_memory[i] = allocated_memory[i];
                }
                delete[] (allocated_memory);
                allocated_memory = new_memory;
            } else {
                allocated_memory = new T[allocated_memory_size];
            }
        }
    }

public:
    array() = default;

    array(int size) {
        resize(size);
    }

    array(array<T> const& other) {
        ensure_size(other.size);
        size = other.size;
        for (int i = 0; i < size; i++) {
            allocated_memory[i] = other.allocated_memory[i];
        }
    }

    array(array<T>&& other) noexcept {
        allocated_memory = other.allocated_memory;
        size = other.size;
        allocated_memory_size = other.allocated_memory_size;
        other.allocated_memory = nullptr;
        other.size = 0;
        other.allocated_memory_size = 0;
    }

    array<T>& operator= (array<T> const& other) {
        ensure_size(other.size);
        size = other.size;
        for (int i = 0; i < size; i++) {
            allocated_memory[i] = other.allocated_memory[i];
        }
        return *this;
    }

    array<T>& operator= (array<T>&& other) noexcept {
        allocated_memory = other.allocated_memory;
        size = other.size;
        allocated_memory_size = other.allocated_memory_size;
        other.allocated_memory = nullptr;
        other.size = 0;
        other.allocated_memory_size = 0;
        return *this;
    }

    int length() const {
        return size;
    }

    T& operator[] (int index) {
        return allocated_memory[index];
    }

    T const& operator[] (int index) const {
        return allocated_memory[index];
    }

    void resize(int new_size) {
        if (new_size == 0) {
            clear();
        } else {
            ensure_size(new_size);
            size = new_size;
        }
    }

    T& add(T const& elem) {
        ensure_size(size + 1);
        return allocated_memory[size++] = elem;
    }

    void add_all(array<T> const& arr) {
        for (int i = 0; i < arr.size; i++) {
            add(arr[i]);
        }
    }

    void clear() {
        if (allocated_memory != nullptr) {
            delete[] (allocated_memory);
            allocated_memory = nullptr;
        }
        size = allocated_memory_size = 0;
    }

    ~array() {
        clear();
    }
};

#endif