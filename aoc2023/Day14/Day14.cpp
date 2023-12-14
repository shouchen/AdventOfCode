#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

std::vector<std::string> grid;

void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

auto do_part1()
{
    // tilt N
    for (auto col = 0; col < grid[0].size(); col++)
    {
        for (;;)
        {
            auto moved = false;
            for (int row = 0; row < grid.size() - 1; row++)
            {
                if (grid[row][col] == '.' && grid[row + 1][col] == 'O')
                {
                    grid[row][col] = 'O', grid[row + 1][col] = '.';
                    moved = true;
                }
            }

            if (!moved)
                break;
        }
    }

    auto total = 0LL;

    for (auto i = 0; i < grid.size(); i++)
        for (auto j = 0; j < grid[0].size(); j++)
            if (grid[i][j] == 'O')
                total += grid.size() - i;

    return total;
}

std::map<std::vector<std::string>, int> seen;
int period_start = -1, period_len = -1;

auto do_part2()
{
    auto cycle = 1;
    for (cycle = 1; ; cycle++)
    {
        // tilt N
        for (auto col = 0; col < grid[0].size(); col++)
        {
            for (;;)
            {
                auto moved = false;
                for (auto row = 0; row < grid.size() - 1; row++)
                {
                    if (grid[row][col] == '.' && grid[row + 1][col] == 'O')
                    {
                        grid[row][col] = 'O', grid[row + 1][col] = '.';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }


        // tilt W
        for (auto row = 0; row < grid.size(); row++)
        {
            for (;;)
            {
                auto moved = false;
                for (auto col = 0; col < grid[0].size() - 1; col++)
                {
                    if (grid[row][col] == '.' && grid[row][col + 1] == 'O')
                    {
                        grid[row][col] = 'O', grid[row][col + 1] = '.';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }

        // tilt S
        for (auto col = 0; col < grid[0].size(); col++)
        {
            for (;;)
            {
                auto moved = false;
                for (auto row = 0; row < grid.size() - 1; row++)
                {
                    if (grid[row][col] == 'O' && grid[row + 1][col] == '.')
                    {
                        grid[row][col] = '.', grid[row + 1][col] = 'O';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }

        // tilt E
        for (auto row = 0; row < grid.size(); row++)
        {
            for (;;)
            {
                auto moved = false;
                for (auto col = 0; col < grid[0].size() - 1; col++)
                {
                    if (grid[row][col] == 'O' && grid[row][col + 1] == '.')
                    {
                        grid[row][col] = '.', grid[row][col + 1] = 'O';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }

        auto total = 0LL;

        for (auto i = 0; i < grid.size(); i++)
            for (auto j = 0; j < grid[0].size(); j++)
                if (grid[i][j] == 'O')
                    total += grid.size() - i;

        if (seen.find(grid) == seen.end())
        {
            seen[grid] = cycle;
        }
        else
        {
            period_start = seen[grid];
            period_len = cycle - seen[grid];
            break;
        }
    }

    auto index = (1000000000 - period_start) % period_len + period_start;

    for (auto &i : seen)
    {
        if (i.second == index)
        {
            grid = i.first;

            auto total = 0LL;

            for (auto i = 0; i < grid.size(); i++)
                for (auto j = 0; j < grid[0].size(); j++)
                    if (grid[i][j] == 'O')
                        total += grid.size() - i;

            return total;
        }
    }

    return -1LL;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 106990);
    assert(part2 == 100531);
    return 0;
}
