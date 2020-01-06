#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <limits>
#include <cassert>

// maps first string to second (backwards towards COM)
std::map<std::string, std::string> data;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
    {
        auto parenth = line.find(")");
        auto from = line.substr(0, parenth), to = line.substr(parenth + 1);
        data[to] = from;
    }
}

auto build_path_to_com(const std::string &start)
{
    std::vector<std::string> retval;
    std::string curr = start;

    for (std::string curr = start; curr != "COM"; )
    {
        curr = data[curr];
        retval.push_back(curr);
    }

    return retval;
}

auto do_part1()
{
    auto total = 0U;
    for (auto curr : data)
        total += build_path_to_com(curr.first).size();

    return total;
}

auto do_part2()
{
    auto you_path = build_path_to_com("YOU");
    auto san_path = build_path_to_com("SAN");

    for (auto i = you_path.size() - 1, j = san_path.size() - 1;; --i, --j)
        if (i < 0 || j < 0 || you_path[i] != san_path[j])
            return (i + 1) + (j + 1);
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 278744);
    assert(part2 == 475);
    return 0;
}
