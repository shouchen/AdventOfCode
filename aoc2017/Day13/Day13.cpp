#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

struct DepthInfo
{
    unsigned depth, range, scanner_cycle;
};

typedef std::vector<DepthInfo> Firewall;

Firewall read_input(const std::string &filename)
{
    std::ifstream f("filename");
    Firewall firewall;
    DepthInfo item;
    auto colon = ':';

    while (f >> item.depth >> colon >> item.range)
    {
        item.scanner_cycle = (item.range - 1) * 2;
        firewall.push_back(item);
    }

    return firewall;
}

const DepthInfo *caught_at(const Firewall &firewall, unsigned delay = 0)
{
    for (auto item : firewall)
        if ((item.depth + delay) % item.scanner_cycle == 0)
            return &item;

    return nullptr;
}

unsigned do_part1(const Firewall &firewall)
{
    auto depth_info = caught_at(firewall);
    return depth_info ? (depth_info->depth * depth_info->range) : 0;
}

unsigned do_part2(const Firewall &firewall)
{
    for (auto delay = 0U; delay < UINT_MAX; delay++)
        if (!caught_at(firewall, delay))
            return delay;

    return UINT_MAX;
}

int main()
{
    auto firewall = read_input("input-test.txt");
    assert(do_part1(firewall) == 24);
    assert(do_part2(firewall) == 10);

    firewall = read_input("input.txt");
    auto part1 = do_part1(firewall);
    auto part2 = do_part2(firewall);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 2384);
    assert(part2 == 3921270);
    return 0;
}
