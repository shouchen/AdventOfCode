#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <cassert>

std::map<std::pair<int, int>, int> cloth;
std::set<std::pair<int, int>> overlapped_squares;
std::set<int> non_overlapped_ids;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    char pound, at, comma, colon, by;
    int id, x, y, width, height;

    while (file >> pound >> id >> at >> x >> comma >> y >> colon >> width >> by >> height)
    {
        non_overlapped_ids.insert(id);
        
        for (int row = 0; row < height; row++)
        {
            for (int column = 0; column < width; column++)
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
    read_input("input.txt");

    auto part1 = overlapped_squares.size();
    auto part2 = *non_overlapped_ids.begin();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 111485);
    assert(part2 == 113);
    return 0;
}
