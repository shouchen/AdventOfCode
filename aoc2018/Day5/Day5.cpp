#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <cassert>

bool apply_rule(std::list<char> &s)
{
    bool modified = false;

    std::list<char>::iterator prev = s.end();
    for (std::list<char>::iterator curr = s.begin(); curr != s.end(); curr++)
    {
        if (prev != s.end())
        {
            if (*prev != *curr && toupper(*prev) == toupper(*curr))
            {
                s.erase(prev);
                s.erase(curr);
                return true;
            }
        }

        prev = curr;
    }

    return modified;
}

//dabAcCaCBAcCcaDA  The first 'cC' is removed.
//dabAaCBAcCcaDA    This creates 'Aa', which is removed.
//dabCBAcCcaDA      Either 'cC' or 'Cc' are removed(the result is the same).
//dabCBAcaDA        No further actions can be taken.

std::set<char> letter; // save for part2

unsigned do_part1()
{
    std::ifstream file("input.txt");

    std::list<char> s;
    char c;
    while (file >> c)
    {
        s.push_back(c);
        letter.insert(tolower(c));
    }

    while (apply_rule(s));
    return static_cast<unsigned>(s.size());
}

unsigned do_work(char omit)
{
    std::ifstream file("input.txt");

    std::list<char> s;
    char c;
    while (file >> c)
    {
        if (tolower(c) != omit)
            s.push_back(c);
    }

    while (apply_rule(s));
    return static_cast<unsigned>(s.size());
}

unsigned do_part2()
{
    unsigned smallestsize = 1000000;
    unsigned smallestletter = ' ';

    for (auto x : letter)
    {
        auto len = do_work(x);
        if (len < smallestsize)
        {
            smallestsize = len;
            smallestletter = x;
            std::cout << x << ' ' << len << std::endl;
        }
    }

    return smallestsize;
}


int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 11476);
    assert(part2 == 5446);
    return 0;
}
