#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <string>
#include <queue>
#include <map>
#include <numeric>
#include <algorithm>
#include <cassert>

std::vector<std::pair<int, int>> locations;
std::vector<std::pair<int, int>> velocities;

int grid[301][301];

int get_power(unsigned x, unsigned y, int gsn)
{
    int rack_id = x + 10;
    int power = rack_id * y;
    power += gsn;
    power *= rack_id;
    power = power / 100 % 10;
    power -= 5;
    return power;
}

void populate_grid(int gsn)
{
    for (int y = 1; y <= 300; y++)
        for (int x = 1; x <= 300; x++)
        {
            grid[y][x] = get_power(x, y, gsn);
        }
}

int get_power_of_3x3(unsigned x, unsigned y, int gsn)
{
    int power = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            power += grid[y + i][x + j];
        }
    return power;
}

int get_power_of_nxn(unsigned x, unsigned y, unsigned size, int gsn)
{
    int power = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            power += grid[y + i][x + j];
        }
    return power;
}

std::pair<unsigned, unsigned> part1;
void do_part1(int gsn)
{
    int highest_total_power = std::numeric_limits<int>::min();
    unsigned best_x = 0, best_y = 0;

    for (int y = 1; y < 298; y++)
        for (int x = 1; x < 298; x++)
        {
            auto power = get_power_of_3x3(x, y, gsn);
            if (power > highest_total_power)
            {
                highest_total_power = power;
                best_x = x;
                best_y = y;
            }
        }

    std::cout << "Answer: " << best_x << "," << best_y << std::endl;
}

void do_part2(int gsn)
{
    int highest_total_power = std::numeric_limits<int>::min();
    unsigned best_x = 0, best_y = 0, best_size = 0;

    for (int i = 1; i <= 300; i++)
    {
        for (int y = 1; y < 301 - i; y++)
            for (int x = 1; x < 301 - i; x++)
            {
                auto power = get_power_of_nxn(x, y, i, gsn);
                if (power > highest_total_power)
                {
                    highest_total_power = power;
                    best_x = x;
                    best_y = y;
                    best_size = i;
                    std::cout << x << "," << y << "," << i << std::endl;
                }
            }
    }

    std::cout << "Answer: " << best_x << "," << best_y << "," << best_size << std::endl;
}

int main()
{
    populate_grid(9306);
    do_part2(9306);

    return 0;  //235,38 (part1) //233,146,13 (part2)
}
