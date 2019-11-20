#pragma once

#include "api/matrix.hpp"

class MutableSQIntMatrix: public SQIntMatrix {
public:
    explicit MutableSQIntMatrix(int defValue = 0, size_t initialSize = 0): SQIntMatrix(defValue, initialSize) {}

    int& at(size_t i, size_t j) {
        if (i < getSize() && j < getSize()) {
            return data[i][j];
        }
        throw std::range_error("Out of range " + std::to_string(i) + " or " + std::to_string(j) + " > " + std::to_string(size));
    }

    void extendForIndex(size_t index) {
        if (index < size) return;
        resize(index + 1);
    }

};
