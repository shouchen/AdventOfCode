#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <numeric>
#include <cassert>

const unsigned grid_size = 300;

int grid[grid_size + 1][grid_size + 1];

int compute_power(unsigned x, unsigned y, int gsn)
{
    auto rack_id = x + 10;
    auto power = rack_id * y;
    power += gsn;
    power *= rack_id;
    power = power / 100 % 10;
    power -= 5;
    return power;
}

void populate_grid(int gsn)
{
    for (auto y = 1; y <= grid_size; y++)
        for (auto x = 1; x <= grid_size; x++)
            grid[y][x] = compute_power(x, y, gsn);
}

int get_power_of_square(unsigned x, unsigned y, unsigned n)
{
    auto power = 0;
    for (auto i = 0U; i < n; i++)
        for (auto j = 0U; j < n; j++)
            power += grid[y + i][x + j];

    return power;
}

std::string find_square_with_highest_power(unsigned min_n, unsigned max_n)
{
    auto highest_total_power = std::numeric_limits<int>::min();
    auto best_x = 0U, best_y = 0U, best_n = 0U;

    for (auto n = min_n; n <= max_n; n++)
        for (auto y = 1U; y < grid_size + 1 - n; y++)
            for (auto x = 1U; x < grid_size + 1 - n; x++)
            {
                auto power = get_power_of_square(x, y, n);
                if (power > highest_total_power)
                {
                    highest_total_power = power;
                    best_x = x;
                    best_y = y;
                    best_n = n;
                }
            }

    std::ostringstream o;
    o << best_x << "," << best_y;
    if (min_n != max_n)
        o << "," << best_n;
    return o.str();
}

int main()
{
    const int input = 9306;
    populate_grid(input);

    auto part1 = find_square_with_highest_power(3, 3);
    auto part2 = find_square_with_highest_power(1, 100);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "235,38");
    assert(part2 == "233,146,13");

    return 0;
}
