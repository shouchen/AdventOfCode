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

    auto qe = Solver::Solve(weights, 3);
    assert(qe == 11266889531);
    std::cout << "part one: " << qe << std::endl;

    qe = Solver::Solve(weights, 4);
    assert(qe == 77387711);
    std::cout << "part two: " << qe << std::endl;
}
