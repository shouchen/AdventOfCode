#include "stdafx.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>

template<typename T>
std::ostream &operator<<(std::ostream &ostream, const std::vector<T> &v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(ostream));
    return ostream;
}

template<typename T>
bool operator==(const std::vector<T> &lhs, const std::vector<T> &rhs)
{
    if (lhs.size() != rhs.size()) return false;
    for (auto i = 0; i < lhs.size(); i++)
        if (lhs[i] != rhs[i]) return false;
    return true;
}

std::vector<unsigned> do_part1(unsigned input)
{
    std::vector<unsigned> recipes { 3 , 7 };
    auto e1 = 0U, e2 = 1U;

    for (;;)
    {
        auto total = recipes[e1] + recipes[e2];

        if (total >= 10)
        {
            recipes.push_back(1);
            total -= 10;
        }

        recipes.push_back(total);

        if (recipes.size() >= input + 10)
        {
            std::vector<unsigned> retval;
            for (auto i = 0; i < 10; i++)
                retval.push_back(recipes[input++]);
            return retval;
        }

        e1 = (e1 + recipes[e1] + 1) % recipes.size();
        e2 = (e2 + recipes[e2] + 1) % recipes.size();
    }
}

unsigned do_part2(const std::vector<unsigned> &input)
{
    std::vector<unsigned> recipes { 3, 7 };
    auto e1 = 0U, e2 = 1U, checked_index = 0U;

    for (;;)
    {
        auto total = recipes[e1] + recipes[e2];

        if (total >= 10)
        {
            recipes.push_back(1);
            total -= 10;
        }

        recipes.push_back(total);

        for (auto i = checked_index; i + input.size() < recipes.size(); i++)
        {
            auto match = true;
            for (auto j = 0; j < input.size(); j++)
            {
                if (recipes[i + j] != input[j])
                {
                    match = false;
                    break;
                }
            }

            if (match)
                return i;

            checked_index = std::max(checked_index, i);
        }

        e1 = (e1 + recipes[e1] + 1) % recipes.size();
        e2 = (e2 + recipes[e2] + 1) % recipes.size();
    }
}

int main()
{
    auto part1 = do_part1(290431);
    auto part2 = do_part2({ 2, 9, 0, 4, 3, 1 });

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    std::vector<unsigned> part1_answer { 1, 7, 7, 6, 7, 1, 8, 1, 7, 5 };
    assert(part1 == part1_answer);
    assert(part2 == 20220949);

    return 0;
}
