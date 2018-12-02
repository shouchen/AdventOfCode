#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <algorithm>

std::vector<std::string> input;

void read_input_into_vector(const std::string &filename)
{
    std::ifstream file(filename);
    std::string a;

    while (file >> a)
        input.push_back(a);
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

bool differ_by_one_char(const std::string &first, const std::string &second, std::string &common)
{
    if (first.length() != second.length()) return false;
    int single_diff = -1;

    for (int i = 0; i < first.length(); i++)
    {
        if (first[i] != second[i])
        {
            if (single_diff > -1)
            {
                single_diff = -1;
                break;
            }

            single_diff = i;
        }
    }

    if (single_diff > -1)
    {
        common = first;
        common.erase(single_diff, 1);
        return true;
    }

    return false;
}

std::string do_part2()
{
    for (auto i = 0; i < input.size(); i++)
    {
        for (auto j = i + 1; j < input.size(); j++)
        {
            std::string common;
            if (differ_by_one_char(input[i], input[j], common))
                return common;
        }
    }

    return "";
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
