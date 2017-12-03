#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <map>

unsigned do_part1(unsigned input)
{
    int top = 0, bottom = 0, left = 0, right = 0;
    int x = 0, y = 0;
    unsigned n = 0;

    for (;;)
    {
        // right
        for (; x <= right; x++)
            if (++n == input)
                return abs(x) + abs(y);

        right++;

        // up
        for (; y >= top; y--)
            if (++n == input)
                return abs(x) + abs(y);

        top--;

        // left
        for (; x >= left; x--)
            if (++n == input)
                return abs(x) + abs(y);

        left--;

        // down
        for (; y <= bottom; y++)
            if (++n == input)
                return abs(x) + abs(y);

        bottom++;
    }
}

std::map<std::pair<int, int>, unsigned> board;

unsigned sum_neighbors(int x, int y)
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
        for (; y >= top; y--)
        {
            auto sum = sum_neighbors(x, y);
            if (sum > input) return sum;
            board[{x, y}] = sum;
        }

        top--;

        // left
        for (; x >= left; x--)
        {
            auto sum = sum_neighbors(x, y);
            if (sum > input) return sum;
            board[{x, y}] = sum;
        }

        left--;

        // down
        for (; y <= bottom; y++)
        {
            auto sum = sum_neighbors(x, y);
            if (sum > input) return sum;
            board[{x, y}] = sum;
        }

        bottom++;

        // right
        for (; x <= right; x++)
        {
            auto sum = sum_neighbors(x, y);
            if (sum > input) return sum;
            board[{x, y}] = sum;
        }

        right++;
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
