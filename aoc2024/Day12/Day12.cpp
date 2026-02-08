#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <cassert>

using Grid = std::vector<std::string>;
using Point = std::pair<int, int>;
using Visited1 = std::set<std::pair<int, int>>;
using Visited2 = std::vector<std::vector<bool>>;

const struct { int dr, dc; } dirs[]{ { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0 ,1 } };

void recur(const Grid &grid, int row, int col, int &area, int &perimeter, Visited1 &visited)
{
    if (visited.find({ row, col }) != visited.end())
        return;

    visited.insert({ row, col });

    area++;
    auto label = grid[row][col];

    for (auto &dir : dirs)
    {
        auto new_row = row + dir.dr, new_col = col + dir.dc;

        if (grid[new_row][new_col] != label)
            perimeter++;
        else
            recur(grid, new_row, new_col, area, perimeter, visited);
    }
}

auto price_region(const Grid &grid, int row, int col, Visited1 &visited)
{
    auto area = 0, perimeter = 0;

    recur(grid, row, col, area, perimeter, visited);
    return area * perimeter;
}

auto check_corner(const Grid &grid, const Point &p)
{
    // Get state of all eight neighbors
    auto label = grid[p.first][p.second];
    auto above_left = grid[p.first - 1][p.second - 1] == label, above = grid[p.first - 1][p.second] == label, above_right = grid[p.first - 1][p.second + 1] == label;
    auto left =       grid[p.first][p.second - 1] == label,             /* reference point */                 right = grid[p.first][p.second + 1] == label;
    auto below_left = grid[p.first + 1][p.second - 1] == label, below = grid[p.first + 1][p.second] == label, below_right = grid[p.first + 1][p.second + 1] == label;

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

auto compute_price(const Grid &grid, int row, int col, Visited2 &visited)
{
    static constexpr std::array<Point, 4> directions{ Point(0, -1), Point(-1, 0), Point(0, 1), Point(1, 0) }; // order matters

    auto label = grid[row][col];
    auto start = Point(row, col);
    auto cost = 0LL, area = 0LL, corner = 0LL;

    std::queue<Point> q;
    q.push(start);

    while (!q.empty())
    {
        auto p = q.front(); q.pop();

        if (visited[p.first][p.second])
            continue;

        visited[p.first][p.second] = true;

        area++;
        corner += check_corner(grid, p);

        for (auto &dir : directions)
        {
            auto next = Point(p.first + dir.first, p.second + dir.second);

            if (grid[next.first][next.second] != label)
                continue;

            if (!visited[next.first][next.second])
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

    Visited1 visited1;
    Visited2 visited2(grid.size(), std::vector<bool>(grid[0].size(), false));
    std::pair<int, long long> retval;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (grid[row][col] != '#')
            {
                if (visited1.find({ row, col }) == visited1.end())
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
