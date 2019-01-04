#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <functional>
#include <cassert>

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

struct Cave
{
    Cave(int depth, const Index &target) : depth(depth), target(target) {}

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

    auto is_allowed(const Index &index)
    {
        if (index.x < 0 || index.y < 0)
            return false;

        switch (get_type(index.x, index.y))
        {
        case '.': return index.tools == 'T' || index.tools == 'C';
        case '=': return index.tools == 'C' || index.tools == 'N';
        case '|': return index.tools == 'T' || index.tools == 'N';
        default: return false;
        }
    }

    Index target;

private:
    int get_erosion_level(int x, int y)
    {
        auto loc = std::make_pair(x, y);
        if (erosion_level_cache.find(loc) == erosion_level_cache.end())
            erosion_level_cache[loc] = (get_geologic_index(x, y) + depth) % 20183;

        return erosion_level_cache[loc];
    }

    int get_geologic_index(int x, int y)
    {
        auto loc = std::make_pair(x, y);
        if (geologic_index_cache.find(loc) == geologic_index_cache.end())
        {
            if (x == 0 && y == 0)
                geologic_index_cache[loc] = 0;
            else if (x == target.x && y == target.y)
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

    int depth;
    std::map<std::pair<int, int>, int> erosion_level_cache;
    std::map<std::pair<int, int>, char> type_cache;
    std::map<std::pair<int, int>, int> geologic_index_cache;
};

auto do_part1(Cave &cave)
{
    auto total = 0;
    for (auto row = 0; row <= cave.target.y; row++)
    {
        for (auto col = 0; col <= cave.target.x; col++)
        {
            char type = cave.get_type(col, row);
            if (type == '.') total += 0;
            else if (type == '=') total += 1;
            else if (type == '|') total += 2;
        }
    }
    return total;
}

auto do_part2(Cave &cave)
{
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

    std::map<Index, int> dist;
    std::priority_queue<State, std::vector<State>, std::greater<State>> q;
    auto retval = 0;

    auto curr = Index{ 0, 0, 'T' };
    q.push({ 0, curr });
    dist[curr] = 0;
    
    while (!q.empty())
    {
        curr = q.top().index;
        q.pop();

        if (curr == cave.target)
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
            if (next == curr || !cave.is_allowed(next))
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
    Cave cave(5913, Index{ 8, 701, 'T' });

    auto part1 = do_part1(cave);
    auto part2 = do_part2(cave);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 6256);
    assert(part2 == 973);

    return 0;
}
