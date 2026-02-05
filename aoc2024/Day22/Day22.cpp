#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

using SequenceToPrice = std::unordered_map<int, int>;
using Buyers = std::vector<SequenceToPrice>;

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    Buyers buyers;
    std::unordered_set<int> all_keys;
    uint32_t secret = 0;
    auto retval = std::make_pair(0ULL, 0);

    auto debug_key = ((-2 + 10) << 24) | ((1 + 10) << 16) | ((-1 + 10) << 8) | (3 + 10);

    while (file >> secret)
    {
        buyers.push_back({});

        auto prev_price = int(secret % 10), key = 0;

        for (auto i = 0; i < 2000; i++)
        {
            secret ^= (secret << 6) & 0xffffff;
            secret ^= (secret >> 5);
            secret ^= (secret << 11) & 0xffffff;

            auto price = int(secret % 10);
            auto change = price - prev_price;

            key = (key >> 8) | (change + 10) << 24;

            if (i > 2 && buyers.back().find(key) == buyers.back().end())
            {
                buyers.back()[key] += price;
                all_keys.insert(key);
            }

            prev_price = price;
        }

        retval.first += secret;
    }

    for (auto &i : all_keys)
    {
        auto bananas = 0;
        for (auto &b : buyers)
            if (b.find(i) != b.end())
                bananas += b[i];

        retval.second = std::max(retval.second, bananas);
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(part1 == 13584398738);
    assert(part2 == 1612);
    return 0;
}
