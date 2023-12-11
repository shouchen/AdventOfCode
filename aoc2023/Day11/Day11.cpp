#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

std::vector<std::pair<int, int>> galaxies;
std::vector<bool> row_has_galaxies, col_has_galaxies;

void read_grid(const std::string &filename)
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
}

auto get_dists(std::pair<int, int> &g1, std::pair<int, int> &g2)
{
    auto d1 = 0, d2 = 0;
    if (g1.first < g2.first) d1 = 1; else if (g1.first > g2.first) d1 = -1;
    if (g1.second < g2.second) d2 = 1; else if (g1.second > g2.second) d2 = -1;

    auto dist1 = 0LL, dist2 = 0LL;
    if (d1)
        for (int row = g1.first; row != g2.first; row += d1)
            if (row_has_galaxies[row])
                dist1++, dist2++;
            else
                dist1 += 2, dist2 += 1000000;

    if (d2)
        for (int col = g1.second; col != g2.second; col += d2)
            if (col_has_galaxies[col])
                dist1++, dist2++;
            else
                dist1 += 2, dist2 += 1000000;

    return std::make_pair(dist1, dist2);
}

auto solve(int expansion)
{
    auto retval = std::make_pair(0LL, 0LL);

    auto total = 0LL;
    for (auto i = 0; i < galaxies.size(); i++)
        for (auto j = i + 1; j < galaxies.size(); j++)
        {
            auto dists = get_dists(galaxies[i], galaxies[j]);
            retval.first += dists.first;
            retval.second += dists.second;
        }

    return retval;
}

int main()
{
    read_grid("input.txt");

    auto answer = solve(2);
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 9536038);
    assert(answer.second == 447744640566);
    return 0;
}
