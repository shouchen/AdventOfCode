// AnimatedLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include "Grid.h"

void _tmain(int argc, _TCHAR *argv[])
{
    Grid<100> grid1;
    grid1.ReadInitialConfiguration("Input.txt");

    Grid<100> grid2 = grid1;
    grid2.SetSticky();

    for (auto i = 0; i < 100; i++)
    {
        grid1.Animate();
        grid2.Animate();
    }

    auto part1 = grid1.Count();
    std::cout << "part one: " << part1 << std::endl;
    assert(part1 == 768);

    auto part2 = grid2.Count();
    std::cout << "part two: " << part2 << std::endl;
    assert(part2 == 781);
}
