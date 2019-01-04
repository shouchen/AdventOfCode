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
auto get_erosion_level(int x, int y)
{
    auto loc = std::make_pair(x, y);
    if (erosion_level_cache.find(loc) == erosion_level_cache.end())
        erosion_level_cache[loc] = (get_geologic_index(x, y) + depth) % 20183;

    return erosion_level_cache[loc];
}

std::map<std::pair<int, int>, char> type_cache;
auto get_type(int x, int y)
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

auto do_part1()
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

struct Index
{
    int x, y;
    char tools;

    auto allowed() const
    {
        if (x < 0 || y < 0)
            return false;

        switch (get_type(x, y))
        {
        case '.': return tools == 'T' || tools == 'C';
        case '=': return tools == 'C' || tools == 'N';
        case '|': return tools == 'T' || tools == 'N';
        default: return false;
        }
    }

    bool operator<(const Index &other) const
    {
        if (x < other.x) return true;
        if (x > other.x) return false;
        if (y < other.y) return true;
        if (y > other.y) return false;
        return tools < other.tools;
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
    Index index;

    bool operator>(const State &other) const
    {
        if (dist > other.dist) return true;
        if (dist < other.dist) return false;
        return index > other.index;
    }
};

auto do_part2()
{
    std::map<Index, int> dist;
    std::priority_queue<State, std::vector<State>, std::greater<State>> q;

    const auto target = Index{ target_x, target_y, 'T' };
    auto retval = 0;

    auto curr = Index{ 0, 0, 'T' };
    q.push({ 0, curr });
    dist[curr] = 0;
    
    while (!q.empty())
    {
        curr = q.top().index;
        q.pop();

        if (curr == target)
        {
            retval = dist[curr];
            break;
        }
        
        std::vector<Index> next_indices {
            { curr.x - 1, curr.y, curr.tools }, { curr.x + 1, curr.y, curr.tools },
            { curr.x, curr.y - 1, curr.tools }, { curr.x, curr.y + 1, curr.tools },
            { curr.x, curr.y, 'C' }, { curr.x, curr.y, 'N' }, { curr.x, curr.y, 'T' }
        };

        for (const auto &next : next_indices)
        {
            if (next == curr || !next.allowed())
                continue;

            auto cost = (curr.tools == next.tools) ? 1 : 7;
            if (!dist.count(next) || dist[curr] + cost < dist[next])
            {
                dist[next] = dist[curr] + cost;
                q.push({ dist[next], { next.x, next.y, next.tools } });
            }
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
