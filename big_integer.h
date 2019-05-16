//
// Created by DeepThought on 07.04.2019.
//

#ifndef CPPHOMEWORK_BIG_INTEGER_H
#define CPPHOMEWORK_BIG_INTEGER_H

#include <iostream>
#include "ref_vector.h"
#include <cstring>

struct big_integer {
    ref_vector *data;

    void check_data();

    void negate();

    void no();

    big_integer();

    big_integer(int value);

    big_integer(unsigned value);

    explicit big_integer(std::string const &str);

    big_integer(big_integer const &obj);

    ~big_integer();

    big_integer operator-() const;

    big_integer operator+() const;

    big_integer &operator=(big_integer const &b);

    big_integer &operator+=(big_integer const &b);

    big_integer &operator+=(int32_t b);

    big_integer &operator-=(big_integer const &b);

    big_integer &operator*=(big_integer const &b);

    big_integer &operator*=(uint32_t b);

    big_integer &operator*=(int32_t b);

    big_integer &operator/=(big_integer const &b);

    big_integer &operator/=(int32_t b);

    big_integer &operator/=(uint32_t b);

    big_integer &operator%=(big_integer const &b);

    big_integer &operator%=(int32_t b);

    big_integer &operator%=(uint32_t b);

    big_integer &operator<<=(unsigned b);

    big_integer &operator>>=(unsigned b);

    big_integer &operator&=(big_integer const &b);

    big_integer &operator|=(big_integer const &b);

    big_integer &operator^=(big_integer const &b);

    big_integer &operator++();

    const big_integer operator++(int);

    big_integer &operator--();

    const big_integer operator--(int);

    int sign() const;

    int32_t operator%(int32_t b) const;
};

big_integer operator+(big_integer const &a, big_integer const &b);

big_integer operator-(big_integer const &a, big_integer const &b);

big_integer operator*(big_integer const &a, big_integer const &b);

big_integer operator*(big_integer const &a, uint32_t b);

big_integer operator*(uint32_t b, big_integer const &a);

big_integer operator/(big_integer const &a, big_integer const &b);

big_integer operator/(big_integer const &a, int32_t b);

big_integer operator/(big_integer const &a, uint32_t b);

big_integer operator%(big_integer const &a, big_integer const &b);

big_integer operator%(big_integer const &a, uint32_t b);

bool operator==(big_integer const &a, big_integer const &b);

bool operator!=(big_integer const &a, big_integer const &b);

bool operator<(big_integer const &a, big_integer const &b);

bool operator>(big_integer const &a, big_integer const &b);

bool operator<=(big_integer const &a, big_integer const &b);

bool operator>=(big_integer const &a, big_integer const &b);

big_integer operator&(big_integer const &a, big_integer const &b);

big_integer operator|(big_integer const &a, big_integer const &b);

big_integer operator^(big_integer const &a, big_integer const &b);

big_integer operator~(big_integer const &a);

big_integer operator<<(big_integer const &a, unsigned b);

big_integer operator>>(big_integer const &a, unsigned b);

std::string to_string(big_integer const &a);

#endif //CPPHOMEWORK_BIG_INTEGER_H
