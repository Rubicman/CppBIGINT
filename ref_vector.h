//
// Created by DeepThought on 27.04.2019.
//

#ifndef BIGINT_REF_VECTOR_H
#define BIGINT_REF_VECTOR_H

#include <vector>
#include <cstdint>

class ref_vector:public std::vector<uint32_t> {
    int ref_count;
public:
    explicit ref_vector(size_t size = 0, uint32_t init_value = 0) : std::vector<uint32_t >(size, init_value) {ref_count = 1;}
    ref_vector(ref_vector const& obj): std::vector<uint32_t>(obj) {ref_count = 1;}
    uint32_t const operator[](size_t i) const;
    uint32_t& operator[](size_t i);
    int add_ref();
    void del_ref();
    int get_ref();
    void optimize();
    void extend(size_t new_size);
};


#endif //BIGINT_REF_VECTOR_H
