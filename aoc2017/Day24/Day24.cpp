#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Component
{
    unsigned a, b;
    bool used = false;
};

std::vector<Component> components;

auto max_overall_strength = 0U;
auto max_length = 0U;
auto max_strength_among_longest = 0U;

void recur(unsigned ports, unsigned length, unsigned strength)
{
    max_overall_strength = std::max(strength, max_overall_strength);
    max_length = std::max(length, max_length);

    if (length == max_length)
        max_strength_among_longest = std::max(strength, max_strength_among_longest);

    for (auto &c : components)
    {
        if (!c.used && (c.a == ports || c.b == ports))
        {
            c.used = true;
            recur((c.a == ports) ? c.b : c.a, length + 1, strength + c.a + c.b);
            c.used = false;
        }
    }
}

void process_input(const std::string &filename)
{
    components.clear();

    std::ifstream f(filename);
    Component component;
    auto slash = '/';
    while (f >> component.a >> slash >> component.b)
        components.push_back(component);

    max_overall_strength = 0U;
    max_length = 0U;
    max_strength_among_longest = 0U;

    recur(0, 0, 0);
}

int main()
{
    process_input("input-test.txt");
    assert(max_overall_bridge_strength == 31);
    assert(max_bridge_strength_among_longest == 19);

    process_input("input.txt");
    std::cout << "Part 1: " << max_overall_strength << std::endl;
    std::cout << "Part 2: " << max_strength_among_longest << std::endl;

    assert(max_overall_bridge_strength == 1859);
    assert(max_bridge_strength_among_longest == 1799);
    return 0;
}
