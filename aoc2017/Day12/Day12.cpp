#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <set>

auto read_input(const std::string &filename)
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

void do_visit(unsigned x, std::map<unsigned, std::vector<unsigned>> &data, std::set<unsigned> &visited)
{
    if (visited.find(x) == visited.end())
    {
        visited.insert(x);
        for (auto n : data[x])
            do_visit(n, data, visited);
    }
}

void do_both_parts(std::map<unsigned, std::vector<unsigned>> &data, unsigned &group_zero_size, unsigned &num_groups)
{
    std::set<unsigned> visited;
    do_visit(0, data, visited);
    group_zero_size = unsigned(visited.size());

    num_groups = 1U;

    for (auto item : data)
    {
        if (visited.find(item.first) == visited.end())
        {
            num_groups++;
            do_visit(item.first, data, visited);
        }
    }
}

int main()
{
    auto part1 = 0U, part2 = 0U;

    auto data = read_input("input-test.txt");
    do_both_parts(data, part1, part2);
    assert(part1 == 6);
    assert(part2 == 2);

    data = read_input("input.txt");
    do_both_parts(data, part1, part2);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 380);
    assert(part2 == 181);
    return 0;
}
