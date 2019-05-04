//
// Created by DeepThought on 27.04.2019.
//

#include "ref_vector.h"

const uint32_t ref_vector::operator[](size_t i) const {
    if (i >= size()) {
        return ((int32_t) std::vector<uint32_t >::operator[](size() - 1)) >> 31;
    }
    return std::vector<uint32_t >::operator[](i);
}

uint32_t& ref_vector::operator[](size_t i) {
    return std::vector<uint32_t>::operator[](i);
}

int ref_vector::add_ref() {
    return ++ref_count;
}

int ref_vector::get_ref() {
    return ref_count;
}

void ref_vector::del_ref() {
    ref_count--;
    if (ref_count == 0) {
        delete this;
    }
}

void ref_vector::optimize() {
    for (size_t i = size() - 1; i > 0; i--) {
        if ((operator[](i) == 0 && (int32_t) operator[](i - 1) >= 0) ||
            (operator[](i) == (uint32_t) -1 && (int32_t) operator[](i - 1) < 0)) {
            pop_back();
            continue;
        }
        break;
    }
}

void ref_vector::extend(size_t new_size) {
    if (size() < new_size) {
        new_size -= size();
        int d = (int32_t)operator[](size() - 1) >> 31;
        for (size_t i = 0; i < new_size; i++) {
            push_back(d);
        }
    }
}
