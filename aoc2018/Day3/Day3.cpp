#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <cassert>

std::map<std::pair<unsigned, unsigned>, unsigned> cloth;
std::set<std::pair<unsigned, unsigned>> overlapped_squares;
std::set<unsigned> non_overlapped_ids;

void process_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto pound = '#', at = '@', comma = ',', colon = ':', by = 'x';
    auto id = 0U, x = 0U, y = 0U, width = 0U, height = 0U;

    while (file >> pound >> id >> at >> x >> comma >> y >> colon >> width >> by >> height)
    {
        non_overlapped_ids.insert(id);
        
        for (auto row = 0U; row < height; row++)
        {
            for (auto column = 0U; column < width; column++)
            {
                auto loc = std::make_pair(x + column, y + row);
                if (cloth.find(loc) == cloth.end())
                {
                    cloth[loc] = id;
                }
                else
                {
                    overlapped_squares.insert(loc);
                    non_overlapped_ids.erase(cloth[loc]);
                    non_overlapped_ids.erase(id);
                }
            }
        }
    }
}

int main()
{
    process_input("input.txt");

    auto part1 = overlapped_squares.size();
    auto part2 = non_overlapped_ids.size() ? *non_overlapped_ids.begin() : 0;

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 111485);
    assert(part2 == 113U);
    return 0;
}
