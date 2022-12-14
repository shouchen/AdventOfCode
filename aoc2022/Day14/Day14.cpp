#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>

inline auto signum(int x) { return (x == 0) ? 0 : ((x < 0) ? -1 : 1); }

auto process_input(const std::string &filename)
{
    std::map<std::pair<int, int>, char> grid;
    static const auto source = std::make_pair(500, 0);
    auto max_y = INT_MIN;

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::pair<int, int> from, to;
        std::string arrow;
        auto comma = ',';

        ss >> from.first >> comma >> from.second;
        while (ss >> arrow >> to.first >> comma >> to.second)
        {
            max_y = std::max(std::max(max_y, from.second), to.second);

            auto d1 = signum(to.first - from.first), d2 = signum(to.second - from.second);
            grid[from] = '#';

            while (from != to)
            {
                from.first += d1, from.second += d2;
                grid[from] = '#';
            }
        }
    }

    auto retval = std::make_pair(-1, -1);

    for (auto units = 1; ; units++)
    {
        auto sand = source;

        while (sand.second != max_y + 1)
        {
            if (grid.find(std::make_pair(sand.first, sand.second + 1)) == grid.end())
                sand.second++;
            else if (grid.find(std::make_pair(sand.first - 1, sand.second + 1)) == grid.end())
                sand.first--, sand.second++;
            else if (grid.find(std::make_pair(sand.first + 1, sand.second + 1)) == grid.end())
                sand.first++, sand.second++;
            else
                break;
        }

        grid[sand] = 'o';

        if (sand.second == max_y + 1 && retval.first == -1)
            retval.first = units - 1;
        else if (sand == source)
        {
            retval.second = units;
            return retval;
        }
    }
}

int main()
{
    auto output = process_input("input.txt");
    std::cout << "Part One: " << output.first << std::endl;
    std::cout << "Part Two: " << output.second << std::endl;

    assert(output.first == 901);
    assert(output.second == 24589);
    return 0;
}
