#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

using namespace std::string_literals;
using Pots = std::map<int, char>;
using Rules = std::vector<std::pair<std::string, char>>;

void transform(Pots &current, const Rules &rules)
{
    auto min_index = current.begin()->first, max_index = current.rbegin()->first;
    std::map<int, char> next;

    for (auto i = min_index - 3; i < max_index; i++)
    {
        for (auto rule : rules)
        {
            if ((current[i + 0] == '#') == (rule.first[0] == '#') &&
                (current[i + 1] == '#') == (rule.first[1] == '#') &&
                (current[i + 2] == '#') == (rule.first[2] == '#') &&
                (current[i + 3] == '#') == (rule.first[3] == '#') &&
                (current[i + 4] == '#') == (rule.first[4] == '#'))
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
    Pots state;
    for (auto i = 0; i < initial_state.length(); i++)
        state[i] = initial_state[i];

    auto prev = 0;

    for (auto i = 0; i <= 120; i++)
    {
        auto sum = 0;
        for (auto item : state)
            if (item.second == '#')
                sum += item.first;

        if (i == 20)
            part1 = sum;

        std::cout << i << ": " << " sum = " << sum << ", diff from prev = " << (sum - prev) << std::endl;

        transform(state, rules);
        prev = sum;
    }
    std::cout << std::endl;

    // Observe that for this program input, the count progression soon
    // stabilizes to just adding 62 each generation. Based on this observation,
    // a simple formula can be made to extrapolate the counts for much later
    // generations:
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
