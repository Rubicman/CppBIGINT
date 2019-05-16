//
// Created by DeepThought on 27.04.2019.
//

#ifndef BIGINT_REF_VECTOR_H
#define BIGINT_REF_VECTOR_H

#include <cstdlib>
#include <vector>
#include <cstdint>

class ref_vector {
private:
    static const size_t BORDER = 16;
    uint32_t *pointer;
    size_t ref_count;
    size_t _size;
    union data_type {
        std::vector<uint32_t> *big;
        uint32_t small[BORDER];
    } data;
public:
    ref_vector(size_t size = 1, uint32_t init_value = 0);

    ref_vector(ref_vector const &obj);

    ~ref_vector();

    size_t size() const;

    uint32_t &operator[](size_t i);

    void push_back(uint32_t value);

    void pop_back();

    void add_ref();

    void del_ref();

    size_t get_ref();

    void extend(size_t new_size);

    void optimize();

    uint32_t &back();
};


#endif //BIGINT_REF_VECTOR_H
