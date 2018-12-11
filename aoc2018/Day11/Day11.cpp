#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <numeric>
#include <cassert>

int grid[301][301];

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
    for (auto y = 1; y <= 300; y++)
        for (auto x = 1; x <= 300; x++)
            grid[y][x] = compute_power(x, y, gsn);
}

//int get_power_of_3x3(unsigned x, unsigned y)
//{
//    auto power = 0;
//    for (auto i = 0; i < 3; i++)
//        for (auto j = 0; j < 3; j++)
//            power += grid[y + i][x + j];
//
//    return power;
//}

int get_power_of_nxn(unsigned x, unsigned y, unsigned size)
{
    auto power = 0;
    for (auto i = 0U; i < size; i++)
        for (auto j = 0U; j < size; j++)
            power += grid[y + i][x + j];

    return power;
}

std::string do_part1()
{
    auto highest_total_power = std::numeric_limits<int>::min();
    auto best_x = 0U, best_y = 0U;

    for (auto y = 1; y < 298; y++)
        for (auto x = 1; x < 298; x++)
        {
            auto power = get_power_of_nxn(x, y, 3);
            if (power > highest_total_power)
            {
                highest_total_power = power;
                best_x = x;
                best_y = y;
            }
        }

    std::ostringstream o;
    o << best_x << "," << best_y;
    return o.str();
}

std::string do_part2()
{
    auto highest_total_power = std::numeric_limits<int>::min();
    auto best_x = 0U, best_y = 0U, best_size = 0U;

    for (auto size = 1; size <= 300; size++)
        for (auto y = 1; y < 301 - size; y++)
            for (auto x = 1; x < 301 - size; x++)
            {
                auto power = get_power_of_nxn(x, y, size);
                if (power > highest_total_power)
                {
                    highest_total_power = power;
                    best_x = x;
                    best_y = y;
                    best_size = size;
                }
            }

    std::ostringstream o;
    o << best_x << "," << best_y << "," << best_size;
    return o.str();
}

int main()
{
    const int input = 9306;
    populate_grid(input);

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == "235,38");
    assert(part2 == "233,146,13");

    return 0;
}
