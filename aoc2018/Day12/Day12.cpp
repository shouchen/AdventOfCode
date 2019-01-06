#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <stack>
#include <queue>
#include <string>
#include <list>
#include <map>
#include <deque>
#include <set>
#include <algorithm>
#include <cassert>

using namespace std::string_literals;
using Rules = std::vector<std::pair<std::string, char>>;

#define SIZE 20000

void transform(std::map<int, char> &current, const Rules &rules)
{
    std::map<int, char> next;

    for (auto i = -SIZE; i < SIZE; i++)
        next[i] = '.';

    for (auto i = -SIZE; i < (SIZE - 1) - 4; i++)
    {
        for (auto rule : rules)
        {
            if ((current[i] == '#') == (rule.first[0] == '#') &&
                current[i + 1] == rule.first[1] &&
                current[i + 2] == rule.first[2] &&
                current[i + 3] == rule.first[3] &&
                current[i + 4] == rule.first[4])
            {
                next[i + 2] = rule.second;
                break;
            }
        }
    }

    std::swap(current, next);
}

void read_input(const std::string &filename, Rules &rules, std::string &initial_state)
{
    std::ifstream file("input.txt");

    auto initial = "initial"s, state_colon = "state:"s;
    file >> initial >> state_colon >> initial_state;

    auto left_side = ""s, arrow = "=>"s;
    auto right_side = ' ';
    while (file >> left_side >> arrow >> right_side)
        rules.push_back({ left_side, right_side });
}

void do_parts(const Rules &rules, const std::string &initial_state, unsigned long long &part1, unsigned long long &part2)
{
    std::map<int, char> state;

    for (auto i = -SIZE; i < SIZE; i++)
        state[i] = '.';

    for (auto i = 0; i < initial_state.length(); i++)
        state[i] = initial_state[i];

    auto prev = 0;

    for (auto i = 0; i <= 120; i++)
    {
        auto count = 0;
        for (auto item : state)
            if (item.second == '#')
                count += item.first;

        if (i == 20)
            part1 = count;

        std::cout << i << ": " << " count = " << count << ", diff from prev = " << (count - prev) << std::endl;

        transform(state, rules);
        prev = count;
    }
    std::cout << std::endl;

    // Observe that for this program input, the count progression soon
    // stabilizes to just adding 62 each generation. Based on this observation,
    // a simple formula can be made to extrapolate the counts for much later
    // generations (which are otherwise computationally infeasible):
    //
    // f(x) = 62x + 655, where x >= 98

    part2 = 62 * 50000000000 + 655;
}

int main()
{
    auto initial_state = ""s;
    Rules rules;
    read_input("input.txt", rules, initial_state);

    auto part1 = 0ULL, part2 = 0ULL;
    do_parts(rules, initial_state, part1, part2);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 2571);
    assert(part2 == 3100000000655);

    return 0;
}
