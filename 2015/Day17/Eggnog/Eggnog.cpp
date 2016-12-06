// Eggnog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Solver.h"
#include <fstream>
#include <iostream>
#include <cassert>

void ReadInput(const char *filename, std::vector<unsigned> &containers)
{
    std::ifstream f(filename);
    unsigned n;

    while (f >> n)
        containers.push_back(n);
}

void _tmain(int argc, _TCHAR *argv[])
{
    std::vector<unsigned> containers;
    ReadInput("Input.txt", containers);

    auto numCombinations = 0U, numOfMinimumLength = 0U;
    Solver::Solve(containers, 150, numCombinations, numOfMinimumLength);

    std::cout << "part one: " << numCombinations << std::endl;
    assert(numCombinations == 4372);

    std::cout << "part two: " << numOfMinimumLength << std::endl;
    assert(numOfMinimumLength == 4);
}
