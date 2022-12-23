#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <cassert>

using RowCol = std::pair<int, int>;
using Proposals = std::map<RowCol, std::vector<RowCol>>;

enum { NW = 0x01, N = 0x02, NE = 0x04, W = 0x08, E = 0x10, SW = 0x20, S= 0x40, SE = 0x80 };

struct
{
    int check, row_dir, col_dir;
}
const rules[] =
{
    { NE | E | SE,  0,  1},
    { NW | N | NE, -1,  0},
    { SW | S | SE,  1,  0},
    { NW | W | SW,  0, -1}
};

std::set<RowCol> elves;

auto get_neighbor_bitmap(const RowCol &elf)
{
    auto neighbors = 0, bit = 0x1;

    for (auto i = -1; i <= 1; i++)
        for (auto j = -1; j <= 1; j++)
            if (i || j)
            {
                if (elves.find(std::make_pair(elf.first + i, elf.second + j)) != elves.end())
                    neighbors |= bit;

                bit <<= 1;
            }

    return neighbors;
}

auto get_dot_count()
{
    auto min_row = INT_MAX, max_row = INT_MIN, min_col = INT_MAX, max_col = INT_MIN;

    for (auto &i : elves)
    {
        min_row = std::min(i.first, min_row);
        max_row = std::max(i.first, max_row);
        min_col = std::min(i.second, min_col);
        max_col = std::max(i.second, max_col);
    }

    return (max_row - min_row + 1) * (max_col - min_col + 1) - elves.size();
}

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto row = 0;
    auto retval = std::make_pair(0, 0);

    while (getline(file, line))
    {
        for (auto col = 0; col < line.length(); col++)
            if (line[col] == '#')
                elves.insert(std::make_pair(row, col));
        row++;
    }

    for (auto round = 1; ; round++)
    {
        Proposals proposals;

        for (auto &from : elves)
        {
            auto neighbors = get_neighbor_bitmap(from);

            if (!neighbors)
                continue;

            for (auto i = 0; i < sizeof(rules)/sizeof(rules[0]); i++)
            {
                auto &r = rules[(round + i) % 4];
                if ((neighbors & r.check) == 0)
                {
                    auto to = std::make_pair(from.first + r.row_dir, from.second + r.col_dir);
                    proposals[to].push_back(from);
                    break;
                }
            }
        }

        auto elf_moved = false;
        for (auto &p : proposals)
        {
            if (p.second.size() == 1)
            {
                elves.insert(p.first);
                elves.erase(elves.find(p.second[0]));
                elf_moved = true;
            }
        }

        if (!elf_moved)
        {
            retval.second = round;
            return retval;
        }

        if (round == 10)
            retval.first = int(get_dot_count());
    }
}

int main()
{
    auto output = process_input("input.txt");
    std::cout << "Part One: " << output.first << std::endl;
    std::cout << "Part Two: " << output.second << std::endl;

    assert(output.first == 4254);
    assert(output.second == 992);
    return 0;
}
