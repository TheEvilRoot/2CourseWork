#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdexcept>

class SQIntMatrix {
protected:
    size_t size;
    size_t allocSize;
    int defaultValue;

    int **data;
public:
    explicit SQIntMatrix(int defValue = 0, size_t initialSize = 0):
        size(0),
        allocSize(0),
        defaultValue(defValue),
        data(nullptr) { if (initialSize) resize(initialSize); }

    ~SQIntMatrix() {
        if (data) {
            for (size_t i = 0; i < allocSize; i++) {
                if (data[i]) delete[] data[i];
            }
            delete[] data;
        }
    }

    int get(size_t i, size_t j) {
        if (i < size && j < size) {
            return data[i][j];
        }
        throw std::range_error("Out of range " + std::to_string(i) + " or " + std::to_string(j) + " > " + std::to_string(size));
    }

    size_t getSize() { return size; }

protected:
    void alloc(size_t newSize) {
        if (data == nullptr) {
            data = static_cast<int**>(calloc(newSize, sizeof(int*)));
        } else {
            data = static_cast<int**>(realloc(data, newSize * sizeof(int*)));
        }

        for (size_t i = 0; i < newSize; i++) {
            if (i >= allocSize || data[i] == nullptr) {
                data[i] = static_cast<int*>(calloc(newSize, sizeof(int)));
            } else {
                data[i] = static_cast<int*>(realloc(data[i], newSize * sizeof(int)));
            }
        }
        allocSize = newSize;
    }

    void resize(size_t newSize) {
        if (newSize > allocSize) alloc(newSize);

        size_t oldSize = size;
        size = newSize;

        if (newSize > oldSize) {
            for (size_t i = 0; i < size; i++) {
                for (size_t j = (i <= oldSize ? oldSize : 0); j < size; j++) {
                    data[i][j] = defaultValue;
                }
            }
        }
    }

};
