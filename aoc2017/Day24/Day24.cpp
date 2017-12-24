#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <vector>
#include <string>
#include <algorithm>

struct Port
{
    unsigned a, b;
    bool used;
};

std::vector<Port> ports;
std::vector<Port> bridge;

std::vector<Port> best_bridge;
unsigned best_bridge_cost = 0;

void recur(unsigned prev, unsigned total_so_far)
{
    if (total_so_far > best_bridge_cost)
    {
        best_bridge = bridge;
        best_bridge_cost = total_so_far;
    }

    for (auto &port : ports)
    {
        if (port.used)
            continue;

        if (port.a == prev)
        {
            bridge.push_back(port);
            port.used = true;
            recur(port.b, total_so_far + port.a + port.b);
            port.used = false;
            bridge.pop_back();
        }
        else if (port.b == prev)
        {
            bridge.push_back(port);
            port.used = true;
            recur(port.a, total_so_far + port.a + port.b);
            port.used = false;
            bridge.pop_back();
        }
    }
}

std::vector<Port> longest_bridge;
unsigned longest_bridge_cost;

void recur2(unsigned prev, unsigned total_so_far)
{
    if (bridge.size() > longest_bridge.size())
    {
        longest_bridge = bridge;
        longest_bridge_cost = total_so_far;
    }
    else if (bridge.size() == longest_bridge.size() &&
        total_so_far > longest_bridge_cost)
    {
        longest_bridge = bridge;
        longest_bridge_cost = total_so_far;
    }

    for (auto &port : ports)
    {
        if (port.used)
            continue;

        if (port.a == prev)
        {
            bridge.push_back(port);
            port.used = true;
            recur2(port.b, total_so_far + port.a + port.b);
            port.used = false;
            bridge.pop_back();
        }
        else if (port.b == prev)
        {
            bridge.push_back(port);
            port.used = true;
            recur2(port.a, total_so_far + port.a + port.b);
            port.used = false;
            bridge.pop_back();
        }
    }
}

unsigned do_part1()
{
    recur(0, 0);
    return best_bridge_cost;
}

unsigned do_part2()
{
    recur2(0, 0);
    return longest_bridge_cost;
}

int main()
{
    std::ifstream f("input.txt");
    Port port;
    char slash;
    port.used = false;
    while (f >> port.a >> slash >> port.b)
        ports.push_back(port);

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 1859);
    assert(part2 == 1799);
    return 0;
}
