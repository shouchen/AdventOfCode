// Sleigh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "Solver.h"

void _tmain(int argc, _TCHAR *argv[])
{
    unsigned weight;
    std::vector<unsigned> weights;

    std::ifstream f("Input.txt");
    while (f >> weight)
        weights.push_back(weight);

    auto part1 = Solver::Solve(weights, 3);
    assert(part1 == 11266889531ULL);
    std::cout << "part one: " << part1 << std::endl;

    auto part2 = Solver::Solve(weights, 4);
    assert(part2 == 77387711ULL);
    std::cout << "part two: " << part2 << std::endl;
}
