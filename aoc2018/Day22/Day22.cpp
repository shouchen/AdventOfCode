#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <cassert>

unsigned depth = 5913, target_x = 8, target_y = 701;
//unsigned depth = 510, target_x = 10, target_y = 10;

unsigned get_geologic_index(unsigned x, unsigned y);

std::map<std::pair<unsigned, unsigned>, unsigned> erosion_level_cache;
unsigned get_erosion_level(unsigned x, unsigned y)
{
    auto loc = std::make_pair(x, y);
    if (erosion_level_cache.find(loc) == erosion_level_cache.end())
        erosion_level_cache[loc] = (get_geologic_index(x, y) + depth) % 20183;

    return erosion_level_cache[loc];
}

std::map<std::pair<unsigned, unsigned>, char> type_cache;
char get_type(unsigned x, unsigned y)
{
    auto loc = std::make_pair(x, y);
    if (type_cache.find(loc) == type_cache.end())
    {
        auto el_mod_3 = get_erosion_level(x, y) % 3;
        if (el_mod_3 == 0) type_cache[loc] = '.';
        else if (el_mod_3 == 1) type_cache[loc] = '=';
        else type_cache[loc] = '|';
    }

    return type_cache[loc];
}

std::map<std::pair<unsigned, unsigned>, unsigned> geologic_index_cache;
unsigned get_geologic_index(unsigned x, unsigned y)
{
    auto loc = std::make_pair(x, y);
    if (geologic_index_cache.find(loc) == geologic_index_cache.end())
    {
        if (x == 0 && y == 0)
            geologic_index_cache[loc] = 0; // rule 1
        else if (x == target_x && y == target_y)
            geologic_index_cache[loc] = 0; // rule2
        else if (y == 0)
            geologic_index_cache[loc] = x * 16807;
        else if (x == 0)
            geologic_index_cache[loc] = y * 48271;
        else
            geologic_index_cache[loc] = get_erosion_level(x - 1, y) * get_erosion_level(x, y - 1);
    }

    return geologic_index_cache[loc];
}

unsigned do_part1()
{
    unsigned total = 0;
    for (auto row = 0U; row <= target_y; row++)
    {
        for (auto col = 0U; col <= target_x; col++)
        {
            char type = get_type(col, row);
            if (type == '.') total += 0;
            else if (type == '=') total += 1;
            else if (type == '|') total += 2;
        }
    }
    return total;
}

bool allow_with_tools(unsigned x, unsigned y, char tools)
{
    switch (get_type(x, y))
    {
    case '.': return tools == 'T' || tools == 'C';
    case '=': return tools == 'C' || tools == 'N';
    case '|': return tools == 'T' || tools == 'N';
    }

    return false;
}

struct State
{
    unsigned x, y;
    char tools; // T, C, N
    unsigned cost_so_far;
};

struct Index
{
    unsigned x, y;
    char tools;
};

bool operator<(const Index &lhs, const Index &rhs)
{
    if (lhs.x < rhs.x) return true;
    if (lhs.x > rhs.x) return false;
    if (lhs.y < rhs.y) return true;
    if (lhs.y > rhs.y) return false;
    if (lhs.tools < rhs.tools) return true;
    if (lhs.tools > rhs.tools) return false;
    return false;
}

unsigned do_part2()
{
    std::map<Index, unsigned> best_cost;
    auto min_cost = std::numeric_limits<unsigned>::max();

    auto cmp = [](State &lhs, State &rhs)
    {
        if (lhs.cost_so_far > rhs.cost_so_far) return true;
        if (lhs.cost_so_far < rhs.cost_so_far) return false;
        if (lhs.x > rhs.x) return true;
        if (lhs.x < rhs.x) return false;
        if (lhs.y > rhs.y) return true;
        if (lhs.y < rhs.y) return false;
        if (lhs.tools > rhs.tools) return true;
        if (lhs.tools < rhs.tools) return false;
        return false;
    };
    std::priority_queue<State, std::vector<State>, decltype(cmp)> q(cmp);

    q.push(State{ 0, 0, 'T', 0 });
    best_cost[Index{ 0, 0, 'T' }] = 0;

    while (!q.empty())
    {
        State state = q.top();
        q.pop();

        // If disallowed, stop this
        if (!allow_with_tools(state.x, state.y, state.tools))
            continue;

        // If there was already a cheaper way to get here, stop looking at this.
        auto temp = Index{ state.x, state.y, state.tools };
        if (best_cost.find(temp) != best_cost.end() && state.cost_so_far > best_cost[temp])
            continue;
        best_cost[temp] = state.cost_so_far;

        //std::cout << "(" << state.x << "," << state.y << "), tools=" << state.tools << ", cost=" << state.cost_so_far << std::endl;

        if (state.x == target_x && state.y == target_y && state.tools == 'T')
        {
            min_cost = state.cost_so_far;
            break;
        }

        q.push(State{ state.x + 1, state.y, state.tools, state.cost_so_far + 1 });
        q.push(State{ state.x, state.y + 1, state.tools, state.cost_so_far + 1 });
        if (state.x > 0)
            q.push(State{ state.x - 1, state.y, state.tools, state.cost_so_far + 1 });
        if (state.y > 0)
            q.push(State{ state.x, state.y - 1, state.tools, state.cost_so_far + 1 });

        if (get_type(state.x, state.y) == '.') state.tools = (state.tools == 'T') ? 'C' : 'T';
        else if (get_type(state.x, state.y) == '=') state.tools = (state.tools == 'C') ? 'N' : 'C';
        else if (get_type(state.x, state.y) == '|') state.tools = (state.tools == 'T') ? 'N' : 'T';
        q.push(State{ state.x, state.y, state.tools, state.cost_so_far + 7 });
    }

    return min_cost;
}

void dump(unsigned max_x, unsigned max_y)
{
    for (auto row = 0U; row <= max_y; row++)
    {
        for (auto col = 0U; col <= max_x; col++)
        {
            if (row == 0 && col == 0) std::cout << "M";
            else if (row == target_y && col == target_x) std::cout << "T";
            else std::cout << get_type(col, row);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int main()
{
    //std::ifstream file("input.txt");

    //assert(get_geologic_index(0, 0) == 0);
    //assert(get_erosion_level(0, 0) == 510);
    //assert(get_type(0, 0) == '.');

    //assert(get_geologic_index(1, 0) == 16807);
    //assert(get_erosion_level(1, 0) == 17317);
    //assert(get_type(1, 0) == '=');

    //assert(get_geologic_index(0, 1) == 48271);
    //assert(get_erosion_level(0, 1) == 8415);
    //assert(get_type(0, 1) == '.');

    //assert(get_geologic_index(1, 1) == 145722555.);
    //assert(get_erosion_level(1, 1) == 1805);
    //assert(get_type(1, 1) == '|');

    //assert(get_geologic_index(10, 10) == 0);
    //assert(get_erosion_level(10, 10) == 510);
    //assert(get_type(10, 10) == '.');

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    //dump(15, 16);

    assert(part1 == 114); // test input
    assert(part2 == 45); // test input

    //assert(part1 == 6256); // real problem
    //assert(part2 == 973); // real problem

    return 0;
}

