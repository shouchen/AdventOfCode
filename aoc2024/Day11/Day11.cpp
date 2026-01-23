#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <cassert>

using StoneCounts = std::unordered_map<unsigned long long, unsigned long long>;

auto solve(const std::string &filename, int blinks)
{
    std::ifstream file(filename);
    StoneCounts stones;
    auto n = 0ULL;

    while (file >> n)
        stones[n]++;

    while (blinks--)
    {
        StoneCounts next_stones;

        for (auto [stone, count] : stones)
        {
            if (stone == 0)
            {
                next_stones[1] += count;
                continue;
            }

            auto ss = std::to_string(stone);
            if (ss.length() % 2 == 0)
            {
                auto n = int(ss.length());
                auto left = std::stoull(ss.substr(0, n / 2)), right = stoull(ss.substr(n / 2));
                next_stones[left] += count, next_stones[right] += count;
                continue;
            }

            next_stones[stone * 2024] += count;
        }

        stones = next_stones;
    }

    auto retval = 0ULL;
    for (auto pair : stones)
        retval += pair.second;

    return retval;
}

int main()
{
    auto part1 = solve("input.txt", 25);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 183484);

    auto part2 = solve("input.txt", 75);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 218817038947400);

    return 0;
}
