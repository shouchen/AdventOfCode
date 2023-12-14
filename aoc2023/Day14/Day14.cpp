#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

using Grid = std::vector<std::string>;
Grid grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

void tilt_v(char c1, char c2)
{
    for (auto col = 0; col < grid[0].size(); col++)
        for (auto row = 0, save = -1; row < grid.size(); row++)
            if (grid[row][col] == '#')
                save = -1;
            else if (grid[row][col] == c1 && save == -1)
                save = row;
            else if (grid[row][col] == c2 && save > -1)
                std::swap(grid[save++][col], grid[row][col]);
}

void tilt_h(char c1, char c2)
{
    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0, save = -1; col < grid[row].size(); col++)
            if (grid[row][col] == '#')
                save = -1; 
            else if (grid[row][col] == c1 && save == -1)
                save = col;
            else if (grid[row][col] == c2 && save > -1)
                std::swap(grid[row][save++], grid[row][col]);
}

void tilt_N() { tilt_v('.', 'O'); }
void tilt_W() { tilt_h('.', 'O'); }
void tilt_S() { tilt_v('O', '.'); }
void tilt_E() { tilt_h('O', '.'); }

auto compute_load(const Grid &grid)
{
    auto total = 0LL;

    for (auto i = 0; i < grid.size(); i++)
        for (auto j = 0; j < grid[0].size(); j++)
            if (grid[i][j] == 'O')
                total += grid.size() - i;
    
    return total;
}

auto do_part1()
{
    tilt_N();
    return compute_load(grid);
}

auto do_part2()
{
    std::map<Grid, int> seen;

    for (auto cycle = 1; ; cycle++)
    {
        tilt_N(); tilt_W(); tilt_S(); tilt_E();

        if (seen.find(grid) != seen.end())
        {
            auto period_start = seen[grid];
            auto period_len = cycle - period_start;
            auto index = (1'000'000'000 - period_start) % period_len + period_start;
            auto found = std::find_if(seen.begin(), seen.end(),
                [&index](auto &x) { return x.second == index; });

            return compute_load(found->first);
        }

        seen[grid] = cycle;
    }
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 106990);
    assert(part2 == 100531);
    return 0;
}
