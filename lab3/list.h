#include <string>


#ifndef M_LIST_H
#define M_LIST_H

template <typename T>
class list {
public:
    class list_node {
    public:
        T value;
        list_node* next = nullptr;
        list_node* prev = nullptr;

        list_node() {};
        list_node(T const& v) : value(v) {}
    };

    class iterator {
    public:
        list_node* node = nullptr;

        iterator() {}
        iterator(list_node* n) : node(n) {}

        iterator operator++(int) {
            list_node* last = node;
            node = node->next;
            return iterator(last);
        }

        iterator operator--(int) {
            list_node* last = node;
            node = node->prev;
            return iterator(last);
        }

        T& operator*() {
            return node->value;
        }

        bool operator==(iterator const& it) {
            return it.node == node;
        }

        bool operator!=(iterator const& it) {
            return it.node != node;
        }
    };

private:
    list_node* first = nullptr;
    list_node* last = nullptr;
    list_node list_end;
    int length = 0;

public:
    list() = default;

    list(list const& other) {
        for (auto it = other.begin(); it != other.end(); it++) {
            add(*it);
        }
    }

    list(list&& other) {
        first = other.first;
        last = other.last;
        length = other.length;
        other.first = other.last = nullptr;
        other.length = 0;
    }

    list& operator= (list const& other) {
        for (auto it = other.begin(); it != other.end(); it++) {
            add(*it);
        }
        return *this;
    }

    list& operator= (list&& other) {
        first = other.first;
        last = other.last;
        length = other.length;
        other.first = other.last = nullptr;
        other.length = 0;
        return *this;
    }

    iterator begin() {
        return first != nullptr ? iterator(first) : end();
    }

    iterator end() {
        return iterator(&list_end);
    }

    iterator add(T const& value) {
        list_node* node = new list_node(value);
        if (last != nullptr) {
            last->next = node;
            node->prev = last;
        } else {
            first = last = node;
        }
        list_end.prev = node;
        node->next = &list_end;
        last = node;
        length++;
        return iterator(node);
    }

    void erase(iterator iterator) {
        list_node* node = iterator.node;
        if (node->next == &list_end) {
            last = list_end.prev = node->prev;
        }
        if (node->prev == nullptr) {
            first = node->next;
        } else {
            node->prev->next = node->next;
        }
        node->next->prev = node->prev;
        length--;
        delete (node);
    }

    void print() {
        std::cout << "[";
        for (auto it = begin(); it != end(); it++) {
            std::cout << *it << ", ";
        }
        std::cout << "]";
    }

    void clear() {
        list_node* node = first;
        while (node != nullptr && node != &list_end) {
            list_node* next = node->next;
            delete(node);
            node = next;
        }
        list_end.next = list_end.prev = nullptr;
        first = last = nullptr;
        length = 0;
    }

    int get_length() {
        return length;
    }

    ~list() {
        clear();
    }
};

#endif