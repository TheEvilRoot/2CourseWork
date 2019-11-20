#pragma once

#include <vector>
#include <algorithm>

template <class T>
class OrderedSet {
private:
    std::vector<T> data;

public:
    explicit OrderedSet(size_t initialSize = 0) { data.resize(initialSize); }

    size_t push(T &item) {
        size_t itemInData = data.size();
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i] == item) { itemInData = i; break; }
        }

        if (itemInData == data.size()) {
            size_t index = data.size();
            data.push_back(item);
            return index;
        }
        return itemInData;
    }

    T& at(size_t index) { return data[index]; }

    T& operator[](size_t index) { return at(index); }

    size_t size() { return data.size(); }

    size_t indexOf(T &item) {
        for (size_t i = 0; i < size(); i++) {
            if (at(i) == item) return i;
        }
        return size();
    }
};
