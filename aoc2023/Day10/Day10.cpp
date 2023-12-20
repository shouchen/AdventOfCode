#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

enum Dir
{
    North = 0x1, South = 0x2, East = 0x4, West = 0x8
};

auto find_loop(std::vector<std::vector<int>> &exits, int row, int col)
{
    std::set<std::pair<int, int>> loop;

    for (;;)
    {
        loop.insert(std::make_pair(row, col));

        if ((exits[row][col] & North) && loop.find(std::make_pair(row - 1, col)) == loop.end())
            row--;
        else if ((exits[row][col] & South) && loop.find(std::make_pair(row + 1, col)) == loop.end())
            row++;
        else if ((exits[row][col] & East) && loop.find(std::make_pair(row, col + 1)) == loop.end())
            col++;
        else if ((exits[row][col] & West) && loop.find(std::make_pair(row, col - 1)) == loop.end())
            col--;
        else
            return loop;
    }
}

auto solve(const std::string &filename)
{
    std::vector<std::vector<int>> exits;
    auto s_row = 0, s_col = 0;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        exits.push_back(std::vector<int>());

        for (auto c : line)
        {
            switch (c)
            {
            case '|': exits.back().push_back(North | South); break;
            case '-': exits.back().push_back(East | West); break;
            case 'L': exits.back().push_back(North | East); break;
            case 'J': exits.back().push_back(North | West); break;
            case '7': exits.back().push_back(South | West); break;
            case 'F': exits.back().push_back(South | East); break;
            case 'S': s_row = int(exits.size() - 1), s_col = int(exits.back().size());
            default: exits.back().push_back(0);
            }
        }
    }

    if (s_row > 0 && exits[s_row - 1][s_col] & South)
        exits[s_row][s_col] |= North;
    if (s_row < exits.size() - 1 && exits[s_row + 1][s_col] & North)
        exits[s_row][s_col] |= South;
    if (s_col > 0 && exits[s_row][s_col - 1] & East)
        exits[s_row][s_col] |= West;
    if (s_col < exits[s_row].size() - 1 && exits[s_row][s_col + 1] & West)
        exits[s_row][s_col] |= East;

    auto loop = find_loop(exits, s_row, s_col);
    auto total_inside = 0;

    for (auto row = 0; row < exits.size(); row++)
    {
        auto inside = false;

        for (auto col = 0; col < exits[row].size(); col++)
        {
            auto is_part_of_loop = loop.find(std::make_pair(row, col)) != loop.end();

            if (is_part_of_loop && (exits[row][col] & North))
                inside = !inside;
            else if (!is_part_of_loop && inside)
                total_inside++;
        }
    }

    return std::make_pair(loop.size() / 2, total_inside);
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 7030);
    assert(answer.second == 285);
    return 0;
}
