#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

void add_dist(int from, int to, std::vector<bool> &has_galaxies, std::pair<long long, long long> &dists)
{
    auto delta = (from < to) ? 1 : -1;

    for (auto rc = from; rc != to; rc += delta)
        if (has_galaxies[rc])
            dists.first++, dists.second++;
        else
            dists.first += 2, dists.second += 1000000;
}

auto solve(const std::string &filename)
{
    std::vector<std::pair<int, int>> galaxies;
    std::vector<bool> row_has_galaxies, col_has_galaxies;
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

    auto retval = std::make_pair(0LL, 0LL);

    for (auto g1 = galaxies.begin(); g1 != galaxies.end(); g1++)
        for (auto g2 = g1 + 1; g2 != galaxies.end(); g2++)
        {
            add_dist(g1->first, g2->first, row_has_galaxies, retval);
            add_dist(g1->second, g2->second, col_has_galaxies, retval);
        }

    return retval;
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
