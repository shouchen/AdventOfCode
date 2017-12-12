#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>

std::map<int, std::vector<int>> data;
std::set<int> visited;

void do_visit(int x)
{
    std::cout << x << std::endl;
    auto y = visited.find(x);
    if (y != visited.end())
        return;

    visited.insert(x);
    for (auto n : data[x])
        do_visit(n);
}

int main()
{
    std::ifstream f("input.txt");
    std::string s, arrow, comma;
    int pipe1, pipe2;
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

    do_visit(0);
    auto part1 = visited.size();
    std::cout << "Part 1: " << part1 << std::endl;
    assert(part1 == 380);

    int num_groups = 1;

    for (auto p : data)
    {
        if (visited.find(p.first) != visited.end())
            continue;

        num_groups++;
        std::cout << "new group #" << num_groups << " at " << p.first << std::endl;
        do_visit(p.first);
    }

    std::cout << std::endl;

    //assert(do_part1(5, "input-test.txt") == 12);

    //auto part1 = do_part1(256, "input.txt");
    //auto part2 = do_part2(256, "input.txt");

    //std::cout << "Part 1: " << part1 << std::endl;
    //std::cout << "Part 2: " << part2 << std::endl;

    //assert(part1 == 4480);
    //assert(part2 == "c500ffe015c83b60fad2e4b7d59dabc4");
    //return 0;

} // part1 380, part2 181
