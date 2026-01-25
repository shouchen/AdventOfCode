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

Grid grid;
Visited1 visited;

auto count_sides(char label)
{
    return 4;
}

void recur(int row, int col, int &area, int &perimeter)
{
    if (visited.find({ row, col }) != visited.end())
        return;

    visited.insert({ row, col });

    area++;
    auto label = grid[row][col];

    if (row == 0 || grid[row - 1][col] != label)
        perimeter++;
    else
        recur(row - 1, col, area, perimeter);

    if ((row == grid.size() - 1) || grid[row + 1][col] != label)
        perimeter++;
    else
        recur(row + 1, col, area, perimeter);

    if (col == 0 || grid[row][col - 1] != label)
        perimeter++;
    else
        recur(row, col - 1, area, perimeter);

    if ((col == grid[row].size() - 1) || grid[row][col + 1] != label)
        perimeter++;
    else
        recur(row, col + 1, area, perimeter);
}

auto price_region(int row, int col)
{
    auto area = 0, perimeter = 0;

    recur(row, col, area, perimeter);
    return area * perimeter;
}

auto check_corner(const Point &p)
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

auto compute_price(int row, int col, Visited2 &visited)
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
        corner += check_corner(p);

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

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        grid.push_back(line);

    auto retval = 0;
    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (visited.find({ row, col }) == visited.end())
                retval += price_region(row, col);

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    grid.clear();

    // Add a buffer of '#' around the map to avoid boundary checks
    grid.emplace_back();
    while (std::getline(file, line))
        grid.push_back('#' + line + '#');

    grid[0] = std::string(grid[1].size(), '#');
    grid.emplace_back(grid[0]);

    Visited2 visited(grid.size(), std::vector<bool>(grid[0].size(), false));
    auto total_cost = 0LL;

    for (auto row = 0; row < grid.size(); row++)
        for (auto col = 0; col < grid[row].size(); col++)
            if (!visited[row][col] && grid[row][col] != '#')
                total_cost += compute_price(row, col, visited);

    return total_cost;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 1489582);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 914966);

    return 0;
}
