#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

struct LayerInfo
{
    unsigned layer, range, scanner_period;
    auto catches(unsigned delay = 0) const { return (layer + delay) % scanner_period == 0; }
};

typedef std::vector<LayerInfo> Firewall;

auto catches(const Firewall &firewall, unsigned delay)
{
    for (const auto &item : firewall)
        if (item.catches(delay))
            return true;

    return false;
}

auto read_input(const std::string &filename)
{
    std::ifstream f(filename);
    Firewall firewall;
    LayerInfo item;
    auto colon = ':';

    while (f >> item.layer >> colon >> item.range)
    {
        item.scanner_period = (item.range - 1) * 2;
        firewall.push_back(item);
    }

    return firewall;
}

auto do_part1(const Firewall &firewall)
{
    auto severity = 0U;

    for (const auto &item : firewall)
        if (item.catches())
            severity += item.layer * item.range;

    return severity;
}

auto do_part2(const Firewall &firewall)
{
    for (auto delay = 0U; ; delay++)
        if (!catches(firewall, delay))
            return delay;
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
