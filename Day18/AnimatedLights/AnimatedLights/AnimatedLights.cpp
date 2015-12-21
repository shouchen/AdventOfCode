// AnimatedLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "Grid.h"


void _tmain(int argc, _TCHAR *argv[])
{
    Grid<100> grid1;
    grid1.ReadInitialConfiguration("Input.txt");

    Grid<100> grid2 = grid1;
    grid2.SetSticky();

    for (int i = 0; i < 100; i++)
    {
        grid1.Animate();
        grid2.Animate();
    }

    std::cout << "part one: " << grid1.Count() << std::endl;
    std::cout << "part two: " << grid2.Count() << std::endl;
}
