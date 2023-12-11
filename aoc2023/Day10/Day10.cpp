#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

std::set<std::pair<int, int>> loop;
std::vector<std::vector<int>> exits;
auto s_row = 0, s_col = 0;

enum Dir
{
    North = 0x1, South = 0x2, East = 0x4, West = 0x8
};

auto pipe_to_dirn(char p)
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

        for (auto c : line)
        {
            exits.back().push_back(pipe_to_dirn(c));
            if (c == 'S')
                s_row = exits.size() - 1, s_col = exits.back().size() - 1;
        }
    }
}

void infer_s_pipe()
{
    auto s_dirns = 0;
    
    if (s_row > 0 && exits[s_row - 1][s_col] & South)
        s_dirns |= North;
    if (s_row < exits.size() - 1 && exits[s_row + 1][s_col] & North)
        s_dirns |= South;
    if (s_col > 0 && exits[s_row][s_col - 1] & East)
        s_dirns |= West;
    if (s_col < exits[s_row].size() - 1 && exits[s_row][s_col + 1] & West)
        s_dirns |= East;

    if (s_dirns == (North | South))
        exits[s_row][s_col] = North | South;
    else if (s_dirns == (East | West))
        exits[s_row][s_col] = East | West;
    else if (s_dirns == (North | East))
        exits[s_row][s_col] = North | East;
    else if (s_dirns == (North | West))
        exits[s_row][s_col] = North | West;
    else if (s_dirns == (South | West))
        exits[s_row][s_col] = South | West;
    else if (s_dirns == (South | East))
        exits[s_row][s_col] = South | East;
}

void find_loop()
{
    auto row = s_row, col = s_col;

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

int main()
{
    read_grid("input.txt");
    infer_s_pipe();
    find_loop();

    auto answer = solve();
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 7030);
    assert(answer.second == 285);
    return 0;
}
