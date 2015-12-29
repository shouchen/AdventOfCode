// Houses.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <cassert>

struct Address
{
    int x = 0, y = 0;
};

struct LessAddress
{
    bool operator()(const Address &lhs, const Address &rhs) const
    {
        if (lhs.x < rhs.x) return true;
        if (lhs.x > rhs.x) return false;
        return lhs.y < rhs.y;
    }
};

void ProcessMove(char move, Address &santa)
{
    switch (move)
    {
        case '^': --santa.y; break;
        case 'v': santa.y++; break;
        case '<': --santa.x; break;
        case '>': santa.x++; break;
    }
}

unsigned ProcessFile(const std::string &filename, bool doubleSanta)
{
    std::set<const Address, LessAddress> visited;

    Address santa1, santa2;
    visited.insert(santa1);

    std::ifstream f;
    f.open("Input.txt");

    while (!f.eof())
    {
        char move;
        if (f >> move)
        {
            ProcessMove(move, santa1);
            visited.insert(santa1);

            if (doubleSanta && f >> move)
            {
                ProcessMove(move, santa2);
                visited.insert(santa2);
            }
        }
    }

    return visited.size();
}

void _tmain(int argc, _TCHAR* argv[])
{
    auto part1 = ProcessFile("Input.txt", false);
    std::cout << "part one: " << part1 << std::endl;
    assert(part1 == 2565);
    
    auto part2 = ProcessFile("Input.txt", true);
    std::cout << "part two: " << part2 << std::endl;
    assert(part2 == 2639);
}
