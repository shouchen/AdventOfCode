#include "stdafx.h"
#include <cstdlib>
#include "Coordinates.h"

long long Coordinates::dist_from_origin() const
{
    return std::abs(x) + std::abs(y) + std::abs(z);
}

bool Coordinates::operator<(const Coordinates &p) const
{
    if (x < p.x) return true; else if (x > p.x) return false;
    if (y < p.y) return true; else if (y > p.y) return false;
    return z < p.z;
}

bool Coordinates::operator==(const Coordinates &p) const
{
    return x == p.x && y == p.y && z == p.z;
}

std::istream &operator>>(std::istream &is, Coordinates &loc)
{
    auto c = ' ', comma = ',';
    return is >> c >> c >> c >> c >> c >> loc.x >> comma >> loc.y >> comma >> loc.z >> c;
}
