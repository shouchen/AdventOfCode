#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <cassert>

using RowCol = std::pair<int, int>;

static const std::array<RowCol, 4> deltas{
    std::make_pair(-1, 0), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(0, 1)
};

std::vector<std::string> grid;
RowCol startpos, endpos;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line))
    {
        auto pos = line.find('S');
        if (pos != -1)
        {
            line[pos] = 'a';
            startpos = std::make_pair(grid.size(), pos);
        }

        pos = line.find('E');
        if (pos != -1)
        {
            line[pos] = 'z';
            endpos = std::make_pair(grid.size(), pos);
        }

        grid.push_back(line);
    }
}

auto do_part(bool part2)
{
    std::map<RowCol, int> dist;

    for (auto i = 0; i < grid.size(); i++)
        for (auto j = 0; j < grid[i].length(); j++)
            dist[std::make_pair(i, j)] = INT_MAX;
    dist[endpos] = 0;

    for (auto d = 0; ; d++)
    {
        auto next_dist = d + 1;

        for (auto i = 0; i < grid.size(); i++)
            for (auto j = 0; j < grid[i].length(); j++)
            {
                auto curr = std::make_pair(i, j);
                if (dist[curr] != d)
                    continue;

                for (auto &delta : deltas)
                {
                    auto next = std::make_pair(i + delta.first, j + delta.second);

                    if (next.first >= 0 && next.first < grid.size() && next.second >= 0 && next.second < grid[0].length() &&
                        grid[curr.first][curr.second] - grid[next.first][next.second] <= 1 && dist[next] > next_dist)
                    {
                        if (part2 && grid[next.first][next.second] == 'a' || next == startpos)
                            return next_dist;
                        dist[next] = next_dist;
                    }
                }
            }
    }
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part(false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 408);
    assert(part2 == 399);
    return 0;
}
