#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

class Int128 {
private:
    uint64_t low;
    int64_t high;

    Int128(uint64_t lo, int64_t hi);

    bool zero() const;
    bool neg() const;
    bool bit(int p) const;
    void set(int p);
    void shl1();

    static int cmpu(const Int128& a, const Int128& b);
    static Int128 subu(const Int128& a, const Int128& b);
    static Int128 divu(const Int128& a, const Int128& b, Int128* rem);

public:
    Int128();
    Int128(int64_t value);
    Int128(std::string_view str);

    explicit operator int64_t() const;
    explicit operator double() const;

    std::string str() const;

    Int128 operator+(const Int128& other) const;
    Int128& operator+=(const Int128& other);

    Int128 operator-(const Int128& other) const;
    Int128& operator-=(const Int128& other);

    Int128 operator*(const Int128& other) const;
    Int128& operator*=(const Int128& other);

    Int128 operator/(const Int128& other) const;
    Int128& operator/=(const Int128& other);

    Int128 operator-() const;

    bool operator==(const Int128& other) const;
    bool operator!=(const Int128& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Int128& v);
};