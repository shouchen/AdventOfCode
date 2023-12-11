#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

std::vector<std::string> grid;
std::set<std::pair<int, int>> loop;
std::vector<std::vector<int>> exits;
auto s_row = 0, s_col = 0;

enum Dir
{
    North = 0x1, South = 0x2, East = 0x4, West = 0x8
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
    default: return 0;
    }
}

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        exits.push_back(std::vector<int>());
        grid.push_back(line);

        for (auto c : line)
        {
            exits.back().push_back(pipe_to_dirn(c));
            if (c == 'S')
                s_row = exits.size() - 1, s_col = exits.back().size() - 1;
        }
    }
}

void infer_s_pipe(int &row, int &col)
{
    int s_dirns = 0;
    
    auto i = s_row, j = s_col;

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
        exits[i][j] = pipe_to_dirn(grid[i][j]);
    }
    else if (s_dirns == (East | West))
    {
        grid[i][j] = '-';
        exits[i][j] = pipe_to_dirn(grid[i][j]);
    }
    else if (s_dirns == (North | East))
    {
        grid[i][j] = 'L';
        exits[i][j] = pipe_to_dirn(grid[i][j]);
    }
    else if (s_dirns == (North | West))
    {
        grid[i][j] = 'J';
        exits[i][j] = pipe_to_dirn(grid[i][j]);
    }
    else if (s_dirns == (South | West))
    {
        grid[i][j] = '7';
        exits[i][j] = pipe_to_dirn(grid[i][j]);
    }
    else if (s_dirns == (South | East))
    {
        grid[i][j] = 'F';
        exits[i][j] = pipe_to_dirn(grid[i][j]);
    }
    row = i, col = j;
}

void find_loop()
{
    auto row = 0, col = 0;
    infer_s_pipe(row, col);

    for (;;)
    {
        loop.insert(std::make_pair(row, col));

        auto dirns = exits[row][col];

        if ((dirns & North) && loop.find(std::make_pair(row - 1, col)) == loop.end())
            row -= 1;
        else if ((dirns & South) && loop.find(std::make_pair(row + 1, col)) == loop.end())
            row += 1;
        else if ((dirns & East) && loop.find(std::make_pair(row, col + 1)) == loop.end())
            col += 1;
        else if ((dirns & West) && loop.find(std::make_pair(row, col - 1)) == loop.end())
            col -= 1;
        else
            break;
    }
}

auto solve()
{
    auto count_inside = 0;

    for (auto row = 0; row < exits.size(); row++)
    {
        auto inside = false;

        for (auto col = 0; col < exits[row].size(); col++)
        {
            auto is_part_of_loop = loop.find(std::make_pair(row, col)) != loop.end();

            if (is_part_of_loop && (exits[row][col] & North))
                inside = !inside;
            else if (!is_part_of_loop && inside)
                count_inside++;
        }
    }

    return std::make_pair(loop.size() / 2, count_inside);
}

// TODO: Stop using grid at all. But need to fix the S input part.
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
