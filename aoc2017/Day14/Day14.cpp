#include "stdafx.h"
#include "KnotHash.h"
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

static const auto GRID_SIZE = 128U;
bool grid[GRID_SIZE][GRID_SIZE];

auto hex_to_unsigned(char hex_digit)
{
    return unsigned(hex_digit - ((hex_digit > '9') ? ('a' - 10) : '0'));
}

unsigned clear_contiguous_region(int row, int col)
{
    auto count = 0U;

    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE && grid[row][col])
    {
        grid[row][col] = false;
        count++;

        count += clear_contiguous_region(row - 1, col);
        count += clear_contiguous_region(row + 1, col);
        count += clear_contiguous_region(row, col - 1);
        count += clear_contiguous_region(row, col + 1);
    }

    return count;
}

void process_input(const std::string &input, unsigned &part1, unsigned &part2)
{
    part1 = part2 = 0U;

    for (auto row = 0U; row < GRID_SIZE; row++)
    {
        std::stringstream ss;
        ss << input << '-' << row;
        auto hash = do_knot_hash(ss.str());

        auto col = 0U;
        for (auto i = 0U; i < GRID_SIZE / 4; i++)
        {
            auto val = hex_to_unsigned(hash[i]);
            for (unsigned mask = 0x8; mask; mask >>= 1)
                grid[row][col++] = (val & mask) != 0U;
        }
    }

    for (auto row = 0U; row < GRID_SIZE; row++)
    {
        for (auto col = 0U; col < GRID_SIZE; col++)
        {
            unsigned region_size = clear_contiguous_region(row, col);
            part1 += region_size;
            part2 += region_size ? 1 : 0;
        }
    }
}

int main()
{
    auto part1 = 0U, part2 = 0U;

    process_input("flqrgnkx", part1, part2);
    assert(part1 == 8108);
    assert(part2 == 1242);

    process_input("vbqugkhl", part1, part2);
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 8148);
    assert(part2 == 1180);
    return 0;
}
