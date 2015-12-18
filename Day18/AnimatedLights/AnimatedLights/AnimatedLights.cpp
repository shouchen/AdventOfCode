// AnimatedLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <algorithm>

template<int SIZE>
class Grid
{
public:
    Grid()
    {
        for (int row = 0; row < SIZE; row++)
            for (int col = 0; col < SIZE; col++)
                data[row][col] = false;

        sticky = false;
    }

    Grid &operator=(const Grid &rhs)
    {
        for (int row = 0; row < SIZE; row++)
            for (int col = 0; col < SIZE; col++)
                data[row][col] = rhs.data[row][col];

        sticky = rhs.sticky;
    }

    void ReadInitialConfiguration(const char *filename)
    {
        std::ifstream f(filename);
        for (int row = 0; row < SIZE; row++)
            for (int col = 0; col < SIZE; col++)
            {
                char c;
                f >> c;
                data[row][col] = c == '#';
            }

        UpdateStickyLights();
    }

    void SetSticky()
    {
        sticky = true;
    }

    void Animate()
    {
        Grid prev = *this;

        for (int row = 0; row < SIZE; row++)
            for (int col = 0; col < SIZE; col++)
            {
                unsigned neighborsOn = prev.CountOnNeighbors(row, col);
                data[row][col] = data[row][col] ? (neighborsOn == 2 || neighborsOn == 3) : (neighborsOn == 3);
            }

        UpdateStickyLights();
    }

    unsigned Count()
    {
        unsigned count = 0;

        for (int row = 0; row < SIZE; row++)
            for (int col = 0; col < SIZE; col++)
                if (data[row][col])
                    count++;

        return count;
    }

private:
    bool data[SIZE][SIZE];
    bool sticky;

    unsigned CountOnNeighbors(int row, int col)
    {
        unsigned count = 0;

        for (int i = std::max(row - 1, 0); i <= std::min(row + 1, SIZE - 1); i++)
            for (int j = std::max(col - 1, 0); j <= std::min(col + 1, SIZE - 1); j++)
                if ((i != row || j != col) && data[i][j])
                    count++;

        return count;
    }

    void UpdateStickyLights()
    {
        if (sticky)
            data[0][0] = data[0][SIZE - 1] = data[SIZE - 1][0] = data[SIZE - 1][SIZE - 1] = true;
    }
};

void _tmain(int argc, _TCHAR *argv[])
{
    Grid<100> grid1;
    grid1.ReadInitialConfiguration("Input.txt");

    Grid<100> grid2 = grid1;
    grid2.SetSticky();

    for (int i = 1; i <= 100; i++)
    {
        grid1.Animate();
        grid2.Animate();
    }

    std::cout << "part one: " << grid1.Count() << std::endl;
    std::cout << "part two: " << grid2.Count() << std::endl;
}
