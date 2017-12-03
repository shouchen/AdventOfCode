#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <map>

unsigned do_part1(unsigned m)
{
    if (m == 1) return 0;

    unsigned part1 = 0;

    int top = 0, bottom = 0, left = 0, right = 1;
    int x = 1, y = 0;

    unsigned n = 1;

    for (;;)
    {
        // up
        while (y >= top)
        {
            auto curr = abs(x) + abs(y--);
            if (++n == m) return curr;
        }
        top--;

        // left
        while (x >= left)
        {
            auto curr = abs(x--) + abs(y);
            if (++n == m) return curr;
        }
        left--;

        // down
        while (y <= bottom)
        {
            auto curr = abs(x) + abs(y++);
            if (++n == m) return curr;
        }
        bottom++;

        // right
        while (x <= right)
        {
            auto curr = abs(x++) + abs(y);
            if (++n == m) return curr;
        }
        right++;
    }
}

std::map<std::pair<int, int>, unsigned> board;
unsigned part2 = 0;

unsigned sum_neighbors(int x, int y, unsigned input)
{
    unsigned sum = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i || j)
            {
                auto f = board.find({ x + i, y + j });
                if (f != board.end())
                    sum += f->second;
            }
        }
    }

    if (!part2 && sum > input)
        part2 = sum;

    return sum;
}

unsigned do_part2(unsigned input)
{
    int top = 0, bottom = 0, left = 0, right = 1;
    int x = 1, y = 0;
    board[{0, 0}] = 1;

    for (;;)
    {
        // up
        do { board[{x, y}] = sum_neighbors(x, y, input); } while (--y >= top);
        top--;

        if (part2) return part2;

        // left
        do { board[{x, y}] = sum_neighbors(x, y, input); } while (--x >= left);
        left--;

        if (part2) return part2;

        // down
        do { board[{x, y}] = sum_neighbors(x, y, input); } while (++y <= bottom);
        bottom++;

        if (part2) return part2;

        // right
        do { board[{x, y}] = sum_neighbors(x, y, input); } while (++x <= right);
        right++;

        if (part2) return part2;
    }
}

int main()
{
    assert(do_part1(1) == 0);
    assert(do_part1(12) == 3);
    assert(do_part1(23) == 2);
    assert(do_part1(1024) == 31);

    auto part1 = do_part1(347991);
    auto part2 = do_part2(347991);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 480);
    assert(part2 == 349975);
    return 0;
}
