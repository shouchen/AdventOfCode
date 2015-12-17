// Eggnog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>

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

    //containers.push_back(20);
    //containers.push_back(15);
    //containers.push_back(10);
    //containers.push_back(5);
    //containers.push_back(5);
}

// returns count of (sub)solutions
unsigned Solve(vector<unsigned>::iterator begin, vector<unsigned>::iterator end, unsigned amount)
{
    if (begin == end) return 0;

    if (amount > *begin)
    {
        unsigned retval = 0;

        // first try taking container and all the ones under it
        auto temp = *begin;

        begin++;
        retval += Solve(begin, end, amount - temp);  // TAKE THIS ONE, SUM SUB ONES
        retval += Solve(begin, end, amount);         // DON'T TAKE THIS ONE, SUM SUB ONES

        return retval;
    }
    else if (amount == *begin)
    {
        cout << "FOUND!" << endl;
        return 1 + Solve(++begin, end, amount);  // ONE FOR THIS, PLUS ANY OTHERS
    }
    else // if (amount < *begin) SKIP THIS ONE, SUM SUB ONES
    {
        unsigned retval = 0;

        begin++;
        retval += Solve(begin, end, amount);

        return retval;
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInput("Input.txt");
    unsigned count = Solve(containers.begin(), containers.end(), 150);
}
