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
            geologic_index_cache[loc] = 0;
        else if (x == target_x && y == target_y)
            geologic_index_cache[loc] = 0;
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
    default: return false;
    }
}

struct Index
{
    int x, y;
    char tools;

    bool operator<(const Index &other) const
    {
        if (x < other.x) return true;
        if (x > other.x) return false;
        if (y < other.y) return true;
        if (y > other.y) return false;
        if (tools < other.tools) return true;
        if (tools > other.tools) return false;
        return false;
    }

    bool operator>(const Index &other) const
    {
        if (x > other.x) return true;
        if (x < other.x) return false;
        if (y > other.y) return true;
        if (y < other.y) return false;
        return tools > other.tools;
    }

    bool operator==(const Index &other) const
    {
        return x == other.x && y == other.y && tools == other.tools;
    }
};

struct State
{
    int dist;
    Index xytools;

    bool operator>(const State &other) const
    {
        if (dist > other.dist) return true;
        if (dist < other.dist) return false;
        return xytools > other.xytools;
    }
};

auto do_part2()
{
    std::map<Index, int> distmap;
    std::priority_queue<State, std::vector<State>, std::greater<State>> q;

    q.push(State{ 0, { 0, 0, 'T' } });
    distmap[Index{ 0, 0, 'T' }] = 0;

    auto retval = 0;

    while (!q.empty())
    {
        State state = q.top();
        q.pop();

        auto dist = state.dist;
        auto x = state.xytools.x, y = state.xytools.y;
        auto tools = state.xytools.tools;

        auto index = Index{ x, y, tools };

        if (state.dist > distmap[index])
            continue;

        if (index == Index{ target_x, target_y, 'T' })
        {
            retval = distmap[index];
            break;
        }
        
        std::vector<std::pair<int, int>> next_loc { { x - 1, y }, { x + 1, y }, { x, y - 1 }, { x, y + 1 } };
        
        for (const auto &next : next_loc)
        {
            if (next.first < 0 || next.second < 0 ||
                !allow_with_tools(next.first, next.second, tools))
            {
                continue;
            }

            Index next_index{ next.first, next.second, tools };
            if (!distmap.count(next_index) || distmap[next_index] > distmap[index] + 1)
            {
                distmap[next_index] = distmap[index] + 1;
                q.push({ distmap[index] + 1, next_index.x, next_index.y, next_index.tools });
            }
        }

        auto next_tool = ' ';
        if (get_type(x, y) == '.') next_tool = (tools == 'T') ? 'C' : 'T';
        else if (get_type(x, y) == '=') next_tool = (tools == 'C') ? 'N' : 'C';
        else if (get_type(x, y) == '|') next_tool = (tools == 'T') ? 'N' : 'T';

        Index next_index{ x, y, next_tool };
        if (!distmap.count(next_index) || distmap[next_index] > distmap[index] + 7)
        {
            distmap[next_index] = distmap[index] + 7;
            q.push({ distmap[index] + 7, next_index.x, next_index.y, next_index.tools });
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
