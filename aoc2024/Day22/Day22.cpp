#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using SequenceToPrice = std::vector<int>;
using Buyers = std::vector<SequenceToPrice>;

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    Buyers buyers;
    std::vector<int> all_keys;
    int secret = 0;
    auto retval = std::make_pair(0ULL, 0);

    while (file >> secret)
    {
        buyers.push_back(std::vector<int>(19 * 19 * 19 * 19, -1));

        auto prev_price = int(secret % 10), key = 0;

        for (auto i = 0; i < 2000; i++)
        {
            secret ^= (secret << 6) & 0xffffff;
            secret ^= (secret >> 5);
            secret ^= (secret << 11) & 0xffffff;

            auto price = int(secret % 10);
            auto change = price - prev_price;

            key = (key % (19 * 19 * 19)) * 19 + (change + 9);

            if (i > 2 && buyers.back()[key] == -1)
            {
                buyers.back()[key] = price;
                all_keys.push_back(key);
            }

            prev_price = price;
        }

        retval.first += secret;
    }

    std::sort(all_keys.begin(), all_keys.end());
    all_keys.erase(std::unique(all_keys.begin(), all_keys.end()), all_keys.end());

    for (auto &i : all_keys)
    {
        auto bananas = 0;
        for (auto &b : buyers)
            if (b[i] != -1)
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

    assert(answer.first == 13584398738);
    assert(answer.second == 1612);
    return 0;
}
