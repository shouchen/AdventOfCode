#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cassert>

std::vector<std::string> input;

void read_input_into_vector(const std::string &filename)
{
    std::ifstream file(filename);
    std::string a;

    while (file >> a)
        input.push_back(a);
}

int get_index_of_single_diff(const std::string &s1, const std::string &s2)
{
    auto single_diff_index = -1;

    if (s1.length() == s2.length())
    {
        for (auto i = 0; i < s1.length(); i++)
        {
            if (s1[i] != s2[i])
            {
                if (single_diff_index > -1)
                    return -1;

                single_diff_index = i;
            }
        }
    }

    return single_diff_index;
}

unsigned do_part1()
{
    auto num_exactly2 = 0U, num_exactly3 = 0U;

    for (auto id : input)
    {
        std::map<char, unsigned> count;
        for (auto c : id)
            count[c]++;

        if (std::any_of(count.begin(), count.end(), [&](const std::pair<const char, unsigned> &x) { return x.second == 2; }))
            num_exactly2++;
        if (std::any_of(count.begin(), count.end(), [&](const std::pair<const char, unsigned> &x) { return x.second == 3; }))
            num_exactly3++;
    }

    return num_exactly2 * num_exactly3;
}

std::string do_part2()
{
    for (auto i = 0; i < input.size(); i++)
    {
        for (auto j = i + 1; j < input.size(); j++)
        {
            auto single_diff_index = get_index_of_single_diff(input[i], input[j]);

            if (single_diff_index > -1)
            {
                auto retval = input[i];
                return retval.erase(single_diff_index, 1);
            }
        }
    }

    return std::string();
}

int main()
{
    read_input_into_vector("input.txt");

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 5750);
    assert(part2 == "tzyvunogzariwkpcbdewmjhxi");
    return 0;
}
