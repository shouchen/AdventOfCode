#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <set>

std::map<unsigned, std::vector<unsigned>> data;
std::set<unsigned> visited;

std::map<unsigned, std::vector<unsigned>> read_input(const std::string &filename)
{
    std::map<unsigned, std::vector<unsigned>> data;
    std::ifstream f(filename);
    std::string s, arrow, comma;
    auto pipe1 = 0U, pipe2 = 0U;

    while (std::getline(f, s))
    {
        std::istringstream ss(s);
        ss >> pipe1 >> arrow;

        while (ss >> pipe2)
        {
            ss >> comma;
            data[pipe1].push_back(pipe2);
        }
    }

    return data;
}

void do_visit(unsigned x)
{
    auto y = visited.find(x);
    if (y != visited.end())
        return;

    visited.insert(x);
    for (auto n : data[x])
        do_visit(n);
}

unsigned do_part1()
{
    visited.clear();
    do_visit(0);
    return unsigned(visited.size());
}

unsigned do_part2()
{
    visited.clear();

    auto num_groups = 0U;

    for (auto p : data)
    {
        if (visited.find(p.first) != visited.end())
            continue;

        num_groups++;
        do_visit(p.first);
    }

    return num_groups;
}

int main()
{
    data = read_input("input-test.txt");
    assert(do_part1() == 6);
    assert(do_part2() == 2);

    data = read_input("input.txt");
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 380);
    assert(part2 == 181);
    return 0;
}
