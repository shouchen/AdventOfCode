// Eggnog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Solver.h"
#include <fstream>
#include <iostream>

void ReadInput(const char *filename, std::vector<unsigned> &containers)
{
    std::ifstream f(filename);
    unsigned n;

    while (f >> n)
        containers.push_back(n);

    f.close();
}

void _tmain(int argc, _TCHAR *argv[])
{
    std::vector<unsigned> containers;
    ReadInput("Input.txt", containers);

    unsigned numCombinations = 0, numOfMinimumLength = 0;
    Solver::Solve(containers, 150, numCombinations, numOfMinimumLength);

    std::cout << "part one: " << numCombinations << std::endl;
    std::cout << "part two: " << numOfMinimumLength << std::endl;
}
