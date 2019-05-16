//
// Created by DeepThought on 27.04.2019.
//
#include <iostream>
#include "ref_vector.h"

ref_vector::ref_vector(size_t size, uint32_t init_value) {
    /*if (size <= BORDER) {
        for (size_t i = 0; i < size; i++) {
            data.small[i] = init_value;
        }
    } else {*/
        data.big = new std::vector<uint32_t>(size, init_value);
        pointer = data.big->data();
    //}
    _size = size;
    ref_count = 1;
}

ref_vector::ref_vector(ref_vector const &obj) {
    _size = obj.size();
    ref_count = 1;
    data.big =new std::vector<uint32_t>(*obj.data.big);
    pointer = data.big->data();
}

ref_vector::~ref_vector() {

}

size_t ref_vector::size() const{

    return _size;
}

uint32_t &ref_vector::operator[](size_t i) {
    return pointer[i];
}

void ref_vector::push_back(uint32_t value) {
    data.big->push_back(value);
    pointer = data.big->data();
    _size++;
}

void ref_vector::pop_back() {
    _size--;
    data.big->pop_back();
    pointer = data.big->data();
}

void ref_vector::add_ref() {
    ref_count++;
}

void ref_vector::del_ref() {
    ref_count--;
    if (ref_count == 0) {
        delete this;
    }
}

size_t ref_vector::get_ref() {
    return ref_count;
}

void ref_vector::extend(size_t new_size) {
    if (new_size > _size) {
        new_size -= _size;
        int32_t d = ((int32_t) pointer[_size - 1]) >> 31;
        for (size_t i = 0; i < new_size; i++) {
            push_back(d);
        }
    }
}

void ref_vector::optimize() {
    while(_size > 1 && ((pointer[_size - 1] == 0 && ((int32_t)pointer[_size - 2]) >= 0) || (((int32_t)pointer[_size - 1]) == -1 && ((int32_t)pointer[_size - 2]) < 0))) {
        pop_back();
    }
}

uint32_t &ref_vector::back() {
    return pointer[_size - 1];
}