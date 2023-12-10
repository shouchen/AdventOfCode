#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

std::vector<std::string> grid;
std::set<std::pair<int, int>> loop;

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
    default: return 0;
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

void find_loop()
{
    int row = 0, col = 0;
    infer_s_pipe(row, col);

    for (;;)
    {
        loop.insert(std::make_pair(row, col));

        auto dirns = pipe_to_dirn(grid[row][col]);

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

    for (auto row = 0; row < grid.size(); row++)
    {
        auto inside = false;

        for (auto col = 0; col < grid[row].size(); col++)
        {
            auto is_part_of_loop = loop.find(std::make_pair(row, col)) != loop.end();

            if (is_part_of_loop && (pipe_to_dirn(grid[row][col]) & North))
                inside = !inside;
            else if (!is_part_of_loop && inside)
                count_inside++;
        }
    }

    return std::make_pair(loop.size() / 2, count_inside);
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
