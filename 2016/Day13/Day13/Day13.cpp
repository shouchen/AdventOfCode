#include "stdafx.h"
#include <iostream>
#include <climits>

// Array:
// -1 = wall
// MAX_INT = never visited

#define MAX_Y 250
#define MAX_X 250
int board[MAX_Y][MAX_X];

#define TARGET_X 31
#define TARGET_Y 39

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

void Dump()
{
    for (int i = 0; i < MAX_Y; i++)
    {
        for (int j = 0; j < MAX_X; j++)
        {
            if (board[i][j] == -1)
                std::cout << "# ";
            else if (board[i][j] == INT_MAX)
                std::cout << ". ";
            else
                std::cout << char(board[i][j] + '@') << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Foo(int x, int y, unsigned gen)
{
    if (x >= 0 && x < MAX_X && y >=0 && y <= MAX_Y && board[y][x] == INT_MAX)
    {
        board[y][x] = gen;
        return x == TARGET_X && y == TARGET_Y;
    }
    return false;
}

bool DoGeneration(unsigned gen)
{
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            if (board[y][x] == gen - 1)
            {
                if (Foo(x, y - 1, gen)) return true; // UP
                if (Foo(x, y + 1, gen)) return true; // DOWN
                if (Foo(x - 1, y, gen)) return true; // LEFT
                if (Foo(x + 1, y, gen)) return true; // RIGHT
            }
        }
    }

    return false;
}

unsigned NumReachable()
{
    auto count = 0U;
    for (int y = 0; y < MAX_Y; y++)
        for (int x = 0; x < MAX_X; x++)
            if (board[y][x] < INT_MAX && board[y][x] != -1)
                count++;

    return count;
}

int main()
{
    for (int y = 0; y < MAX_Y; y++)
        for (int x = 0; x < MAX_X; x++)
            board[y][x] = IsWall(y, x) ? -1 : INT_MAX;

    board[1][1] = 0;

    // Part 1
    //unsigned gen = 0;
    //while (!DoGeneration(++gen));

    // Part 2
    for (int i = 1; i <= 50; i++)
        DoGeneration(i);

    auto part2 = NumReachable();

    // Assert part 1 = 86
    // Assert Part 2 = 127

    return 0;
}
