#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>

std::vector<std::string> grid; // really not be needed for solution
std::map<char, std::vector<std::pair<int, int>>> antennae;
auto height = 0, width = 0;

std::set<std::pair<int, int>> antinodes;

auto count_antinodes(char frequency)
{
    auto ant = antennae[frequency];

    for (auto i = 0; i < ant.size(); i++)
        for (auto j = i + 1; j < ant.size(); j++)
        {
            auto a = ant[i], b = ant[j];
            auto row_dir = b.first - a.first, col_dir = b.second - a.second;

            auto new_antinode_row = a.first - row_dir;
            auto new_antinode_col = a.second - col_dir;
            if (new_antinode_row >= 0 && new_antinode_row < height &&
                new_antinode_col >= 0 && new_antinode_col < width)
                antinodes.insert({ new_antinode_row, new_antinode_col });

            new_antinode_row = b.first + row_dir;
            new_antinode_col = b.second + col_dir;
            if (new_antinode_row >= 0 && new_antinode_row < height &&
                new_antinode_col >= 0 && new_antinode_col < width)
                antinodes.insert({ new_antinode_row, new_antinode_col });
        }
}

auto count_antinodes2(char frequency)
{
    auto ant = antennae[frequency];

    for (auto i = 0; i < ant.size(); i++)
        for (auto j = i + 1; j < ant.size(); j++)
        {
            auto a = ant[i], b = ant[j];
            auto row_dir = b.first - a.first, col_dir = b.second - a.second;

            auto new_antinode_row = a.first;
            auto new_antinode_col = a.second;

            while (new_antinode_row >= 0 && new_antinode_row < height && new_antinode_col >= 0 && new_antinode_col < width)
            {
                antinodes.insert({ new_antinode_row, new_antinode_col });
                new_antinode_row -= row_dir;
                new_antinode_col -= col_dir;
            }

            new_antinode_row = a.first + row_dir;
            new_antinode_col = a.second + col_dir;

            while (new_antinode_row >= 0 && new_antinode_row < height && new_antinode_col >= 0 && new_antinode_col < width)
            {
                antinodes.insert({ new_antinode_row, new_antinode_col });
                new_antinode_row += row_dir;
                new_antinode_col += col_dir;
            }
        }
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        grid.push_back(line);
        for (auto i = 0; i < line.size(); i++)
            if (line[i] != '.')
                antennae[line[i]].push_back({ int(grid.size() - 1), i });
    }

    height = int(grid.size()), width = int(grid[0].length());

    for (auto frequency = '0'; frequency <= '9'; frequency++)
        count_antinodes(frequency);

    for (auto frequency = 'A'; frequency <= 'Z'; frequency++)
        count_antinodes(frequency);

    for (auto frequency = 'a'; frequency <= 'z'; frequency++)
        count_antinodes(frequency);

    return antinodes.size();
}

auto do_part2(const std::string &filename)
{
    grid.clear();
    antennae.clear();
    antinodes.clear();

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        grid.push_back(line);
        for (auto i = 0; i < line.size(); i++)
            if (line[i] != '.')
                antennae[line[i]].push_back({ int(grid.size() - 1), i });
    }

    height = int(grid.size()), width = int(grid[0].length());

    for (auto frequency = '0'; frequency <= '9'; frequency++)
        count_antinodes2(frequency);

    for (auto frequency = 'A'; frequency <= 'Z'; frequency++)
        count_antinodes2(frequency);

    for (auto frequency = 'a'; frequency <= 'z'; frequency++)
        count_antinodes2(frequency);

    return antinodes.size();
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 244);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 912);

    return 0;
}
