#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <cassert>

auto do_generation(std::vector<unsigned long long> stones)
{
    std::vector<unsigned long long> retval;

    for (auto i = 0; i < stones.size(); i++)
    {
        if (stones[i] == 0)
        {
            retval.push_back(1);
            continue;
        }

        auto s = std::to_string(stones[i]);
        if (s.length() % 2 == 0)
        {
            retval.push_back(stoull(s.substr(0, s.length() / 2)));
            retval.push_back(stoull(s.substr(s.length() / 2)));
            continue;
        }

        retval.push_back(stones[i] * 2024);
    }

    return retval;
}

auto do_part1(const std::string &filename, int blinks)
{
    std::ifstream file(filename);
    std::vector<unsigned long long> stones;
    auto n = 0ULL;

    while (file >> n)
        stones.push_back(n);

    for (auto i = 1; i <= blinks; i++)
    {
        stones = do_generation(stones);
        std::cout << "After " << i << " blinks, # stones = " << stones.size() << std::endl;
    }

    return stones.size();
}

std::map<unsigned long long, unsigned long long> memo;

auto do_part2(const std::string &filename, int blinks)
{
    std::ifstream file(filename);
    std::vector<unsigned long long> stones;
    auto n = 0ULL;

    while (file >> n)
        stones.push_back(n);

    // Do for each stone individually
    for (int i = 0; i < stones.size(); i++)
    {
        std::vector<unsigned long long> current{ stones[i] };
        if (memo.find(stones[i]) == memo.end())
        {
            for (auto j = 1; j <= 25; j++)
                if (memo.find(stones[i]) == memo.end())
                    current = do_generation(current);
            memo[stones[i]] = current.size();
        }
    }

    auto retval = 0ULL;
    for (auto s : stones)
        retval += memo[s];

    // return retval; // This is the answer to part 1, and the memo field has the result after 25 ops


}

int main()
{
    //auto part1 = do_part1("input.txt", 25);
    //std::cout << "Part One: " << part1 << std::endl;
    //assert(part1 == 183484);

    auto part2 = do_part2("input.txt", 75);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 218817038947400);

    return 0;
}
