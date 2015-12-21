// InfiniteElves.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <map>
#include <vector>

using namespace std;

vector<unsigned> Factorize(unsigned number)
{
    vector<unsigned> retval;
    unsigned mid = static_cast<unsigned>(sqrt(number));
    for (unsigned i = 1; i <= mid; i++)
        if ((number % i) == 0)
        {
            retval.push_back(i);
            if (number != 1) retval.push_back(number / i);
        }

    return retval;
}

unsigned NumPresents1(unsigned house)
{
    auto factors = Factorize(house);
    auto sum = 0U;
    for (auto f : factors)
        sum += f;

    return 10 * sum;
}

unsigned NumPresents2(unsigned house)
{
    auto factors = Factorize(house);
    auto sum = 0U;
    for (auto f : factors)
        if (house <= 50 * f)
            sum += f;

    return 11 * sum;
}

void _tmain(int argc, _TCHAR *argv[])
{
    auto input = 33100000U;

    //for (auto house = 0U; house < UINT_MAX; house++)
    //{
    //    if (NumPresents1(house) >= input)
    //    {
    //        cout << "part one: House " << house << endl;
    //        break;
    //    }
    //} // 776160

    for (auto house = 0U; house < UINT_MAX; house++)
    {
        if (NumPresents2(house) >= input)
        {
            cout << "part two: House " << house << endl;
            break;
        }
    } // 786240
}
