#include "stdafx.h"
#include <iostream>
#include <climits>
#include <cassert>

#define MAX_Y 250
#define MAX_X 250

int board[MAX_Y][MAX_X];
int numReachable = 1;

#define FAVORITE_NUMBER 1364

inline unsigned CountSetBits(unsigned n)
{
    unsigned count = 0;
    while (n)
    {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

inline bool IsWall(int y, int x)
{
    return CountSetBits(x * x + 3 * x + 2 * x * y + y + y * y + FAVORITE_NUMBER) & 0x1;
}

bool Foo(int x, int y, unsigned gen, unsigned targetX, unsigned targetY)
{
    if (x >= 0 && x < MAX_X && y >=0 && y <= MAX_Y && board[y][x] == INT_MAX)
    {
        board[y][x] = gen;
        ++numReachable;
        return x == targetX && y == targetY;
    }
    return false;
}

bool DoGeneration(unsigned gen, unsigned targetX, unsigned targetY)
{
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            if (board[y][x] == gen - 1)
            {
                if (Foo(x, y - 1, gen, targetX, targetY)) return true; // UP
                if (Foo(x, y + 1, gen, targetX, targetY)) return true; // DOWN
                if (Foo(x - 1, y, gen, targetX, targetY)) return true; // LEFT
                if (Foo(x + 1, y, gen, targetX, targetY)) return true; // RIGHT
            }
        }
    }

    return false;
}

void Solve(unsigned startX, unsigned startY, unsigned targetX, unsigned targetY, unsigned favorite, unsigned part2Steps, unsigned &part1, unsigned &part2)
{
    for (int y = 0; y < MAX_Y; y++)
        for (int x = 0; x < MAX_X; x++)
            board[y][x] = IsWall(y, x) ? -1 : INT_MAX;

    board[startY][startX] = 0;

    part1 = 0, part2 = 0;
    for (unsigned gen = 1; !part1 || !part2; gen++)
    {
        bool temp = DoGeneration(gen, targetX, targetY);
        if (temp && !part1) part1 = gen;
        if (gen == part2Steps) part2 = numReachable;
    }
}

int main()
{
    unsigned part1 = 0, part2 = 0;
    Solve(1, 1, 31, 39, 1364, 50, part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 86);
    assert(part2 == 127);

    return 0;
}
