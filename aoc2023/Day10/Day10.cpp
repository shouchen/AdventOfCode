#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

std::vector<std::string> grid;
void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

enum Dir
{
    North = 0x1,
    South = 0x2,
    East = 0x4,
    West = 0x8
};

int pipe_to_dirn(char p)
{
    switch (p)
    {
    case '|': return North | South;
    case '-': return East | West;
    case 'L': return North | East;
    case 'J': return North | West;
    case '7': return South | West;
    case 'F': return South | East;
    case '.': return 0;
    default: assert(false); return 0;
    }
}

void infer_s_pipe(int &row, int &col)
{
    int s_dirns = 0;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
            if (grid[i][j] == 'S')
            {
                if (i > 0)
                {
                    char above = grid[i - 1][j];
                    if (pipe_to_dirn(above) & South) s_dirns |= North;
                }
                if (i < grid.size() - 1)
                {
                    char below = grid[i + 1][j];
                    if (pipe_to_dirn(below) & North) s_dirns |= South;
                }
                if (j > 0)
                {
                    char left = grid[i][j - 1];
                    if (pipe_to_dirn(left) & East) s_dirns |= West;
                }
                if (j < grid[i].size() - 1)
                {
                    char right = grid[i][j + 1];
                    if (pipe_to_dirn(right) & West) s_dirns |= East;
                }

                if (s_dirns == (North | South))
                {
                    grid[i][j] = '|';
                }
                else if (s_dirns == (East | West))
                {
                    grid[i][j] = '-';
                }
                else if (s_dirns == (North | East))
                {
                    grid[i][j] = 'L';
                }
                else if (s_dirns == (North | West))
                {
                    grid[i][j] = 'J';
                }
                else if (s_dirns == (South | West))
                {
                    grid[i][j] = '7';
                }
                else if (s_dirns == (South | East))
                {
                    grid[i][j] = 'F';
                }
                row = i, col = j;
                return;
            }
}

vector<vector<int>> dist;
set<pair<int, int>> loop;

auto fill_next_dist(int curr)
{
    bool updated = false;
    for (int row = 0; row < dist.size(); row++)
        for (int col = 0; col < dist[row].size(); col++)
        {
            if (dist[row][col] != curr) continue;

            auto dirns = pipe_to_dirn(grid[row][col]);

            if (dirns & North)
            {
                if (dist[row - 1][col] > curr + 1)
                {
                    dist[row - 1][col] = curr + 1;
                    loop.insert(make_pair(row - 1, col));
                    updated = true;
                }
            }
            if (dirns & South)
            {
                if (dist[row + 1][col] > curr + 1)
                {
                    dist[row + 1][col] = curr + 1;
                    loop.insert(make_pair(row + 1, col));
                    updated = true;
                }
            }
            if (dirns & East)
            {
                if (dist[row][col + 1] > curr + 1)
                {
                    dist[row][col + 1] = curr + 1;
                    loop.insert(make_pair(row, col + 1));
                    updated = true;
                }
            }
            if (dirns & West)
            {
                if (dist[row][col - 1] > curr + 1)
                {
                    dist[row][col - 1] = curr + 1;
                    loop.insert(make_pair(row, col - 1));

                    updated = true;
                }
            }
        }
    
    return updated;
}

void recur(int row, int col, int dist_so_far)
{
    if (dist[row][col] > dist_so_far)
    {
        dist[row][col] = dist_so_far;
        loop.insert(make_pair(row, col));

        auto dirns = pipe_to_dirn(grid[row][col]);

        if (dirns & North) recur(row - 1, col, dist_so_far + 1);
        if (dirns & South) recur(row + 1, col, dist_so_far + 1);
        if (dirns & East) recur(row, col + 1, dist_so_far + 1);
        if (dirns & West) recur(row, col - 1, dist_so_far + 1);
    }
}

int s_row, s_col;

void find_loop()
{
    int row = 0, col = 0;

    infer_s_pipe(row, col);
    s_row = row, s_col = col;

    for (int i = 0; i < grid.size(); i++)
    {
        dist.push_back(vector<int>());
        for (int j = 0; j < grid[i].size(); j++)
            dist.back().push_back(INT_MAX);
    }

    dist[row][col] = 0;
    loop.insert(make_pair(row, col));

    for (auto i = 0; ; i++)
        if (!fill_next_dist(i))
            break;
}

auto solve()
{
    auto retval = std::make_pair(0, 0);

    for (auto row = 0; row < grid.size(); row++)
    {
        auto inside = false;

        for (auto col = 0; col < grid[row].size(); col++)
        {
            auto is_part_of_loop = loop.find(make_pair(row, col)) != loop.end();

            if (is_part_of_loop)
                retval.first++;

            if (is_part_of_loop && (pipe_to_dirn(grid[row][col]) & North))
                inside = !inside;
            else if (!is_part_of_loop && inside)
                retval.second++;
        }
    }

    retval.first >>= 1; // just need halfway point
    return retval;
}

int main()
{
    read_grid("input.txt");
    find_loop();

    auto answer = solve();
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 7030);
    assert(answer.second == 285);
    return 0;
}
