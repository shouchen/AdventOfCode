#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cassert>

std::vector<std::string> input;

void read_input_into_vector(const std::string &filename)
{
    std::ifstream file(filename);
    std::string a;

    while (file >> a)
    {
        input.push_back(a);
    }
}

bool has_exactly_two_of_any_latter(const std::string &s)
{
    std::map<char, unsigned> count;

    for (auto c : s)
    {
        count[c]++;
    }

    for (auto pair : count)
    {
        if (pair.second == 2)
            return true;
    }

    return false;
}

bool has_exactly_three_of_any_latter(const std::string &s)
{
    std::map<char, unsigned> count;

    for (auto c : s)
    {
        count[c]++;
    }

    for (auto pair : count)
    {
        if (pair.second == 3)
            return true;
    }

    return false;
}

unsigned do_part1()
{
    unsigned two = 0, three = 0;
    for (auto id : input)
    {
        if (has_exactly_two_of_any_latter(id))
            two++;
        if (has_exactly_three_of_any_latter(id))
            three++;
    }
    return two * three;
}

int differ_by_one_char(const std::string &first, const std::string &second)
{
    if (first.length() != second.length()) return false;
    int diff = -1;

    for (int i = 0; i < first.length(); i++)
    {
        if (first[i] != second[i])
        {
            if (diff > -1)
                return -1;

            diff = i;
        }
    }

    return diff;
}

std::string do_part2()
{
    for (auto i = 0; i < input.size(); i++)
    {
        for (auto j = i + 1; j < input.size(); j++)
        {
            auto diff = differ_by_one_char(input[i], input[j]);
            if (diff > -1)
            {
                std::string retval;
                for (int k = 0; k < input[i].length(); k++)
                {
                    if (k != diff)
                        retval.push_back(input[i][k]);
                }

                return retval;
            }
        }
    }

    return "";
}

int main()
{
    read_input_into_vector("input.txt");

    //auto part1 = do_part1();
    auto part2 = do_part2();

    //std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 5750); // 5750
    //assert(part2 == 563); // tzyvunogzariwkpcbdewmjhxi
    return 0;
}
