#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>

using Coordinates = std::pair<int, int>;
using CoordinateList = std::vector<Coordinates>;
using CoordinateSet = std::set<Coordinates>;
using AntennaMap = std::map<char, CoordinateList>;

auto add_antinodes(CoordinateList &same_freq_antennae, int max_row, int max_col, bool part2, CoordinateSet &antinodes)
{
    for (auto i = 0; i < same_freq_antennae.size(); i++)
        for (auto j = i + 1; j < same_freq_antennae.size(); j++)
        {
            auto a = same_freq_antennae[i], b = same_freq_antennae[j];
            auto row_dir = b.first - a.first, col_dir = b.second - a.second;

            auto row = a.first - (part2 ? 0 : row_dir), col = a.second - (part2 ? 0 : col_dir);
            while (row >= 0 && row < max_row && col >= 0 && col < max_col)
            {
                antinodes.insert({ row, col });
                row -= row_dir, col -= col_dir;

                if (!part2) break;
            }

            row = b.first + (part2 ? 0 : row_dir), col = b.second + (part2 ? 0 : col_dir);
            while (row >= 0 && row < max_row && col >= 0 && col < max_col)
            {
                antinodes.insert({ row, col });
                row += row_dir, col += col_dir;

                if (!part2) break;
            }
        }
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    AntennaMap antennae;
    auto max_row = 0, max_col= 0;

    while (file >> line)
    {
        for (auto i = 0; i < line.size(); i++)
            if (line[i] != '.')
                antennae[line[i]].push_back({ max_row, i });

        max_row++;
        max_col = int(line.length());
    }

    CoordinateSet antinodes;

    for (auto test : antennae)
        add_antinodes(test.second, max_row, max_col, false, antinodes);

    auto part1 = antinodes.size();

    for (auto test : antennae)
        add_antinodes(test.second, max_row, max_col, true, antinodes);

    return std::make_pair(part1, antinodes.size());
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 244);
    assert(answer.second == 912);
    return 0;
}
