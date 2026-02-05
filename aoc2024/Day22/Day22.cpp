#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

void update_secret(long long &secret)
{
    auto temp = secret * 64;
    secret ^= temp; // mix
    secret %= 16777216; // prune

    temp = secret / 32;
    secret ^= temp; // mix
    secret %= 16777216; // prune

    temp = secret * 2048;
    secret ^= temp; // mix
    secret %= 16777216; // prune
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto secret = 0LL, retval = 0LL;

    while (file >> secret)
    {
        for (auto i = 0; i < 2000; i++)
            update_secret(secret);

        retval += secret;
    }

    return retval;
}

using BuyerCheat = std::unordered_map<int, int>; // this counts how may bananas a hashed s1..s4 will return for a single buyer
using Buyers = std::vector<BuyerCheat>; // one of the above for each agent

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    Buyers buyers;
    std::unordered_set<int> all_keys;
    auto secret = 0LL;
    auto retval = 0;

    auto debug_key = ((-2 + 10) << 24) | ((1 + 10) << 16) | ((-1 + 10) << 8) | (3 + 10);

    while (file >> secret)
    {
        buyers.push_back(BuyerCheat());

        auto price = int(secret % 10);
        auto prev_price = price;
        auto key = 0;

        for (auto i = 0; i < 2000; i++)
        {
            update_secret(secret);
            price = int(secret % 10);
            auto change = price - prev_price;
            key = (key >> 8) | (change + 10) << 24;

            if (i > 2 && buyers.back().find(key) == buyers.back().end()) // only consider first match
            {
                buyers.back()[key] += price;
                all_keys.insert(key);
            }

            prev_price = price;
        }
    }

    for (auto &i : all_keys)
    {
        auto bananas = 0;
        for (auto &b : buyers)
        {
            if (b.find(i) != b.end())
                bananas += b[i];
        }

        retval = std::max(retval, bananas);
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 13584398738);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 1612);

    return 0;
}
