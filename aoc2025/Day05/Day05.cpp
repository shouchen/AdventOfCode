#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

std::vector<std::pair<unsigned long long, unsigned long long>> ranges;

auto coalesce_range()
{
    for (auto a = 0; a < ranges.size(); a++)
        for (auto b = 0; b < ranges.size(); b++)
        {
            if (a == b)
                continue;

            // Extend A left and delete B
            if (ranges[b].first < ranges[a].first && ranges[b].second >= ranges[a].first && ranges[b].second <= ranges[a].second)
            {
                ranges[a].first = ranges[b].first;
                ranges.erase(ranges.begin() + b);
                return true;
            }

            // Just delete A
            if (ranges[b].first <= ranges[a].first && ranges[b].second >= ranges[a].second)
            {
                ranges.erase(ranges.begin() + a);
                return true;
            }
        }

    return false;
}

auto is_fresh(unsigned long long ingredient)
{
    for (const auto &range : ranges)
        if (ingredient >= range.first && ingredient <= range.second)
            return true;

    return false;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line.length())
    {
        std::stringstream ss(line);
        auto from = 0ULL, to = 0ULL;
        auto dash = '-';
        
        ss >> from >> dash >> to;
        ranges.push_back(std::make_pair(from, to));
    }

    while(coalesce_range());

    auto retval = 0;
    auto ingredient = 0ULL;

    while (file >> ingredient)
        if (is_fresh(ingredient))
            retval++;

    return retval;
}

auto do_part2()
{
    auto retval = 0LL;

    for (const auto &range : ranges)
        retval += range.second - range.first + 1;

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 611);

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 345995423801866);

    return 0;
}
