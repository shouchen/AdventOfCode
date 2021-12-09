#include "Utils.h"

set<pair<int, int>> visited;
int min_cost = INT_MAX;

void recurse(std::vector<std::vector<int>> &grid, std::vector<std::pair<int, int>> &route, int row, int col, int cost)
{
    if (row < 0 || row > 9 || col < 0 || col > 9 || visited.find(make_pair(row, col)) != visited.end())
        return;

    if (row == 9 && col == 9)
    {
        if (cost < min_cost)
        {
            min_cost = cost;
            cout << "cost = " << cost << endl;
        }
        return;
    }

    cost += grid[row][col];
    route.push_back(make_pair(row, col));
    auto new_visited = visited.insert(make_pair(row, col)).first;

    recurse(grid, route, row, col - 1, cost);
    recurse(grid, route, row, col + 1, cost);
    recurse(grid, route, row - 1, col, cost);
    recurse(grid, route, row + 1, col, cost);

    cost -= grid[row][col];
    route.pop_back();
    visited.erase(new_visited);
}

vector<pair<int, int>> find_cheapest_route(std::vector<std::vector<int>> &grid)
{
    // TODO: Don't overwrite what's passed in
    grid = { {  0, 6, 14,  3,  0,  1,  2, 51, 52, 53 },
             {  7,  1, 99,  1,  1,  9, 22, 68, 69, 69 },
             {  3,  2,  1,  0, 50, 50, 51, 50, 50,  0 },
             { 15, 16, 25, 25, 15, 16, 25, 25, 15, 16 },
             {  0,  0,  0,  0,  0,  1,  0,  0,  0,  0 },
             {  7,  8,  9, 10,  1,  2,  3,  4,  5,  6 },
             { 90, 80, 70, 60, 50, 40, 30, 20, 10,  0 },
             {  0, 11, 22, 33, 44, 55, 66, 77, 88, 99 },
             { 50,  0, 50,  0, 50,  0, 50,  0, 50,  0 },
             { 36, 64, 25, 16,  4, 71, 81, 91,  3, 15 }
    };

    vector<pair<int, int>> route;
    recurse(grid, route, 0, 0, 0);

    for (auto &coords : route)
    {
        cout << '(' << coords.first << ',' << coords.second << ') ';
    }
    cout << endl;

    return route;
}
