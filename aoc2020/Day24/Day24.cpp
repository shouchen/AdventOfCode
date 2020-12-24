#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <cassert>

using Location = std::pair<int, int>;
std::set<Location> black_tiles;

auto follow_instructions(const std::string &instructions)
{
    std::stringstream ss(instructions);
    auto c = ' ';
    auto row = 0, col = 0;

    while (ss >> c)
    {
        switch (c)
        {
            case 'e':
                col += 2;
                continue;
            case 's':
                row++;
                break;
            case 'w':
                col -= 2;
                continue;
            case 'n':
                --row;
                break;
        }

        ss >> c;
        col += (c == 'e') ? 1 : -1;
    }

    return std::make_pair(row, col);
}

auto count_adjacent_black_tiles(std::set<Location> &floor, Location &loc)
{
    auto retval = 0;

    for (auto row_dir = -1; row_dir <= 1; row_dir++)
        for (auto col_dir = -2; col_dir <= 2; col_dir++)
        {
            if ((!row_dir && !col_dir) || ((row_dir + col_dir) & 1)) // not valid adjacency
                continue;

            if (floor.find(std::make_pair(loc.first + row_dir, loc.second + col_dir)) != floor.end())
                retval++;
        }

    return retval;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string s;

    while (file >> s)
    {
        auto loc = follow_instructions(s);
        auto tile = black_tiles.find(loc);

        if (tile == black_tiles.end())
            black_tiles.insert(loc);
        else
            black_tiles.erase(tile);
    }

    return black_tiles.size();
}

auto do_part2()
{
    for (int day = 1; day <= 100; day++)
    {
        std::set<std::pair<int, int>> snapshot;
        auto min_row = INT_MAX, min_col = INT_MAX, max_row = INT_MIN, max_col = INT_MIN;
        for (auto &a : black_tiles)
        {
            snapshot.insert(a);
            min_row = std::min(min_row, a.first), max_row = std::max(max_row, a.first);
            min_col = std::min(min_col, a.second), max_col = std::max(max_col, a.second);
        }

        for (int r = min_row - 1; r <= max_row + 1; r++)
            for (int c = min_col - 2; c <= max_col + 2; c++)
            {
                if ((r + c) & 1) // not a valid hex loc
                    continue;

                auto loc = std::make_pair(r, c);
                auto n = count_adjacent_black_tiles(snapshot, loc);

                if (snapshot.find(loc) != snapshot.end())
                {
                    if (n == 0 || n > 2)
                        black_tiles.erase(loc);
                }
                else
                {
                    if (n == 2)
                        black_tiles.insert(loc);
                }
            }
    }

    return black_tiles.size();
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 528);
    assert(part2 == 4200);
    return 0;
}
