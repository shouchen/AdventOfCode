#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <map>

// Algorithm is based on mathematical pattern:
//     +-------+-+
//     |4 3 2 3|4|
//     | +---+-+ |
//     |3|2 1|2|3|
//     | +-+-+ + +
//     |2|1|0|1|2|
//     | | +-+-+ +
//     |3|2|1 2|3|
//     | +-+---+-+
//     |4|3 2 3 4|
//     +-+-------+
unsigned do_part1(unsigned input)
{
    // 1. Find largest odd square (1^2, 3^2, 5^2, etc.) less than or equal to input.
    auto square = (unsigned)sqrt(input);
    if (square % 2 == 0) square--; // if even, round down to odd

    // 2. Determine how far beyond that the input is.
    auto overflow = input - square * square;

    // 3. If no overflow, distance is square - 1.
    if (!overflow) return square - 1;

    // 4. Starting with overflow, there are 4 congruent runs (ULDR). Get run length.
    auto run_length = square + 1;

    // 5. Get zero-based offset into one of the runs (we don't care which of the 4 it is).
    auto offset = (overflow - 1) % run_length;

    // 6. First half of run is decreasing, and second half increasing. Compute distance.
    return (offset < run_length / 2) ? (run_length - 1 - offset) : (offset + 1);
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
    // Initialize origin to 1.
    int x_dir = 1, y_dir = 0;
    int x = 0, y = 0;
    board[{x, y}] = 1;

    // Proceed with 4 runs of 2, 4 runs of 4, 4 runs of 6, etc.
    for (auto run_length = 2; ; run_length += 2)
    {
        x++, y++;

        // Up, Down, Left, Right
        for (int i = 0; i < 4; i++)
        {
            // Turn counterclockwise.
            int temp = x_dir;
            x_dir = y_dir;
            y_dir = -temp;

            // Process a single run.
            for (int j = 0; j < run_length; j++)
            {
                x += x_dir, y += y_dir;

                auto sum = sum_neighbors(x, y);
                if (sum > input) return sum;
                board[{x, y}] = sum;
            }
        }
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
