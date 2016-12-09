#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

unsigned long long compute_length(const std::string &line, bool recurse)
{
    auto count = 0ULL;

    for (auto i = 0U; line[i]; i++)
    {
        if (line[i] == '(')
        {
            auto x = line.find('x', i), parenth = line.find(')', x);

            auto run_length = atoi(line.substr(i + 1, x - i - 1).c_str());
            auto num_repeats = atoi(line.substr(x + 1, parenth - x - 1).c_str());

            count += num_repeats * (recurse ? compute_length(line.substr(parenth + 1, run_length), true) : run_length);
            i = parenth + run_length;
        }
        else
        {
            count++;
        }
    }

    return count;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;

    f >> line;

    auto answer1 = compute_length(line, false);
    auto answer2 = compute_length(line, true);

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == 70186);
    assert(answer2 == 10915059201);
    return 0;
}
