#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cassert>

using namespace std;

map<pair<int,int>, char> grid;

int dump()
{
    int min_row = INT_MAX, max_row = INT_MIN, min_col = INT_MAX, max_col = INT_MIN;
    for (auto &i : grid)
    {
        min_row = min(i.first.first, min_row);
        max_row = max(i.first.first, max_row);
        min_col = min(i.first.second , min_col);
        max_col = max(i.first.second, max_col);
    }

    auto dot_count = 0;
    for (int i = min_row; i <= max_row; i++)
    {
        for (int j = min_col; j <= max_col; j++)
            if (grid.find(make_pair(i, j)) == grid.end())
            {
                cout << '.';
                dot_count++;
            }
            else
                cout << '#';
        cout << endl;
    }
    cout << "dots: " << dot_count << endl << endl;

    return dot_count;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    int row = 0;

    while (getline(file, line))
    {
        for (int i = 0; i < line.length(); i++)
            if (line[i] == '#')
                grid[make_pair(row, i)] = '#';
        row++;
    }

    dump();

    auto dir_order = 0;

    for (int round = 1; round <= 10; round++)
    {
        map<pair<int, int>, char> new_grid;

        map < pair<int, int>, vector<pair<int, int>>> proposals; // to and list of froms

        for (auto &i : grid)
        {
            auto pos = i.first;
            auto row = pos.first, col = pos.second;

            auto num_neighbors = 0;
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0) continue;
                    if (grid.find(make_pair(row + i, col + j)) != grid.end())
                        num_neighbors++;
                }

            if (num_neighbors == 0)
            {
                continue;
            }

            // NSWE
            if (dir_order == 0)
            {
                if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                         grid.find(make_pair(row + 1, col)) == grid.end() &&
                         grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                         grid.find(make_pair(row, col - 1)) == grid.end() &&
                         grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                         grid.find(make_pair(row, col + 1)) == grid.end() &&
                         grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
            }
            else if (dir_order == 1) // SWEN
            {
                if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(make_pair(row, col + 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
            }
            else if (dir_order == 2) // WENS
            {
                if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(make_pair(row, col + 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
            }
            else if (dir_order == 3) // ENSW
            {
                if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(make_pair(row, col + 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
            }
        }

        // part 2 of round
        for (auto &p : proposals)
        {
            if (p.second.size() == 1)
            {
                grid[p.first] = grid[p.second[0]];
                auto temp = grid.find(p.second[0]);
                grid.erase(temp);
            }
        }

        dir_order++;
        if (dir_order > 3) dir_order = 0;

        cout << "After " << round << " rounds:" << endl;
        dump();
    }

        


    return 0;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    int row = 0;

    while (getline(file, line))
    {
        for (int i = 0; i < line.length(); i++)
            if (line[i] == '#')
                grid[make_pair(row, i)] = '#';
        row++;
    }

    dump();

    auto dir_order = 0;

    for (int round = 1; ; round++)
    {
        map<pair<int, int>, char> new_grid;

        map < pair<int, int>, vector<pair<int, int>>> proposals; // to and list of froms

        for (auto &i : grid)
        {
            auto pos = i.first;
            auto row = pos.first, col = pos.second;

            auto num_neighbors = 0;
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0) continue;
                    if (grid.find(make_pair(row + i, col + j)) != grid.end())
                        num_neighbors++;
                }

            if (num_neighbors == 0)
            {
                continue;
            }

            // NSWE
            if (dir_order == 0)
            {
                if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(make_pair(row, col + 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
            }
            else if (dir_order == 1) // SWEN
            {
                if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(make_pair(row, col + 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
            }
            else if (dir_order == 2) // WENS
            {
                if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
                else if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(make_pair(row, col + 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
            }
            else if (dir_order == 3) // ENSW
            {
                if (grid.find(make_pair(row - 1, col + 1)) == grid.end() &&
                    grid.find(make_pair(row, col + 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row, col + 1)].push_back(pos); // E
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row - 1, col)) == grid.end() &&
                    grid.find(make_pair(row - 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row - 1, col)].push_back(pos); // N
                }
                else if (grid.find(make_pair(row + 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col)) == grid.end() &&
                    grid.find(make_pair(row + 1, col + 1)) == grid.end())
                {
                    proposals[make_pair(row + 1, col)].push_back(pos); // S
                }
                else if (grid.find(make_pair(row - 1, col - 1)) == grid.end() &&
                    grid.find(make_pair(row, col - 1)) == grid.end() &&
                    grid.find(make_pair(row + 1, col - 1)) == grid.end())
                {
                    proposals[make_pair(row, col - 1)].push_back(pos); // W
                }
            }
        }

        // part 2 of round
        bool elf_moved = false;
        for (auto &p : proposals)
        {
            if (p.second.size() == 1)
            {
                grid[p.first] = grid[p.second[0]];
                auto temp = grid.find(p.second[0]);
                grid.erase(temp);
                elf_moved = true;
            }
        }

        if (!elf_moved)
        {
            cout << "Part 2 ans " << round << endl;
            return round;
        }

        dir_order++;
        if (dir_order > 3) dir_order = 0;

        cout << "After " << round << " rounds:" << endl;
        //dump();
    }

    return 0;
}

int main()
{
    //do_part1("input.txt");

    do_part2("input.txt");

    //auto part1 = process_data("test.txt", false);
    //std::cout << "Part One: " << part1 << std::endl;

    //auto part2 = process_data("test.txt", true);
    //std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 4254);
    //assert(part2 == 991);
    return 0;
}
