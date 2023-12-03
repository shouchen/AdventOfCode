#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <ctype.h>
#include <algorithm>
#include <cassert>


using namespace std;

vector<string> grid;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

bool is_part_number(int start_row, int start_col, int end_col)
{
    for (int i = start_col - 1; i <= end_col + 1; i++)
    {
        if (i < 0)
            continue;

        if (i >= grid[start_row].size())
            continue;

        // check above
        if (start_row > 0)
        {
            if (grid[start_row - 1][i] != '.' && !isdigit(grid[start_row - 1][i]))
                return true;
        }

        // check below
        if (start_row < grid.size() - 1)
        {
            if (grid[start_row + 1][i] != '.' && !isdigit(grid[start_row + 1][i]))
                return true;
        }
    }

    // check left
    if (start_col > 0 &&
        grid[start_row][start_col - 1] != '.' &&
        !isdigit(grid[start_row][start_col - 1]))
    {
        return true;
    }

    // check right
    if (end_col < grid[start_row].size() - 1 &&
        grid[start_row][end_col + 1] != '.' &&
        !isdigit(grid[start_row][end_col + 1]))
    {
        return true;
    }

    return false;
}

struct Part { int row, col1, col2, num;  };
vector<Part> parts;


auto do_part1()
{
    auto total = 0;

    for (auto i = 0; i < grid.size(); i++)
    {
        auto &row = grid[i];

        for (auto j = 0; j < row.size(); j++)
        {
            if (!isdigit(row[j]))
                continue;

            auto start_row = i;
            auto start_col = j;

            int n = 0;
            while (j < row.size() && isdigit(row[j]))
            {
                n = 10 * n + row[j] - '0';
                j++;
            }

            auto end_col = j - 1;

            if (is_part_number(start_row, start_col, end_col))
                total += n;

            parts.push_back(Part{ start_row, start_col, end_col, n });
        }
    }

    return total;
}

int gear_ratio(int i, int j)
{
    vector<Part> adj;

    for (auto &p : parts)
    {
        if (p.row == i - 1 && j >= p.col1 - 1 && j <= p.col2 + 1) // above
            adj.push_back(p);
        else if (p.row == i + 1 && j >= p.col1 - 1 && j <= p.col2 + 1) // below
            adj.push_back(p);
        else if (p.row == i && p.col2 == j - 1) // left
            adj.push_back(p);
        else if (p.row == i && p.col1 == j + 1) // right
            adj.push_back(p);

    }

    if (adj.size() == 2)
        return adj[0].num * adj[1].num;
    else
        return 0;
}

auto do_part2()
{
    auto total = 0;

    for (auto i = 0; i < grid.size(); i++)
    {
        auto &row = grid[i];

        for (auto j = 0; j < row.size(); j++)
        {
            if (row[j] != '*')
                continue;

            total += gear_ratio(i, j);
        }
    }

    return total;
}

int main()
{
    read_input("input.txt");
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1== 553825);
    assert(part2 == 93994191);
    return 0;
}
