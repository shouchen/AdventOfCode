#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>

auto do_part1(const std::string &filename)
{
    std::vector<std::string> grid;
    std::ifstream file(filename);
    std::string line;
    auto retval = 0;

    while (file >> line)
        grid.push_back(line);

    for (auto row = 1; row < grid.size(); row++)
        for (auto col = 0; col < grid[0].size(); col++)
            if (grid[row - 1][col] == 'S' || grid[row - 1][col] == '|')
            {
                if (grid[row][col] == '.')
                    grid[row][col] = '|';
                else if (grid[row][col] == '^')
                    grid[row][col - 1] = grid[row][col + 1] = '|', retval++;
            }

    return retval;
}

std::map<std::pair<int, int>, unsigned long long> memo;

//auto recur(const std::vector<std::string> &grid, int row, int col)
//{
//    if (row >= grid.size())
//        return 1ULL;
//
//    auto x = memo.find({ row, col });
//    if (x != memo.end())
//        return x->second;
//
//    auto retval = (grid[row][col] == '^')
//        ? recur(grid, row + 1, col - 1) + recur(grid, row + 1, col + 1)
//        : recur(grid, row + 1, col);
//
//    memo[{row, col}] = retval;
//    return retval;
//}

auto do_part2(const std::string &filename)
{
    auto recur2 = [&](auto &&self, const std::vector<std::string> &grid, int row, int col) -> unsigned long long {
        if (row >= grid.size())
            return 1ULL;

        auto x = memo.find({ row, col });
        if (x != memo.end())
            return x->second;

        auto retval = (grid[row][col] == '^')
            ? self(self, grid, row + 1, col - 1) + self(self, grid, row + 1, col + 1)
            : self(self, grid, row + 1, col);

        memo[{row, col}] = retval;
        return retval;
    };

    std::vector<std::string> grid;
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        grid.push_back(line);

    auto row = 0, col = int(grid[0].find('S'));
    return recur2(recur2, grid, row, col);
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1533);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 10733529153890);

    return 0;
}
