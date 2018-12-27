#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <cassert>

auto process_file(unsigned &size, std::set<char> *excluded = NULL)
{
    std::ifstream file("input.txt");
    std::string s;
    auto c = ' ', exclude = ' ';
    
    while (file >> c)
    {
        if (excluded && exclude == ' ' && excluded->find(tolower(c)) == excluded->end())
        {
            exclude = tolower(c);
            excluded->insert(tolower(c));
        }

        if (excluded && exclude != ' ' && tolower(c) == exclude)
            continue;

        if (!s.empty())
        {
            auto prev = s.back();
            if (c != prev && tolower(c) == tolower(prev))
            {
                s.pop_back();
                continue;
            }
        }

        s.push_back(c);
    }

    size = static_cast<unsigned>(s.size());
    return excluded && exclude != ' ';
}

auto do_part1()
{
    auto size = 0U;
    process_file(size);
    return size;
}

auto do_part2()
{
    std::set<char> excluded;
    auto size = 0U;
    auto exclude = ' ';
    auto lowest_size = std::numeric_limits<unsigned>::max();

    auto num_excluded = excluded.size();
    for (;;)
    {
        process_file(size, &excluded);
        if (size < lowest_size)
            lowest_size = size;

        if (excluded.size() == num_excluded)
            break;

        auto num_excluded = excluded.size();
    }

    return lowest_size;
}

int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 11476);
    assert(part2 == 5446);
    return 0;
}
