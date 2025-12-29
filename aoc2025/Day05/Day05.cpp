#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::pair<unsigned long long, unsigned long long>> ranges;

auto is_fresh(unsigned long long ingredient)
{
    for (const auto &range : ranges)
        if (ingredient >= range.first && ingredient <= range.second)
            return true;
 
    return false;
}

auto solve1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0;

    while (std::getline(file, line) && line.length())
    {
        std::stringstream ss(line);
        auto from = 0ULL, to = 0ULL;
        auto dash = '-';
        
        ss >> from >> dash >> to;
        ranges.push_back(std::make_pair(from, to));
    }

    auto ingredient = 0ULL;
    while (file >> ingredient)
    {
        if (is_fresh(ingredient))
            retval++;
    }

    return retval;
}

auto condense_ranges()
{
    for (auto a = 0; a < ranges.size(); a++)
        for (auto b = a + 1; b < ranges.size(); b++)
        {
            // Extend A left and delete B
            if (ranges[b].first < ranges[a].first && ranges[b].second >= ranges[a].first && ranges[b].second <= ranges[a].second)
            {
                ranges[a].first = ranges[b].first;
                ranges.erase(ranges.begin() + b);
                return true;
            }

            // Extend A right and delete B
            if (ranges[b].second > ranges[a].second && ranges[b].first >= ranges[a].first && ranges[b].first <= ranges[a].second)
            {
                ranges[a].second = ranges[b].second;
                ranges.erase(ranges.begin() + b);
                return true;
            }

            // Just delete A
            if (ranges[b].first <= ranges[a].first && ranges[b].second >= ranges[a].second)
            {
                ranges.erase(ranges.begin() + a);
                return true;
            }

            // Just delete B
            if (ranges[b].first >= ranges[a].first && ranges[b].first <= ranges[a].second &&
                ranges[b].second >= ranges[a].first && ranges[b].second <= ranges[a].second)
            {
                ranges.erase(ranges.begin() + b);
                return true;
            }
        }

    return false;
}

auto solve2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = 0LL;

    ranges.clear();

    while (std::getline(file, line) && line.length())
    {
        std::stringstream ss(line);
        auto from = 0ULL, to = 0ULL;
        auto dash = '-';

        ss >> from >> dash >> to;
        ranges.push_back(std::make_pair(from, to));
    }

    while (condense_ranges());

    for (const auto &range : ranges)
        retval += range.second - range.first + 1;

    return retval;
}

int main()
{
    auto part1 = solve1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 611);

    auto part2 = solve2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 345995423801866);

    return 0;
}
