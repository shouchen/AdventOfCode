// AnimatedLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <algorithm>

#define SIZE 100
bool grid[SIZE][SIZE];

void ReadInitialConfiguration(const char *filename)
{
    std::ifstream f(filename);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
        {
            char c;
            f >> c;
            grid[i][j] = c == '#';
        }

    f.close();
}

void StuckOn(bool grid[SIZE][SIZE])
{
    grid[0][0] = grid[0][SIZE - 1] = grid[SIZE - 1][0] = grid[SIZE - 1][SIZE - 1] = true;
}

unsigned CountOnNeighbors(bool grid[SIZE][SIZE], int row, int col)
{
    unsigned count = 0;
    for (int i = std::max(row - 1, 0); i <= std::min(row + 1, SIZE - 1); i++)
        for (int j = std::max(col - 1, 0); j <= std::min(col + 1, SIZE - 1); j++)
        {
            if ((i != row || j != col) && grid[i][j])
                count++;
        }

    return count;
}

void Animate()
{
    bool prev[SIZE][SIZE];
    memcpy(prev, grid, sizeof(prev));

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
        {
            unsigned neighborsOn = CountOnNeighbors(prev, i, j);
            if (prev[i][j])
                grid[i][j] = (neighborsOn == 2 || neighborsOn == 3);
            else
                grid[i][j] = (neighborsOn == 3);
        }
}

unsigned Count()
{
    unsigned count = 0;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
        {
            if (grid[i][j])
                count++;
        }

    return count;
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInitialConfiguration("Input.txt");
    StuckOn(grid);

    for (int i = 1; i <= 100; i++)
    {
        std::cout << std::endl << "After " << i << std::endl;
        Animate();
        StuckOn(grid);
    }

    std::cout << std::endl << Count() << std::endl;
}
