// WeatherMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cassert>

using namespace std;

unsigned GetOrdinal(unsigned row, unsigned col)
{
    return ((row + col - 1) * (row + col)) / 2 - row + 1;
}

void _tmain(int argc, _TCHAR *argv[])
{
    unsigned ordinal = GetOrdinal(2981, 3075);

    auto value = 20151125ULL;
    while (--ordinal)
        value = (value * 252533ULL) % 33554393ULL;

    cout << "part one: " << value << endl;
    assert(value == 9132360UL);
}
