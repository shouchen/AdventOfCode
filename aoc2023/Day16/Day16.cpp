#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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

std::map<std::pair<int,int>, std::set<std::pair<int,int>>> visited;

void recur(int row, int col, int rdir, int cdir)
{
    if (row < 0 || row >= grid.size() || col < 0 || col >= grid[row].size())
        return;

    auto rc = std::make_pair(row, col), rcdir = std::make_pair(rdir, cdir);
    auto vrc = visited.find(rc);
    if (vrc != visited.end() && vrc->second.find(rcdir) != vrc->second.end())
        return;

    visited[rc].insert(rcdir);

    switch (grid[row][col])
    {
    case '/':
        return (rdir == 0)
            ? recur(row - cdir, col + rdir, -cdir, 0)
            : recur(row, col - rdir, 0, -rdir);

    case '\\':
        return (rdir == 0)
            ? recur(row + cdir, col + rdir, cdir, 0)
            : recur(row, col + rdir, 0, rdir);

    case '-':
        if (cdir == 0)
        {
            recur(row, col + rdir, 0, rdir);
            recur(row, col - rdir, 0, -rdir);
            return;
        }
        break;

    case '|':
        if (rdir == 0)
        {
            //recur(row - 1, col + rdir, -1, 0);
            //recur(row + 1, col - rdir, 1, 0);
            if (cdir == 1)
            {
                recur(row - cdir, col + rdir, -cdir, 0);
                recur(row + cdir, col - rdir, cdir, 0);
            }
            else // cdir == -1
            {
                recur(row - cdir, col - rdir, -cdir, 0);
                recur(row + cdir, col + rdir, cdir, 0);
            }
            return;
        }
        break;
    }

    recur(row + rdir, col + cdir, rdir, cdir);
}

auto count_energized(int row, int col, int rdir, int cdir)
{
    visited.clear();
    recur(row, col, rdir, cdir);
    return int(visited.size());
}

auto do_part1()
{
    return count_energized(0, 0, 0, 1);
}

auto do_part2()
{
    auto max_energy = 0;

    for (auto col = 0; col < grid[0].size(); col++)
    {
        max_energy = std::max(max_energy, count_energized(int(grid.size()) - 1, col, -1, 0));
        max_energy = std::max(max_energy, count_energized(0, col, -1, 0));
    }

    for (auto row = 0; row < grid.size(); row++)
    {
        max_energy = std::max(max_energy, count_energized(row, 0, 0, 1));
        max_energy = std::max(max_energy, count_energized(row, int(grid[0].size()) - 1, 0, -1));
    }

    return max_energy;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7434);
    assert(part2 == 8183);
    return 0;
}
