#pragma once
#include <iostream>
#include "Coordinates.h"

struct Nanobot
{
    Coordinates loc;
    long long r;

    Nanobot() {}
    bool reaches(const Coordinates &c, long long buffer = 0) const;
};

std::istream &operator>>(std::istream &is, Nanobot &n);