#pragma once

#include "stdafx.h"


template<int SIZE>
class Grid
{
public:
    Grid();
    Grid &operator=(const Grid &rhs);

    void ReadInitialConfiguration(const char *filename);
    void SetSticky();
    void Animate();
    unsigned Count();

private:
    bool data[SIZE][SIZE];
    bool sticky;

    unsigned CountOnNeighbors(int row, int col);
    void UpdateStickyLights();
};
