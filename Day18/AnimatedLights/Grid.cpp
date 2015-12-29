#include "stdafx.h"
#include <fstream>
#include "Grid.h"
#include <algorithm>

template<int SIZE>
Grid<SIZE>::Grid()
{
    for (auto row = 0; row < SIZE; row++)
        for (auto col = 0; col < SIZE; col++)
            data[row][col] = false;

    sticky = false;
}

template<int SIZE>
Grid<SIZE> &Grid<SIZE>::operator=(const Grid<SIZE> &rhs)
{
    for (auto row = 0; row < SIZE; row++)
        for (auto col = 0; col < SIZE; col++)
            data[row][col] = rhs.data[row][col];

    sticky = rhs.sticky;
    return *this;
}

template<int SIZE>
void Grid<SIZE>::ReadInitialConfiguration(const char *filename)
{
    std::ifstream f(filename);

    for (auto row = 0; row < SIZE; row++)
        for (auto col = 0; col < SIZE; col++)
        {
            char c;
            f >> c;
            data[row][col] = c == '#';
        }

    UpdateStickyLights();
}

template<int SIZE>
void Grid<SIZE>::SetSticky()
{
    sticky = true;
}

template<int SIZE>
void Grid<SIZE>::Animate()
{
    Grid prev = *this;

    for (auto row = 0; row < SIZE; row++)
        for (auto col = 0; col < SIZE; col++)
        {
            unsigned neighborsOn = prev.CountOnNeighbors(row, col);
            data[row][col] = data[row][col] ? (neighborsOn == 2 || neighborsOn == 3) : (neighborsOn == 3);
        }

    UpdateStickyLights();
}

template<int SIZE>
unsigned Grid<SIZE>::Count()
{
    auto count = 0U;

    for (auto row = 0; row < SIZE; row++)
        for (auto col = 0; col < SIZE; col++)
            if (data[row][col])
                count++;

    return count;
}

template<int SIZE>
unsigned Grid<SIZE>::CountOnNeighbors(int row, int col)
{
    auto count = 0U;

    for (auto i = std::max(row - 1, 0); i <= std::min(row + 1, SIZE - 1); i++)
        for (auto j = std::max(col - 1, 0); j <= std::min(col + 1, SIZE - 1); j++)
            if ((i != row || j != col) && data[i][j])
                count++;

    return count;
}

template<int SIZE>
void Grid<SIZE>::UpdateStickyLights()
{
    if (sticky)
        data[0][0] = data[0][SIZE - 1] = data[SIZE - 1][0] = data[SIZE - 1][SIZE - 1] = true;
}

template class Grid<100>;
