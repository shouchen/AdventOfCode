#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <functional>
#include <cassert>

int depth = 5913, target_x = 8, target_y = 701;

int get_geologic_index(int x, int y);

std::map<std::pair<int, int>, int> erosion_level_cache;
int get_erosion_level(int x, int y)
{
    auto loc = std::make_pair(x, y);
    if (erosion_level_cache.find(loc) == erosion_level_cache.end())
        erosion_level_cache[loc] = (get_geologic_index(x, y) + depth) % 20183;

    return erosion_level_cache[loc];
}

std::map<std::pair<int, int>, char> type_cache;
char get_type(int x, int y)
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

std::map<std::pair<int, int>, int> geologic_index_cache;
int get_geologic_index(int x, int y)
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
    for (auto row = 0; row <= target_y; row++)
    {
        for (auto col = 0; col <= target_x; col++)
        {
            char type = get_type(col, row);
            if (type == '.') total += 0;
            else if (type == '=') total += 1;
            else if (type == '|') total += 2;
        }
    }
    return total;
}

auto allow_with_tools(int x, int y, char tools)
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
    int cost_so_far;
    int x, y;
    char tools;
};

bool operator>(const State &lhs, const State &rhs)
{
    if (lhs.cost_so_far > rhs.cost_so_far) return true;
    if (lhs.cost_so_far < rhs.cost_so_far) return false;
    if (lhs.x > rhs.x) return true;
    if (lhs.x < rhs.x) return false;
    if (lhs.y > rhs.y) return true;
    if (lhs.y < rhs.y) return false;
    return lhs.tools > rhs.tools;
}

struct Index
{
    int x, y;
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

bool operator==(const Index &lhs, const Index &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.tools == rhs.tools;
}

auto do_part2()
{
    std::map<Index, int> dist;
    std::priority_queue<State, std::vector<State>, std::greater<State>> q;

    q.push(State{ 0, 0, 0, 'T' });
    dist[Index{ 0, 0, 'T' }] = 0;

    auto retval = 0;

    while (!q.empty())
    {
        State state = q.top();
        q.pop();

        auto index = Index{ state.x, state.y, state.tools };

        if (state.cost_so_far > dist[index])
            continue;

        if (index == Index{ target_x, target_y, 'T' })
        {
            retval = dist[index];
            break;
        }
        
        std::vector<std::pair<int, int>> next_loc { { state.x - 1, state.y }, { state.x + 1, state.y }, { state.x, state.y - 1 }, { state.x, state.y + 1 } };
        
        for (const auto &next : next_loc)
        {
            if (next.first < 0 || next.second < 0)
                continue;

            if (!allow_with_tools(next.first, next.second, state.tools))
                continue;

            Index next_index{ next.first, next.second, state.tools };
            if (!dist.count(next_index) || dist[next_index] > dist[index] + 1)
            {
                dist[next_index] = dist[index] + 1;
                q.push({ dist[index] + 1, next_index.x, next_index.y, next_index.tools });
            }
        }

        auto next_tool = ' ';
        if (get_type(state.x, state.y) == '.') next_tool = (state.tools == 'T') ? 'C' : 'T';
        else if (get_type(state.x, state.y) == '=') next_tool = (state.tools == 'C') ? 'N' : 'C';
        else if (get_type(state.x, state.y) == '|') next_tool = (state.tools == 'T') ? 'N' : 'T';

        Index next_index{ state.x, state.y, next_tool };
        if (!dist.count(next_index) || dist[next_index] > dist[index] + 7)
        {
            dist[next_index] = dist[index] + 7;
            q.push({ dist[index] + 7, next_index.x, next_index.y, next_index.tools });
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 6256);
    assert(part2 == 973);

    return 0;
}

