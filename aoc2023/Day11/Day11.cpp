#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

std::vector<std::pair<int, int>> galaxies;
std::vector<bool> row_has_galaxies, col_has_galaxies;

auto get_dists(std::pair<int, int> &g1, std::pair<int, int> &g2)
{
    auto dist1 = 0LL, dist2 = 0LL;

    if (g1.first != g2.first)
    {
        auto delta = (g1.first < g2.first) ? 1 : -1;

        for (auto row = g1.first; row != g2.first; row += delta)
            if (row_has_galaxies[row])
                dist1++, dist2++;
            else
                dist1 += 2, dist2 += 1000000;
    }

    if (g1.second != g2.second)
    {
        auto delta = (g1.second < g2.second) ? 1 : -1;

        for (auto col = g1.second; col != g2.second; col += delta)
            if (col_has_galaxies[col])
                dist1++, dist2++;
            else
                dist1 += 2, dist2 += 1000000;
    }

    return std::make_pair(dist1, dist2);
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    for (auto row = 0; std::getline(file, line); row++)
    {
        row_has_galaxies.push_back(false);
        while (col_has_galaxies.size() < line.length())
            col_has_galaxies.push_back(false);

        for (auto col = 0; col < line.length(); col++)
            if (line[col] == '#')
            {
                galaxies.push_back(std::make_pair(row, col));
                row_has_galaxies[row] = true;
                col_has_galaxies[col] = true;
            }
    }

    auto part1 = 0LL, part2 = 0LL;

    for (auto g1 = galaxies.begin(); g1 != galaxies.end(); g1++)
        for (auto g2 = g1 + 1; g2 != galaxies.end(); g2++)
        {
            auto dists = get_dists(*g1, *g2);
            part1 += dists.first;
            part2 += dists.second;
        }

    return std::make_pair(part1, part2);
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 9536038);
    assert(answer.second == 447744640566);
    return 0;
}
