#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <map>

unsigned do_part1(unsigned input)
{
    if (input == 1) return 0; // why one-off?

    // 1. Find largest complete odd square (1^2, 3^2, 5^2, etc.).
    unsigned square = sqrt(input);
    if (square % 2 == 0) square--; // if even, round down to odd

    // 2. Starting with overflow, there are 4 congruent runs (ULDR). Get zero-based overflow and run length.
    unsigned overflow = input - square * square - 1;
    unsigned run_length = square + 1;

    // 3. Get offset into one of the runs (mod since don't care which)
    unsigned offset = overflow % run_length;

    // 4. First half is run-1-offset, second half is offset+1.
    if (offset < run_length / 2)
        return run_length - 1 - offset;
    else
        return offset + 1;
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

    for (int i = 1; i < 50; i++)
        std::cout << do_part1(i) << std::endl;

    auto part1 = do_part1(347991);
    auto part2 = do_part2(347991);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 480);
    assert(part2 == 349975);
    return 0;
}
