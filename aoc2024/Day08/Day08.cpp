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

auto add_antinodes(CoordinateList &same_freq_antennae, int height, int width, bool part2, CoordinateSet &antinodes)
{
    for (auto i = 0; i < same_freq_antennae.size(); i++)
        for (auto j = i + 1; j < same_freq_antennae.size(); j++)
        {
            auto a = same_freq_antennae[i], b = same_freq_antennae[j];
            auto row_dir = b.first - a.first, col_dir = b.second - a.second;

            auto new_antinode_row = part2 ? a.first : (a.first - row_dir);
            auto new_antinode_col = part2 ? a.second : (a.second - col_dir);

            while (new_antinode_row >= 0 && new_antinode_row < height && new_antinode_col >= 0 && new_antinode_col < width)
            {
                antinodes.insert({ new_antinode_row, new_antinode_col });
                new_antinode_row -= row_dir;
                new_antinode_col -= col_dir;

                if (!part2) break;
            }

            new_antinode_row = part2 ? b.first : (b.first + row_dir);
            new_antinode_col = part2 ? b.second : (b.second + col_dir);

            while (new_antinode_row >= 0 && new_antinode_row < height && new_antinode_col >= 0 && new_antinode_col < width)
            {
                antinodes.insert({ new_antinode_row, new_antinode_col });
                new_antinode_row += row_dir;
                new_antinode_col += col_dir;

                if (!part2) break;
            }
        }
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    AntennaMap antennae;
    auto height = 0, width = 0;

    while (file >> line)
    {
        for (auto i = 0; i < line.size(); i++)
            if (line[i] != '.')
                antennae[line[i]].push_back({ height, i });

        height++;
        width = int(line.length());
    }

    CoordinateSet antinodes;

    for (auto test : antennae)
        add_antinodes(test.second, height, width, false, antinodes);

    auto retval = std::make_pair(antinodes.size(), 0ULL);

    for (auto test : antennae)
        add_antinodes(test.second, height, width, true, antinodes);

    retval.second = antinodes.size();
    return retval;
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
