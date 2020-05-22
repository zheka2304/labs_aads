#include "list.h"


#ifndef M_QUEUE_H
#define M_QUEUE_H

template <typename T>
class Queue {
    list<T> mList;
public:
    int size() const {
        return mList.get_length();
    }

    bool empty() const {
        return mList.get_length() == 0;
    }

    T dequeue() {
        auto node = mList._erase(mList.begin());
        T result = std::move(node->value);
        delete(node);
        return result;
    }

    void enqueue(T const &value) {
        mList.add(value);
    }

    void clear() {
        mList.clear();
    }
};

#endif
