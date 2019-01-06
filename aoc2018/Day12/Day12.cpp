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

#define SIZE 20000

std::map<int, char> state;
std::vector<std::pair<std::string, char>> rule;

auto transform(std::map<int, char> &current)
{
    std::map<int, char> retval;
    auto test = retval[-10];

    for (auto i = -SIZE; i < SIZE; i++)
        retval[i] = '.';

    for (auto i = -SIZE; i < (SIZE - 1) - 4; i++)
    {
        for (auto r : rule)
        {
            bool found = false;
            if ((current[i] == '#') == (r.first[0] == '#') &&
                current[i + 1] == r.first[1] &&
                current[i + 2] == r.first[2] &&
                current[i + 3] == r.first[3] &&
                current[i + 4] == r.first[4])
            {
                retval[i + 2] = r.second;
                found = true;
                break;
            }
        }
    }

    return retval;
}

auto get_count()
{
    auto count = 0;
    for (auto item : state)
        if (item.second == '#')
            count += item.first;

    return count;
}

void read_input(const std::string &filename, std::string &initial_state)
{
    for (auto i = -20000; i < 20000; i++)
        state[i] = '.';

    std::ifstream file("input.txt");

    auto initial = "initial"s, state_colon = "state:"s;
    file >> initial >> state_colon >> initial_state;

    auto left_side = ""s, arrow = "=>"s;
    auto right_side = ' ';
    while (file >> left_side >> arrow >> right_side)
        rule.push_back(std::make_pair(left_side, right_side));
}

void do_parts(const std::string &initial_state, unsigned &part1, unsigned long long &part2)
{
    for (auto i = 0; i < initial_state.length(); i++)
        state[i] = initial_state[i];

    auto prev = 0;

    for (auto i = 0; i <= 120; i++)
    {
        auto count = get_count();
        if (i == 20)
            part1 = count;

        std::cout << i << ": " << " count = " << count << ", diff from prev = " << (count - prev) << std::endl;
        state = transform(state);
        prev = count;
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
    read_input("input.txt", initial_state);

    auto part1 = 0U;
    auto part2 = 0ULL;
    do_parts(initial_state, part1, part2);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 2571);
    assert(part2 == 3100000000655);

    return 0;
}
