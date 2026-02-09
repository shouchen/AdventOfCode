#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <cassert>

using Grid = std::vector<std::string>;
using Visited = std::vector<std::vector<bool>>;

struct Location { int row, col; };
struct Direction { int dr, dc; } dirs[]{ { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 } }; // order matters for part2 algorithm!

void recur(const Grid &grid, int row, int col, int &area, int &perimeter, Visited &visited)
{
    if (visited[row][col])
        return;

    visited[row][col] = true;

    area++;
    auto label = grid[row][col];

    for (auto &dir : dirs)
    {
        auto new_row = row + dir.dr, new_col = col + dir.dc;

        if (grid[new_row][new_col] == label)
            recur(grid, new_row, new_col, area, perimeter, visited);
        else
            perimeter++;
    }
}

auto price_region(const Grid &grid, int row, int col, Visited &visited)
{
    auto area = 0, perimeter = 0;

    recur(grid, row, col, area, perimeter, visited);
    return area * perimeter;
}

auto check_corner(const Grid &grid, const Location &loc)
{
    // Get state of all eight neighbors
    auto label = grid[loc.row][loc.col];
    auto above_left = grid[loc.row - 1][loc.col - 1] == label, above = grid[loc.row - 1][loc.col] == label, above_right = grid[loc.row - 1][loc.col + 1] == label;
    auto left =       grid[loc.row][loc.col - 1] == label,             /* reference point */                 right = grid[loc.row][loc.col + 1] == label;
    auto below_left = grid[loc.row + 1][loc.col - 1] == label, below = grid[loc.row + 1][loc.col] == label, below_right = grid[loc.row + 1][loc.col + 1] == label;

    // outside corners
    auto num_corners = 0;

    // check for local upper left corner at p
    if (!above && !left)                            //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .   .   .         .   .   .
    else if (above && left && !above_left)          //     .   P  not        .   P   *
        num_corners++; // interior corner           //     .  not  .         .   *  not

    // check for local upper right corner at p
    if (!above && !right)                           //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .   .   .         .   .   .
    else if (above && right && !above_right)        //    not  P   .         *   P   . 
        num_corners++; // interior corner           //     .  not  .        not  *   .

    // check for local lower left corner at p
    if (!below && !left)                            //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .  not  .         .   *  not
    else if (below && left && !below_left)          //     .   P  not        .   P   *
        num_corners++; // interior corner           //     .   .   .         .   .   .

    // check for local lower right corner at p
    if (!below && !right)                           //  Exterior Corner   Interior Corner
        num_corners++; // exterior corner           //     .  not   .       not  *   .
    else if (below && right && !below_right)        //    not  P   .         *   P   .
        num_corners++; // interior corner           //     .   .   .         .   .   .

    return num_corners;
}

auto compute_price(const Grid &grid, int row, int col, Visited &visited)
{
    auto label = grid[row][col];
    auto start = Location{ row, col };
    auto cost = 0LL, area = 0LL, corner = 0LL;

    std::queue<Location> q;
    q.push(start);

    while (!q.empty())
    {
        auto loc = q.front(); q.pop();

        if (visited[loc.row][loc.col])
            continue;

        visited[loc.row][loc.col] = true;

        area++;
        corner += check_corner(grid, loc);

        for (auto &dir : dirs)
        {
            auto next = Location{ loc.row + dir.dr, loc.col + dir.dc };

            if (grid[next.row][next.col] != label)
                continue;

            if (!visited[next.row][next.col])
                q.push(next);
        }
    }

    return area * corner;
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

    // do both parts together
    Visited visited1(grid.size(), std::vector<bool>(grid[0].size(), false));
    Visited visited2(grid.size(), std::vector<bool>(grid[0].size(), false));
    std::pair<int, long long> retval;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (grid[row][col] != '#')
            {
                if (!visited1[row][col])
                    retval.first += price_region(grid, row, col, visited1);

                if (!visited2[row][col])
                    retval.second += compute_price(grid, row, col, visited2);
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
