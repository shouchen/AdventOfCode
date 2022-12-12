#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <array>
#include <vector>
#include <set>
#include <cassert>

static const std::array<std::pair<int, int>, 4> deltas{
    std::make_pair(-1, 0), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(0, 1)
};

bool can_move_back(const std::vector<std::string> &grid, std::map<std::pair<int, int>, int> &dist, const std::pair<int, int> &pos1, const std::pair<int, int> &pos2)
{
    return
        pos2.first >= 0 && pos2.first < grid.size() && pos2.second >= 0 && pos2.second < grid[0].length() &&
        grid[pos1.first][pos1.second] - grid[pos2.first][pos2.second] <= 1;
}

auto process_input(const std::string &filename, bool part2)
{
    std::vector<std::string> grid;
    std::set<std::pair<int, int>> visited;
    std::pair<int, int> startpos, endpos;

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

    std::map<std::pair<int, int>, int> dist;
    for (auto i = 0; i < grid.size(); i++)
        for (auto j = 0; j < grid[i].length(); j++)
            dist[std::make_pair(i, j)] = INT_MAX;
    dist[endpos] = 0;

    for (auto d = 0; ; d++)
        for (auto i = 0; i < grid.size(); i++)
            for (auto j = 0; j < grid[i].length(); j++)
            {
                auto curr = std::make_pair(i, j);
                if (dist[curr] != d)
                    continue;

                for (auto &delta : deltas)
                {
                    auto next = std::make_pair(i + delta.first, j + delta.second);
                    if (can_move_back(grid, dist, curr, next) && dist[next] > d + 1)
                    {
                        dist[next] = d + 1;
                        if (part2 && grid[next.first][next.second] == 'a' || next == startpos)
                            return d + 1;
                    }
                }
            }
}

int main()
{
    auto part1 = process_input("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_input("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 408);
    assert(part2 == 399);
    return 0;
}
