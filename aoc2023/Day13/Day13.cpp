#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

std::vector<std::string> grid;
std::vector<int> row, col;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    grid.clear();
    while (std::getline(file, line) && !line.empty())
        grid.push_back(line);
}

auto find_vref(int skip)
{
    for (auto c = 0; c < grid[0].size() - 1; c++)
    {
        if (c == skip)
            continue;

        auto reflect = true;
        for (auto a = c, b = c + 1; a >= 0 && b < grid[0].size(); a--, b++)
            if (col[a] != col[b])
            {
                reflect = false;
                break;
            }

        if (reflect)
            return c;
    }

    return -1;
}

auto find_href(int skip)
{
    for (auto r = 0; r < grid.size() - 1; r++)
    {
        if (r == skip)
            continue;

        auto reflect = true;
        for (auto a = r, b = r + 1; a >= 0 && b < grid.size(); a--, b++)
            if (row[a] != row[b])
            {
                reflect = false;
                break;
            }

        if (reflect)
            return r;
    }

    return -1;
}

// returns vref, href (vref and href are from part 1, else -1)
auto do_common_part(int vref, int href)
{
    row.clear();
    col.clear();
    while (row.size() < grid.size()) row.push_back(0);
    while (col.size() < grid[0].size()) col.push_back(0);

    for (auto c = 0; c < grid[0].size(); c++)
        for (auto r = 0; r < grid.size(); r++)
            if (grid[r][c] == '#')
                col[c] |= (1 << r);

    for (auto c = 0; c < grid.size(); c++)
        for (auto r = 0; r < grid[c].size(); r++)
            if (grid[c][r] == '#')
                row[c] |= (1 << r);

    auto vref2 = find_vref(vref - 1) + 1; // one based
    auto href2 = find_href(href - 1) + 1; // one based

    return std::make_pair(vref2, href2);
}

void flip_cell(int row, int col)
{
    auto &cell = grid[row][col];
    cell = (cell == '.') ? '#' : '.';
}

auto do_smudge(int vref, int href)
{
    for (auto srow = 0; srow < grid.size(); srow++)
    {
        for (auto scol = 0; scol < grid[srow].size(); scol++)
        {
            flip_cell(srow, scol);

            auto temp = do_common_part(vref, href);
            auto vref2 = find_vref(vref - 1) + 1; // one based
            auto href2 = find_href(href - 1) + 1; // one based

            if (vref2 > 0) return vref2;
            if (href2 > 0) return 100 * href2;

            flip_cell(srow, scol);
        }
    }

    return 0; 
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = std::make_pair(0, 0);

    while (!file.eof())
    {
        grid.clear();
        while (std::getline(file, line) && !line.empty())
            grid.push_back(line);

        auto temp = do_common_part(0, 0);
        auto vref = temp.first;
        auto href = temp.second;

        retval.first += vref + 100 * href;
        
        auto smudge = do_smudge(vref, href);
        retval.second += smudge;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");;
    std::cout << "Part One: " << answer.first<< std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 33975);
    assert(answer.second == 29083);
    return 0;
}
