#pragma once

#include "orderedset.hpp"

template<class T>
class OrderedSetLinkInterface {
private:
  OrderedSet<T> *setA;
  OrderedSet<T> *setB;
public:
  OrderedSetLinkInterface(OrderedSet<T> *a, OrderedSet<T> *b): setA(a), setB(b) { }

  size_t getSize() {
      return aLength() + bLength();
  }

  T& at(size_t index) {
    auto startOfB = aLength();
    if (index < startOfB) {
        return setA->at(index);
    }
    // index < startOfB is always true, so index - startOfB not a UB!
    auto actualIndex = index - startOfB;
    if (actualIndex < bLength()) {
        return setB->at(actualIndex);
    }

    throw std::range_error("OSLI: Index " + std::to_string(index) + " not belongs to any of sets");
  }

  T& operator[](size_t index) {
      return at(index);
  }

  size_t indexOf(T& item) {
      if (setA) {
          auto aIndex = setA->indexOf(item);
          if (aIndex != setA->size())
              return aIndex;
      }
      // Set A not exists or item not in set A
      if (setB) {
          auto bIndex = setB->indexOf(item);
          if (bIndex != setB->size())
              return aLength() + bIndex;
      }

      return getSize();
  }

private:
  size_t aLength() {
      return setA ? setA->size() : 0;
  }

  size_t bLength() {
      return setB ? setB->size() : 0;
  }

};
