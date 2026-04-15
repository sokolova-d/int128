#include "int128.hpp"

#include <algorithm>
#include <cmath>

Int128::Int128() : low(0), high(0) {
}

Int128::Int128(uint64_t lo, int64_t hi)
    : low(lo), high(hi) {
}

Int128::Int128(int64_t value) {
    low = static_cast<uint64_t>(value);
    high = (value < 0 ? -1 : 0);
}

Int128::Int128(std::string_view str) : low(0), high(0) {
    bool n = false;
    std::size_t i = 0;

    if (!str.empty() && str[0] == '-') {
        n = true;
        i = 1;
    } else if (!str.empty() && str[0] == '+') {
        i = 1;
    }

    for (; i < str.size(); ++i) {
        char c = str[i];
        if (c < '0' || c > '9') {
            continue;
        }
        *this *= Int128(10);
        *this += Int128(static_cast<int64_t>(c - '0'));
    }

    if (n) {
        *this = -*this;
    }
}

bool Int128::zero() const {
    return low == 0 && high == 0;
}

bool Int128::neg() const {
    return high < 0;
}

bool Int128::bit(int p) const {
    if (p < 64) {
        return ((low >> p) & 1ULL) != 0;
    }
    uint64_t hi = static_cast<uint64_t>(high);
    return ((hi >> (p - 64)) & 1ULL) != 0;
}

void Int128::set(int p) {
    if (p < 64) {
        low |= (1ULL << p);
    } else {
        uint64_t hi = static_cast<uint64_t>(high);
        hi |= (1ULL << (p - 64));
        high = static_cast<int64_t>(hi);
    }
}

void Int128::shl1() {
    uint64_t hi = static_cast<uint64_t>(high);
    hi = (hi << 1) | (low >> 63);
    low <<= 1;
    high = static_cast<int64_t>(hi);
}

int Int128::cmpu(const Int128& a, const Int128& b) {
    uint64_t ah = static_cast<uint64_t>(a.high);
    uint64_t bh = static_cast<uint64_t>(b.high);

    if (ah < bh) {
        return -1;
    }
    if (ah > bh) {
        return 1;
    }
    if (a.low < b.low) {
        return -1;
    }
    if (a.low > b.low) {
        return 1;
    }
    return 0;
}

Int128 Int128::subu(const Int128& a, const Int128& b) {
    Int128 r;
    r.low = a.low - b.low;

    uint64_t ah = static_cast<uint64_t>(a.high);
    uint64_t bh = static_cast<uint64_t>(b.high);
    uint64_t br = (a.low < b.low ? 1ULL : 0ULL);

    uint64_t rh = ah - bh - br;
    r.high = static_cast<int64_t>(rh);

    return r;
}

Int128 Int128::divu(const Int128& a, const Int128& b, Int128* rem) {
    Int128 q(0);
    Int128 r(0);

    for (int i = 127; i >= 0; --i) {
        r.shl1();
        if (a.bit(i)) {
            r.low |= 1ULL;
        }

        if (cmpu(r, b) >= 0) {
            r = subu(r, b);
            q.set(i);
        }
    }

    if (rem != nullptr) {
        *rem = r;
    }

    return q;
}

Int128::operator int64_t() const {
    return static_cast<int64_t>(low);
}

Int128::operator double() const {
    if (zero()) {
        return 0.0;
    }

    if (neg()) {
        Int128 t = -(*this);
        uint64_t hi = static_cast<uint64_t>(t.high);
        return -(std::ldexp(static_cast<double>(hi), 64) + static_cast<double>(t.low));
    }

    uint64_t hi = static_cast<uint64_t>(high);
    return std::ldexp(static_cast<double>(hi), 64) + static_cast<double>(low);
}

std::string Int128::str() const {
    if (zero()) {
        return "0";
    }

    bool n = neg();
    Int128 t = n ? -(*this) : *this;
    Int128 ten(10);

    std::string s;
    while (!t.zero()) {
        Int128 rem;
        Int128 q = divu(t, ten, &rem);
        s.push_back(static_cast<char>('0' + static_cast<int64_t>(rem)));
        t = q;
    }

    if (n) {
        s.push_back('-');
    }

    std::reverse(s.begin(), s.end());
    return s;
}

Int128 Int128::operator+(const Int128& other) const {
    Int128 r;

    r.low = low + other.low;
    uint64_t c = (r.low < low ? 1ULL : 0ULL);

    uint64_t h1 = static_cast<uint64_t>(high);
    uint64_t h2 = static_cast<uint64_t>(other.high);
    uint64_t rh = h1 + h2 + c;

    r.high = static_cast<int64_t>(rh);
    return r;
}

Int128& Int128::operator+=(const Int128& other) {
    *this = *this + other;
    return *this;
}

Int128 Int128::operator-(const Int128& other) const {
    return *this + (-other);
}

Int128& Int128::operator-=(const Int128& other) {
    *this = *this - other;
    return *this;
}

Int128 Int128::operator-() const {
    Int128 r;
    r.low = ~low + 1ULL;

    uint64_t h = ~static_cast<uint64_t>(high);
    if (r.low == 0) {
        h += 1ULL;
    }
    r.high = static_cast<int64_t>(h);

    return r;
}

Int128 Int128::operator*(const Int128& other) const {
    bool n = (neg() != other.neg());

    Int128 a = neg() ? -(*this) : *this;
    Int128 b = other.neg() ? -other : other;

    Int128 r(0);

    for (int i = 0; i < 128; ++i) {
        if (b.bit(i)) {
            r += a;
        }
        a.shl1();
    }

    if (n) {
        r = -r;
    }

    return r;
}

Int128& Int128::operator*=(const Int128& other) {
    *this = *this * other;
    return *this;
}

Int128 Int128::operator/(const Int128& other) const {
    bool n = (neg() != other.neg());

    Int128 a = neg() ? -(*this) : *this;
    Int128 b = other.neg() ? -other : other;

    Int128 r = divu(a, b, nullptr);

    if (n) {
        r = -r;
    }

    return r;
}

Int128& Int128::operator/=(const Int128& other) {
    *this = *this / other;
    return *this;
}

bool Int128::operator==(const Int128& other) const {
    return low == other.low && high == other.high;
}

bool Int128::operator!=(const Int128& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Int128& v) {
    os << v.str();
    return os;
}