#pragma once
#include <iostream>

struct Coordinates
{
    long long x, y, z;

    Coordinates(long long x = 0, long long y = 0, long long z = 0) : x(x), y(y), z(z) {}
    long long dist_from_origin() const;
    bool operator<(const Coordinates &p) const;
    bool operator==(const Coordinates &p) const;
};

std::istream &operator>>(std::istream &is, Coordinates &loc);