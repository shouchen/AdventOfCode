// Eggnog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

vector<unsigned> containers;

void ReadInput(const char *filename)
{
    ifstream f(filename);
    unsigned n;

    while (f >> n)
    {
        containers.push_back(n);
        cout << n << endl;
    }

    f.close();
}

map<unsigned, unsigned> lengthToCount;

// returns count of (sub)solutions
unsigned Solve(vector<unsigned>::iterator begin, vector<unsigned>::iterator end, unsigned amount, unsigned chosenSoFar)
{
    if (begin == end) return 0;

    if (amount > *begin)
    {
        unsigned retval = 0;

        // first try taking container and all the ones under it
        auto temp = *begin;

        begin++;
        retval += Solve(begin, end, amount - temp, chosenSoFar + 1);  // TAKE THIS ONE, SUM SUB ONES
        retval += Solve(begin, end, amount, chosenSoFar);         // DON'T TAKE THIS ONE, SUM SUB ONES

        return retval;
    }
    else if (amount == *begin)
    {
        lengthToCount[chosenSoFar + 1]++;
        return 1 + Solve(++begin, end, amount, chosenSoFar);  // ONE FOR THIS, PLUS ANY OTHERS
    }
    else // if (amount < *begin) SKIP THIS ONE, SUM SUB ONES
    {
        unsigned retval = 0;

        begin++;
        retval += Solve(begin, end, amount, chosenSoFar);

        return retval;
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInput("Input.txt");
    unsigned count = Solve(containers.begin(), containers.end(), 150, 0);
    unsigned part2 = 0;
    for (int i = 1; i < 1000; i++)
    {
        if (lengthToCount.find(i) != lengthToCount.end())
        {
            part2 = lengthToCount[i];
        }
    }
}
