// WeatherMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cassert>

unsigned GetOrdinal(unsigned row, unsigned col)
{
    return ((row + col - 1) * (row + col)) / 2 - row + 1;
}

void _tmain(int argc, _TCHAR *argv[])
{
    auto value = 20151125ULL;
    for (auto ordinal = GetOrdinal(2981, 3075); ordinal > 1; --ordinal)
        value = (value * 252533ULL) % 33554393ULL;

    std::cout << "answer: " << value << std::endl;
    assert(value == 9132360UL);
}
