#include "big_integer.h"

big_integer::big_integer() : data(new ref_vector(1, 0)) {}

big_integer::big_integer(int value) : data(new ref_vector(1, value)) {}

big_integer::big_integer(unsigned value) : data(new ref_vector(1, value)) {
    if ((int) value < 0) {
        data->push_back(0);
    }
}

big_integer::big_integer(big_integer const &obj) : data(obj.data) {
    data->add_ref();
}

big_integer::~big_integer() {
    data->del_ref();
}

big_integer &big_integer::operator=(big_integer const &b) {
    data->del_ref();
    data = b.data;
    data->add_ref();
    return *this;
}

void big_integer::check_data() {
    if (data->get_ref() > 1) {
        ref_vector *new_data = new ref_vector(*data);
        data->del_ref();
        data = new_data;
    }
}

big_integer &big_integer::operator+=(int32_t b) {
    check_data();
    data->extend(data->size() + 1);
    int32_t carry = b;
    int64_t result;
    for (size_t i = 0; i < data->size(); i++) {
        result = (int64_t) (*data)[i] + carry;
        (*data)[i] = result;
        carry = result >> 32;
    }
    data->optimize();
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &b) {
    check_data();
    data->extend(b.data->size() + 1);
    uint32_t carry = 0;
    uint64_t result;
    for (size_t i = 0; i < b.data->size(); i++) {
        result = (int64_t) (*data)[i] + (*b.data)[i] + carry;
        (*data)[i] = result;
        carry = (result >> 32);
    }
    for (size_t i = b.data->size(); i < data->size(); i++) {
        result = (uint64_t) (*data)[i] + carry + (uint32_t) (b.sign() >> 1);
        (*data)[i] = result;
        carry = (result >> 32);
    }
    data->optimize();
    return *this;
}

big_integer operator+(big_integer const &a, big_integer const &b) {
    big_integer c(a);
    c += b;
    return c;
}

big_integer &big_integer::operator*=(uint32_t b) {
    check_data();
    bool positive = true;
    if ((int32_t) (*data)[data->size() - 1] < 0) {
        negate();
        positive = !positive;
    }
    data->extend(data->size() + 1);
    uint32_t carry = 0;
    uint64_t result;
    for (size_t i = 0; i < data->size(); i++) {
        result = (uint64_t) (*data)[i] * b + carry;
        (*data)[i] = result;
        carry = result >> 32;
    }
    if (!positive) {
        negate();
    }
    data->optimize();
    return *this;
}

big_integer& big_integer::operator*=(int32_t b) {
    if (b < 0) {
        *this *= (uint32_t)(-b);
        negate();
    } else {
        *this *= (uint32_t) b;
    }
    return *this;
}

big_integer &big_integer::operator/=(int32_t b) {
    check_data();
    int32_t carry = 0;
    int64_t result;
    uint32_t c = b;
    bool positive = true;
    if (b < 0) {
        positive = !positive;
        c = -b;
    }
    if ((int32_t) (*data)[data->size() - 1] < 0) {
        negate();
        positive = !positive;
    }
    for (size_t i = data->size(); i--;) {
        result = ((int64_t) carry << 32) + (*data)[i];
        (*data)[i] = result / c;
        carry = result % b;
    }
    if (!positive) {
        negate();
    }
    data->optimize();
    return *this;
}

big_integer &big_integer::operator/=(uint32_t b) {
    check_data();
    uint32_t carry = 0;
    uint64_t result;
    bool positive = true;
    if ((int32_t) (*data)[data->size() - 1] < 0) {
        negate();
        positive = !positive;
    }
    for (size_t i = data->size(); i--;) {
        result = ((int64_t) carry << 32) + (*data)[i];
        (*data)[i] = result / b;
        carry = result % b;
    }
    if (!positive) {
        negate();
    }
    data->optimize();
    return *this;
}

big_integer operator/(big_integer const& a, int32_t b) {
    big_integer c(a);
    c /= b;
    return c;
}

big_integer operator/(big_integer const& a, uint32_t b) {
    big_integer c(a);
    c /= b;
    return c;
}

big_integer &big_integer::operator%=(uint32_t b) {
    check_data();
    int32_t carry = 0;
    int64_t result;
    bool positive = true;
    if ((int32_t) (*data)[data->size() - 1] < 0) {
        negate();
        positive = false;
    }
    for (size_t i = data->size(); i--;) {
        result = ((int64_t) carry << 32) + (*data)[i];
        carry = result % b;
    }
    data->optimize();
    if ((int32_t) (*data)[data->size() - 1] < 0) {
        carry = -carry;
    }
    if (!positive) {
        carry = -carry;
    }
    ref_vector *new_data = new ref_vector(1, carry);
    data->del_ref();
    data = new_data;
    return *this;
}

big_integer& big_integer::operator%=(int32_t b) {
    return *this %= (uint32_t) abs(b);
}

int32_t big_integer::operator%(int32_t b) const {
    big_integer c(*this);
    c %= (uint32_t)abs(b);
    return (*c.data)[0];
}

void big_integer::negate() {
    check_data();
    data->extend(data->size() + 1);
    for (size_t i = 0; i < data->size(); i++) {
        (*data)[i] = ~(*data)[i];
    }
    operator+=(1);
    data->optimize();
}

int big_integer::sign() const {
    if (data->size() == 1 && (*data)[0] == 0) {
        return 0;
    }
    if (((int32_t) (*data)[data->size() - 1]) >= 0) {
        return 1;
    }
    return -1;
}

big_integer::big_integer(std::string const &str) : data(new ref_vector(1, 0)) {
    if (str.empty() || str == "-") {
        throw std::invalid_argument("str must contain numbers and can contain '-' at beginning");
    }
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '-' && i == 0) {
            continue;
        }
        if (str[i] < '0' || str[i] > '9') {
            throw std::invalid_argument("str must contain numbers and can contain '-' at beginning");
        }
        operator*=(10);
        operator+=(str[i] - '0');
    }
    if (str[0] == '-') {
        negate();
    }
}

big_integer big_integer::operator-() const {
    big_integer c(*this);
    c.negate();
    return c;
}

std::string to_string(big_integer const &a) {
    big_integer c(a);
    std::string result;
    bool positive = true;
    if (c.sign() == -1) {
        c.negate();
        positive = false;
    }
    while (c.sign() != 0) {
        result = (char) ('0' + c % 10) + result;
        c /= (uint32_t)10;
    }
    if (result.empty()) {
        result = "0";
    }
    if (!positive) {
        result = "-" + result;
    }
    return result;
}

big_integer &big_integer::operator-=(big_integer const &b) {
    check_data();
    this->negate();
    *this += b;
    this->negate();
    return *this;
}

big_integer operator-(big_integer const &a, big_integer const &b) {
    big_integer c(a);
    c -= b;
    return c;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer &big_integer::operator>>=(unsigned b) {
    check_data();
    if (b / 32 != 0) {
        if (b / 32 >= data->size()) {
            auto *new_data = new ref_vector(1, sign() >> 1);
            data->del_ref();
            data = new_data;
            return (*this);
        } else {
            for (size_t i = 0; i < data->size() - b / 32; i++) {
                (*data)[i] = (*data)[i + b / 32];
            }
            for (size_t i = 0; i < b / 32; i++) {
                data->pop_back();
            }
        }
    }
    b %= 32;
    if (b != 0) {
        uint32_t c, carry = ((*data)[data->size() - 1]) % (1 << b);
        (*data)[data->size() - 1] = (int32_t) (*data)[data->size() - 1] >> b;
        for (size_t i = data->size() - 1; i--;) {
            c = ((*data)[i] >> b) + (carry << (32 - b));
            carry = (*data)[i] << (32 - b) >> (32 - b);
            (*data)[i] = c;
        }
    }
    data->optimize();
    return *this;
}

big_integer operator>>(big_integer const &a, unsigned b) {
    big_integer c(a);
    c >>= b;
    return c;
}

big_integer &big_integer::operator<<=(unsigned b) {
    check_data();
    data->extend(data->size() + b / 32);
    if (b / 32 > 0) {
        for (size_t i = data->size() - 1; i >= b / 32; i--) {
            (*data)[i] = (*data)[i - b / 32];
        }
        for (size_t i = b / 32; i--;) {
            (*data)[i] = 0;
        }
    }
    b %= 32;
    if (b > 0) {
        data->extend(data->size() + 1);
        (*data)[data->size() - 1] = (int32_t) (*data)[data->size() - 2] >> (32 - b);
        for (size_t i = data->size() - 1; i > 0; i--) {
            (*data)[i] = ((*data)[i] << b) + ((*data)[i - 1] >> (32 - b));
        }
        (*data)[0] = (*data)[0] << b;
    }
    data->optimize();
    return *this;
}

big_integer operator<<(big_integer const &a, unsigned b) {
    big_integer c(a);
    c <<= b;
    return c;
}

bool operator==(big_integer const &a, big_integer const &b) {
    if (a.data->size() != b.data->size()) {
        return false;
    }
    for (size_t i = 0; i < std::max(a.data->size(), b.data->size()); i++) {
        if ((*a.data)[i] != (*b.data)[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign() < b.sign()) {
        return true;
    }
    if (a.sign() > b.sign()) {
        return false;
    }
    if (a.data->size() > b.data->size()) {
        return a.sign() == -1;
    }
    if (a.data->size() < b.data->size()) {
        return a.sign() != -1;
    }
    for (size_t i = a.data->size(); i--;) {
        if ((*a.data)[i] < (*b.data)[i]) {
            return a.sign() != -1;
        }
        if ((*a.data)[i] > (*b.data)[i]) {
            return a.sign() == -1;
        }
    }
    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return b < a;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(b < a);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

big_integer &big_integer::operator*=(big_integer const &b) {
    big_integer x(*this), y(b);
    auto *new_data = new ref_vector(1, 0);
    data->del_ref();
    data = new_data;
    bool positive = true;
    if (y.sign() == -1) {
        positive = !positive;
        y.negate();
    }
    if (x.sign() == -1) {
        positive = !positive;
        x.negate();
    }
    for (unsigned i = 0; i < y.data->size(); i++) {
        operator+=((x * (*y.data)[i]) << (32 * i));
    }
    data->optimize();
    if (!positive) {
        negate();
    }
    return *this;
}

big_integer operator*(big_integer const &a, uint32_t b) {
    big_integer c(a);
    c *= b;
    return c;
}

big_integer operator*(uint32_t b, big_integer const &a) {
    big_integer c(a);
    c *= b;
    return c;
}

big_integer operator*(big_integer const &a, big_integer const &b) {
    big_integer c(a);
    c *= b;
    return c;
}

void big_integer::no() {
    check_data();
    for (size_t i = 0; i < data->size(); i++) {
        (*data)[i] = ~(*data)[i];
    }
}

big_integer operator~(big_integer const &a) {
    big_integer c(a);
    c.no();
    return c;
}

big_integer &big_integer::operator&=(big_integer const &b) {
    check_data();
    data->extend(b.data->size());
    for (size_t i = 0; i < b.data->size(); i++) {
        (*data)[i] &= (*b.data)[i];
    }
    for (size_t i = b.data->size(); i < data->size(); i++) {
        (*data)[i] &= (b.sign() >> 1);
    }
    return *this;
}

big_integer operator&(big_integer const &a, big_integer const &b) {
    big_integer c(a);
    c &= b;
    return c;
}

big_integer &big_integer::operator|=(big_integer const &b) {
    check_data();
    data->extend(b.data->size());
    for (size_t i = 0; i < b.data->size(); i++) {
        (*data)[i] |= (*b.data)[i];
    }
    for (size_t i = b.data->size(); i < data->size(); i++) {
        (*data)[i] |= (b.sign() >> 1);
    }
    return *this;
}

big_integer operator|(big_integer const &a, big_integer const &b) {
    big_integer c(a);
    c |= b;
    return c;
}

big_integer &big_integer::operator^=(big_integer const &b) {
    check_data();
    data->extend(b.data->size());
    for (size_t i = 0; i < b.data->size(); i++) {
        (*data)[i] ^= (*b.data)[i];
    }
    for (size_t i = b.data->size(); i < data->size(); i++) {
        (*data)[i] ^= (b.sign() >> 1);
    }
    return *this;
}

big_integer operator^(big_integer const &a, big_integer const &b) {
    big_integer c(a);
    c ^= b;
    return c;
}

big_integer& big_integer::operator/=(big_integer const &b) {
    big_integer x(*this), y(b);
    data = new ref_vector(1, 0);
    x.data->del_ref();
    bool positive = true;
    if (x.sign() == -1) {
        x.negate();
        positive = !positive;
    }
    if (y.sign() == -1) {
        y.negate();
        positive = !positive;
    }
    if (y.data->size() > x.data->size()) {
        data->optimize();
        return *this;
    }
    if (y.data->size() == 1) {
        *this = x / (int32_t)(*y.data)[0];
    } else {
        uint32_t c = y.data->back();
        if (c == 0) {
            c = (*y.data)[y.data->size() - 2];
        }
        auto f = uint32_t ((1ull << 32u) / ((uint64_t) (c + 1)));
        x *= f;
        y *= f;
        c = y.data->back();
        size_t n = y.data->size();
        size_t m = x.data->size() - y.data->size();
        if (c == 0) {
            c = (*y.data)[y.data->size() - 2];
            n--;
            m++;
        }
        data->extend(m + 1);
        if (x >= (y << 32 * m)) {
            (*data)[m] = 1;
            x -= (y << 32 * m);
        }
        big_integer shift_y(y << (32 * m));
        for (int i = m - 1; i >= 0; i--) {
            x.data->extend(n + i + 1);
            shift_y >>= 32;
            uint64_t q64 = (((uint64_t) (*x.data)[n + i] << 32u) + (*x.data)[n + i - 1]) / c;
            uint32_t q = std::min(q64, (uint64_t)UINT32_MAX);
            while (shift_y * q > x) {
                q--;
            }
            x -= shift_y * q;
            (*data)[i] = q;
        }
    }
    if (!positive) {
        negate();
    }
    data->optimize();
    return *this;
}


big_integer operator/(big_integer const& a, big_integer const& b) {
    big_integer c(a);
    c /= b;
    return c;
}

big_integer& big_integer::operator%=(big_integer const &b) {
    *this -= (*this / b) * b;
    return *this;
}

big_integer operator%(big_integer const& a, big_integer const& b) {
    big_integer c(a);
    c %= b;
    return c;
}

big_integer& big_integer::operator++() {
    *this += 1;
    return *this;
}

const big_integer big_integer::operator++(int) {
    big_integer c(*this);
    *this += 1;
    return c;
}

big_integer& big_integer::operator--() {
    *this -= 1;
    return *this;
}

const big_integer big_integer::operator--(int) {
    big_integer c(*this);
    *this -= 1;
    return c;
}

