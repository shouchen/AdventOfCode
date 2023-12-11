#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cassert>

std::vector<std::string> grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

std::set<int> empty_rows, empty_cols;

long long get_dist(std::pair<int, int> &g1, std::pair<int, int> &g2, int expansion)
{
    auto d1 = 0, d2 = 0;
    if (g1.first < g2.first) d1 = 1; else if (g1.first > g2.first) d1 = -1;
    if (g1.second < g2.second) d2 = 1; else if (g1.second > g2.second) d2 = -1;

    auto dist = 0LL;
    if (d1)
        for (int row = g1.first; row != g2.first; row += d1)
            dist += (empty_rows.find(row) == empty_rows.end()) ? 1 : expansion;

    if (d2)
        for (int col = g1.second; col != g2.second; col += d2)
            dist += (empty_cols.find(col) == empty_cols.end()) ? 1 : expansion;

    return dist;
}

auto solve(int expansion)
{
    for (auto row = 0; row < grid.size(); row++)
    {
        auto only_periods = true;
        for (auto col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col] != '.')
            {
                only_periods = false;
                break;
            }
        }

        if (only_periods)
            empty_rows.insert(row);
    }

    for (auto col = 0; col < grid[0].size(); col++)
    {
        auto only_periods = true;
        for (auto row = 0; row < grid.size(); row++)
        {
            if (grid[row][col] != '.')
            {
                only_periods = false;
                break;
            }
        }

        if (only_periods)
            empty_cols.insert(col);
    }

    std::vector<std::pair<int, int>> galaxies;
    for (auto i = 0; i < grid.size(); i++)
        for (auto j = 0; j < grid[i].size(); j++)
            if (grid[i][j] == '#')
                galaxies.push_back(std::make_pair(i, j));

    auto total = 0LL;
    for (auto i = 0; i < galaxies.size(); i++)
        for (auto j = i + 1; j < galaxies.size(); j++)
            total += get_dist(galaxies[i], galaxies[j], expansion);

    return total;
}

int main()
{
    read_grid("input.txt");

    auto part1 = solve(2);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = solve(1000000);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 9536038);
    assert(part2 == 447744640566);
    return 0;
}
