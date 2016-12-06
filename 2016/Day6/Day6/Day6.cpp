// Day6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::map<char, unsigned>> frequencies;
std::vector<std::pair<char, unsigned>> most_common;

void process_line(const std::string &line)
{
    for (auto i = 0U; i < line.length(); i++)
    {
        if (frequencies.size() <= i)
        {
            frequencies.push_back(std::map<char, unsigned>());
            most_common.push_back(std::pair<char, unsigned>(' ', 0));
        }

        auto frequency = frequencies[i][line[i]]++;
        if (frequency > most_common[i].second)
        {
            most_common[i].first = line[i];
            most_common[i].second = frequency;
        }
    }
}

std::string do_for_most_common()
{
    std::string retval;
    for (auto i = 0U; i < frequencies.size(); i++)
    {
        retval.push_back(most_common[i].first);
    }

    return retval;
}

std::string do_for_least_common()
{
    std::string retval;
    for (auto i = 0U; i < frequencies.size(); i++)
    {
        char least_common_char = ' ';
        auto least_common_count = UINT_MAX;

        for (auto j : frequencies[i])
        {
            if (j.second < least_common_count)
            {
                least_common_char = j.first;
                least_common_count = j.second;
            }
        }

        retval.push_back(least_common_char);
    }

    return retval;
}

int main()
{
    std::ifstream f;
    f.open("input.txt");

    std::string line;

    while (f >> line)
    {
        process_line(line);
    }

    auto answer1 = do_for_most_common();
    auto answer2 = do_for_least_common();

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == "liwvqppc");
    assert(answer2 == "caqfbzlh");
    return 0;
}
