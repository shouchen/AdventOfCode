#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <cassert>

using Grid = std::vector<std::string>;

Grid grid;
std::set<std::pair<int, int>> visited;

void recur(int row, int col, int &area, int &perimeter)
{
    if (visited.find({ row, col }) != visited.end())
        return;

    visited.insert({ row, col });

    auto label = grid[row][col];

    area++;

    if (row == 0 || grid[row - 1][col] != label)
        perimeter++;

    if ((row == grid.size() - 1) || grid[row + 1][col] != label)
        perimeter++;

    if (col == 0 || grid[row][col - 1] != label)
        perimeter++;

    if ((col == grid[row].size() - 1) || grid[row][col + 1] != label)
        perimeter++;

    if ((row > 0 && visited.find({ row - 1, col }) == visited.end()) && grid[row - 1][col] == label)
        recur(row - 1, col, area, perimeter);

    if (((row < grid.size() - 1) && visited.find({ row + 1, col }) == visited.end()) && grid[row + 1][col] == label)
        recur(row + 1, col, area, perimeter);

    if ((col > 0 && visited.find({ row, col - 1 }) == visited.end()) && grid[row][col - 1] == label)
        recur(row, col - 1, area, perimeter);

    if (((col < grid[row].size() - 1) && visited.find({ row, col + 1 }) == visited.end()) && grid[row][col + 1] == label)
        recur(row, col + 1, area, perimeter);
}

auto price_region(int row, int col)
{
    auto area = 0, perimeter = 0;

    std::cout << "Region " << grid[row][col] << " at (" << row << "," << col << "): ";
    recur(row, col, area, perimeter);
    std::cout << "Area = " << area << ", Perimeter = " << perimeter << std::endl;

    return area * perimeter;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0ULL;

    while (file >> line)
        grid.push_back(line);

    for (auto row = 0 ; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (visited.find({ row, col }) == visited.end())
                retval += price_region(row, col);

    return retval;
}

auto do_part2()
{
    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1489582);

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
