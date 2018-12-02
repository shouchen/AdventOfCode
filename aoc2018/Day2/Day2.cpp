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
            if (input[i].length() == input[j].length())
            {
                auto single_diff_index = -1;

                for (auto k = 0; k < input[i].length(); k++)
                {
                    if (input[i][k] != input[j][k])
                    {
                        if (single_diff_index > -1)
                        {
                            single_diff_index = -1;
                            break;
                        }

                        single_diff_index = k;
                    }
                }

                if (single_diff_index > -1)
                {
                    auto retval = input[i];
                    return retval.erase(single_diff_index, 1);
                }
            }
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
