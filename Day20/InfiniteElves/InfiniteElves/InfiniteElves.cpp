// InfiniteElves.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <map>

using namespace std;

unsigned NumPresents(unsigned house)
{
    auto presents = 0U;
    for (unsigned elf = 1; elf <= house; elf++)
        if (house % elf == 0)
            presents += 10 * elf;

    return presents;
}

map<unsigned, unsigned> elfToDeliveries;

unsigned NumPresents2(unsigned house)
{
    auto presents = 0U, deliveriesForThisElf = 0U;
    for (unsigned elf = 1; elf <= house; elf++)
    {
        if (house % elf == 0)
        {
            presents += 11 * elf;
            if (++deliveriesForThisElf == 50)
                break;
        }
    }

    return presents;
}

void _tmain(int argc, _TCHAR *argv[])
{
    auto input = 33100000U;

    //for (auto house = 0U; house < UINT_MAX; house++)
    //{
    //    if (NumPresents(house) >= input)
    //    {
    //        cout << "part one: House " << house << endl;
    //        break;
    //    }
    //}

    for (auto house = 0U; house < UINT_MAX; house++)
    {
        if (NumPresents2(house) >= input)
        {
            cout << "part two: House " << house << endl;
            break;
        }
    } // 928440 was too high
}
