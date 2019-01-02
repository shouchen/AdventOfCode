#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>

struct Room
{
    Room *north = nullptr, *south = nullptr, *east = nullptr, *west = nullptr;

    Room *move_north()
    {
        if (!north) (north = new Room())->south = this;
        return north;
    }

    Room *move_south()
    {
        if (!south) (south = new Room())->north = this;
        return south;
    }

    Room *move_east()
    {
        if (!east) (east = new Room())->west = this;
        return east;
    }

    Room *move_west()
    {
        if (!west) (west = new Room())->east = this;
        return west;
    }
};

auto read_input(const std::string &filename)
{
    Room *origin = new Room();
    std::vector<std::vector<Room *>> g = { { origin } };

    std::ifstream file("input.txt");
    auto c = ' ';

    while (file >> c)
    {
        switch (c)
        {
        case 'N':
            for (auto &&room : g.back())
                room = room->move_north();
            break;
        case 'S':
            for (auto &&room : g.back())
                room = room->move_south();
            break;
        case 'E':
            for (auto &&room : g.back())
                room = room->move_east();
            break;
        case 'W':
            for (auto &&room : g.back())
                room = room->move_west();
            break;
        case '(':
            g.push_back(std::move(g.back()));
            g.push_back(g.back());
            break;
        case '|':
            (g.rbegin() + 2)->insert((g.rbegin() + 2)->end(), g.back().begin(), g.back().end());
            g.back() = *(g.rbegin() + 1);
            break;
        case ')':
            (g.rbegin() + 2)->insert((g.rbegin() + 2)->end(), g.back().begin(), g.back().end());
            g.pop_back();
            g.pop_back();

            std::sort(g.back().begin(), g.back().end());
            g.back().erase(std::unique(g.back().begin(), g.back().end()), g.back().end());
            break;
        }
    }

    return origin;
}

void do_parts(Room *graph, unsigned &part1, unsigned &part2)
{
    std::set<Room *> frontier{ graph }, closed{ graph }, next;
    std::vector<int> counts;

    while (!frontier.empty())
    {
        counts.push_back(static_cast<int>(closed.size()));
        for (auto &&r : frontier)
        {
            if (r->north && closed.insert(r->north).second)
                next.insert(r->north);
            if (r->south && closed.insert(r->south).second)
                next.insert(r->south);
            if (r->east && closed.insert(r->east).second)
                next.insert(r->east);
            if (r->west && closed.insert(r->west).second)
                next.insert(r->west);
        }

        frontier.swap(next);
        next.clear();
    }

    constexpr size_t part2_depth = 1000;
    part1 = static_cast<unsigned>(counts.size() - 1);
    part2 = (counts.size() < part2_depth) ? 0 : (counts.back() - counts[part2_depth - 1]);
}

int main()
{
    auto graph = read_input("input.txt");

    auto part1 = 0U, part2 = 0U;
    do_parts(graph, part1, part2);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 4501);
    assert(part2 == 8623);

    return 0;
}
