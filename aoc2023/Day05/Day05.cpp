#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Mapping
{
    long long dest, source, len;
};

std::vector<Mapping> mapping[7];

auto follow_map(int map, long long value)
{
    for (auto i = 0; i < mapping[map].size(); i++)
        if (value>= mapping[map][i].source && value < mapping[map][i].source + mapping[map][i].len)
            return value - mapping[map][i].source + mapping[map][i].dest;

    return value;
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    std::string seeds_line, line;

    std::getline(file, seeds_line);
    std::getline(file, line); // blank line after seeds

    for (int i = 0; i < 7; i++)
    {
        std::getline(file, line); // discard map heading
        while (std::getline(file, line) && !line.empty())
        {
            std::istringstream is(line);
            Mapping m;

            is >> m.dest >> m.source >> m.len;
            mapping[i].push_back(m);
        }
    }

    auto n = 0LL, len = 1LL, min = LLONG_MAX;
    std::string seeds_colon;
    std::istringstream is(seeds_line);

    is >> seeds_colon;
    while (is >> n)
    {
        if (part2)
            is >> len;

        std::cout << n << std::endl; // debug progress
        for (auto j = 0; j < len; j++)
        {
            auto curr = n + j;
            for (auto level = 0; level < 7; level++)
                curr = follow_map(level, curr);

            min = std::min(min, curr);
        }
    }

    return min;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 484023871);

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 46294175);
    return 0;
}
