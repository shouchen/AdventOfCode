#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cassert>

using Grid = std::vector<std::string>;
using Visited = std::vector<std::vector<bool>>;

struct Direction { int dr, dc; } dirs[]{ { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 } }; // order matters for part2 algorithm!

void recur(const Grid &grid, int row, int col, int &area, int &perimeter, Visited &visited)
{
    if (visited[row][col])
        return;

    visited[row][col] = true;

    auto label = grid[row][col];
    area++;

    for (auto &dir : dirs)
    {
        auto new_row = row + dir.dr, new_col = col + dir.dc;

        if (grid[new_row][new_col] == label)
            recur(grid, new_row, new_col, area, perimeter, visited);
        else
            perimeter++;
    }
}

auto price_region_part1(const Grid &grid, int row, int col, Visited &visited)
{
    auto area = 0, perimeter = 0;
    recur(grid, row, col, area, perimeter, visited);
    return area * perimeter;
}

auto count_corners(const Grid &grid, int row, int col)
{
    auto label = grid[row][col];
    auto above_left = grid[row - 1][col - 1] == label, above = grid[row - 1][col] == label, above_right = grid[row - 1][col + 1] == label;
    auto left =       grid[row][col - 1]     == label,         /* reference point */        right       = grid[row][col + 1]     == label;
    auto below_left = grid[row + 1][col - 1] == label, below = grid[row + 1][col] == label, below_right = grid[row + 1][col + 1] == label;

    auto num_corners = 0;

    // check for local upper left corner at row, col
    if (!above && !left)                            //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .   .   .         .   .   .
    else if (above && left && !above_left)          //     .   P  not        .   P   *
        num_corners++; // interior corner           //     .  not  .         .   *  not

    // check for local upper right corner at row, col
    if (!above && !right)                           //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .   .   .         .   .   .
    else if (above && right && !above_right)        //    not  P   .         *   P   . 
        num_corners++; // interior corner           //     .  not  .        not  *   .

    // check for local lower left corner at row, col
    if (!below && !left)                            //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .  not  .         .   *  not
    else if (below && left && !below_left)          //     .   P  not        .   P   *
        num_corners++; // interior corner           //     .   .   .         .   .   .

    // check for local lower right corner at row, col
    if (!below && !right)                           //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .  not   .       not  *   .
    else if (below && right && !below_right)        //    not  P   .         *   P   .
        num_corners++; // interior corner           //     .   .   .         .   .   .

    return num_corners;
}

auto price_region_part2(const Grid &grid, int row, int col, Visited &visited)
{
    auto label = grid[row][col];
    auto area = 0, num_edges = 0;

    std::queue<std::pair<int, int>> q;
    q.push({ row, col });

    while (!q.empty())
    {
        auto row = q.front().first, col = q.front().second; q.pop();

        if (visited[row][col])
            continue;

        visited[row][col] = true;

        area++;
        num_edges += count_corners(grid, row, col);

        for (auto &dir : dirs)
        {
            auto next_row = row + dir.dr, next_col = col + dir.dc;

            if (grid[next_row][next_col] != label)
                continue;

            if (!visited[next_row][next_col])
                q.push({ next_row, next_col });
        }
    }

    return area * num_edges;
}

auto solve(const std::string &filename)
{
    // add border of '#' chars around the grid to avoid boundary checks
    std::ifstream file(filename);
    std::string line;
    Grid grid;

    grid.emplace_back();
    while (std::getline(file, line))
        grid.push_back('#' + line + '#');

    grid[0] = std::string(grid[1].size(), '#');
    grid.emplace_back(grid[0]);

    Visited visited1(grid.size(), std::vector<bool>(grid[0].size(), false));
    Visited visited2(grid.size(), std::vector<bool>(grid[0].size(), false));
    std::pair<int, int> retval;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (grid[row][col] != '#')
            {
                if (!visited1[row][col])
                    retval.first += price_region_part1(grid, row, col, visited1);

                if (!visited2[row][col])
                    retval.second += price_region_part2(grid, row, col, visited2);
            }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 1489582);
    assert(answer.second == 914966);
    return 0;
}
