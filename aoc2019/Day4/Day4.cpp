#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <cassert>

std::pair<bool, bool> check_rules(unsigned code)
{
    std::ostringstream s;
    s << code;
    auto ss = s.str();

    auto prev = ' ';
    auto run_length = 0U;
    auto has_run = false, has_run_of_exactly_two = false;

    for (auto curr : ss)
    {
        if (curr < prev)
            return std::make_pair(false, false);

        if (curr == prev)
        {
            run_length++;
            has_run = true;
        }
        else
        {
            if (run_length == 2)
                has_run_of_exactly_two = true;
            run_length = 1;
        }

        prev = curr;
    }

    if (run_length == 2)
        has_run_of_exactly_two = true;

    return std::make_pair(has_run, has_run_of_exactly_two);
}

std::pair<unsigned, unsigned> do_both_parts(unsigned first, unsigned last)
{
    auto retval = std::make_pair(0U, 0U);

    for (auto code = first; code <= last; code++)
    {
        auto result = check_rules(code);
        if (result.first) retval.first++;
        if (result.second) retval.second++;
    }

    return retval;
}

int main()
{
    std::ifstream file("input.txt");
    auto first = 0U, last = 0U;
    auto dash = '-';

    file >> first >> dash >> last;

    auto result = do_both_parts(first, last);

    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 889);
    assert(result.second == 589);
    return 0;
}
