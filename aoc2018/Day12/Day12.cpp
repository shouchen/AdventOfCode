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

#define SIZE 20000

//const unsigned grid_size = 300;
//
//int grid[grid_size + 1][grid_size + 1];
//
//int compute_power(unsigned x, unsigned y, int gsn)
//{
//    auto rack_id = x + 10;
//    auto power = rack_id * y;
//    power += gsn;
//    power *= rack_id;
//    power = power / 100 % 10;
//    power -= 5;
//    return power;
//}
//
//void populate_grid(int gsn)
//{
//    for (auto y = 1; y <= grid_size; y++)
//        for (auto x = 1; x <= grid_size; x++)
//            grid[y][x] = compute_power(x, y, gsn);
//}
//
//int get_power_of_square(unsigned x, unsigned y, unsigned n)
//{
//    auto power = 0;
//    for (auto i = 0U; i < n; i++)
//        for (auto j = 0U; j < n; j++)
//            power += grid[y + i][x + j];
//
//    return power;
//}
//
//std::string find_square_with_highest_power(unsigned min_n, unsigned max_n)
//{
//    auto highest_total_power = std::numeric_limits<int>::min();
//    auto best_x = 0U, best_y = 0U, best_n = 0U;
//
//    for (auto n = min_n; n <= max_n; n++)
//        for (auto y = 1U; y < grid_size + 1 - n; y++)
//            for (auto x = 1U; x < grid_size + 1 - n; x++)
//            {
//                auto power = get_power_of_square(x, y, n);
//                if (power > highest_total_power)
//                {
//                    highest_total_power = power;
//                    best_x = x;
//                    best_y = y;
//                    best_n = n;
//                }
//            }
//
//    std::ostringstream o;
//    o << best_x << "," << best_y;
//    if (min_n != max_n)
//        o << "," << best_n;
//    return o.str();
//}

//int lowest = 0, highest = 0;
std::map<int, char> state;
std::vector<std::pair<std::string, char>> rule;

std::map<int, char> transform(std::map<int, char> &current)
{
    std::map<int, char> retval;
    for (int i = -SIZE; i < SIZE; i++)
        retval[i] = '.';

    for (int i = -SIZE; i < (SIZE - 1) - 4; i++)
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

void dump()
{
    for (int i = -3; i <= 35; i++)
        if (state[i] == '#')
            std::cout << '#';
        else
            std::cout << '.';
}

//std::vector<int> plants;
//std::set<std::string> rules2;
//
//std::vector<int> fast_transform(const std::vector<int> &plants)
//{
//    std::vector<int> retval;
//    for (int i = plants[0] - 2; i < plants[plants.size() - 1] + 2; i++)
//    {
//        auto left2 = plants..find(i - 2) != plants.end();
//
//
//        std::string five = 
//
//    std::string retval(input.length() + 8);
//
//    for (int i = 0; i < input.length() - 4; i++)
//    {
//        char c = '.';
//        for (auto r : rule)
//        {
//            if (input[i] == r.first[0] &&
//                input[i+1] == r.first[1] &&
//                input[i+2] == r.first[2] &&
//                input[i+3] == r.first[3] &&
//                input[i+4] == r.first[4])
//            {
//                c = '#';
//                break;
//            }
//
//            retval.push_back(c);
//        }
//    }
//
//    auto a = retval.find('#');
//    auto b = retval.find_last_of('#');
//    return retval.substr(a, b - a + 1);
//}
//
//unsigned part2()
//{
//    std::ifstream file("input.txt");
//    std::string line, initial, state_colon, initial_state;
//    std::string left_side, arrow;
//    char right_side;
//
//    file >> initial >> state_colon >> initial_state;
//    for (int i = 0; i < initial_state.length(); i++)
//        if (initial_state[i] == '#')
//            plants.push_back(i);
//
//    while (file >> left_side >> arrow >> right_side)
//        if (right_side == '#')
//            rules2.insert(left_side);
//
//    state = fast_transform(initial_state);
//
//
//    // keep only list of #-producing rules, assume false otherwise
//    // state is a set of plants
//
//    return 0;
//}


long long get_count()
{
    long long count = 0;
    for (auto item : state)
    {
        if (item.second == '#')
        {
            //std::cout << item.first << std::endl;
            count += item.first;
        }
    }

    return count;
}

int main()
{
//    part2();
    
    for (int i = -20000; i < 20000; i++)
    {
        state[i] = '.';
    }

    std::ifstream file("input.txt");
    std::string line, initial, state_colon, initial_state;
    std::string left_side, arrow;
    char right_side;

    file >> initial >> state_colon >> initial_state;

    while (file >>  left_side >> arrow >> right_side)
    {
        rule.push_back(std::make_pair(left_side, right_side));
    }

    for (int i = 0; i < initial_state.length(); i++)
    {
        state[i] = initial_state[i];
    }

    for (auto i = 0ULL; i <= 1000; i++)
    {
        std::cout << i << ':';
//        dump();
        std::cout << " count = " << get_count() << " mod 12 = " << (i % 12) << std::endl;
        if (i < 1000)
            state = transform(state);
    }

    auto part1 = get_count();
    //for (auto item : state)
    //{
    //    if (item.second == '#')
    //    {
    //        std::cout << item.first << std::endl;
    //        part1 += item.first;
    //    }
    //}

    auto part2 = 0; //find_square_with_highest_power(1, 100);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 0);
    assert(part2 == 0); // 50,000,000,000 * 62 + 655 = 3100000000655 (stabilizes to adding 62 each time)

    return 0;
}
/*     0
 0: ...#.##.###.#.##...##..#..##....#.#.#.#
 1: ..#..#####.##.###...#.###..##..#..#.#.#
 2: .###.##...########.#..###...#.####..#.#
 3: .##.####..##......###.####.#..##.#.#..#
 4: ..####.#...##.....##.###..###..###.###.
 5: ..##..##....##.....#####..###..##.####.
 6: ...#...##....##....##..#..###...####.#.
 7: ..##....##....##....#.###.####..##..##.
 8: ...##....##....##..#..##.###.#...#...#.
 9: ....##....##....#.###..####.##..##..###
10: .....##....##..#..###..##..###...#..##.
11: ......##....#.###.###...#..####.###..##
12: .......##..#..##.#####.###.##..####..##
13: ........#.###..####...###.###..##.#...#
14: .......#..###..##.##..##.####...###...#
15: ......###.###...####...####.##..####...
16: ......##.#####..##.##..##..###..##.##..
17: .......####..#...####...#..###...####..
18: .......##.#.##...##.##.###.####..##.##.
19: ........###.###...#######.###.#...#####
20: ........##.#####..##.....###.##...##...
5
6
8
9
10
11
12
15
16
22
23
24
26
27
31
32
38
42
46
50
52
53
58
59
63
66
67
69
73
77
78
83
84
88
89
93
94
100
101
106
107
111
112
117
118

PUZZLE INPUT:
...#.
..#..
..###
.#..#
.##..
.##.#
.####
.###.
#.###
##...
#.##.
##.#.
#.#.#
#..#.
###..
#.#..
##.##
*/