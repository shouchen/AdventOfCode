#include "stdafx.h"
#include <cstdlib>

#include "Nanobot.h"

bool Nanobot::reaches(const Coordinates &c, long long buffer) const
{
    return std::abs(c.x - loc.x) + std::abs(c.y - loc.y) + std::abs(c.z - loc.z) <= r + buffer;
}

std::istream &operator>>(std::istream &is, Nanobot &n)
{
    auto c = ' ', comma = ',';
    return is >> n.loc >> comma >> c >> c >> n.r;
}
