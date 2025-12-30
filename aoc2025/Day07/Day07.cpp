#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

std::vector<std::string> grid;

void dump_grid()
{
    std::cout << std::endl;

    for (auto &line : grid)
        std::cout << line << std::endl;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0;

    while (file >> line)
        grid.push_back(line);

    //dump_grid();

    for (auto row = 1; row < grid.size(); row++)
        for (auto col = 0; col < grid[0].size(); col++)
            if (grid[row - 1][col] == 'S' || grid[row - 1][col] == '|')
            {
                if (grid[row][col] == '.')
                    grid[row][col] = '|';
                else if (grid[row][col] == '^')
                    grid[row][col - 1] = grid[row][col + 1] = '|', retval++;
            }

    //dump_grid();

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1533);

    //auto part2 = do_part2("input.txt");
    //std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
