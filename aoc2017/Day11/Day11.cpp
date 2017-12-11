#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

int get_dist(int x, int y)
{
    x = abs(x);
    y = abs(y);

    return (x > y) ? x : (y - x + 1) / 2 + x;
}

void do_moves(const std::string &moves, int &distance, int &max_distance)
{
    int x = 0, y = 0;
    max_distance = INT_MIN;

    int curr = 0;
    while (curr < moves.length())
    {
        if (moves[curr] == 'n')
        {
            if (curr == moves.length() - 1 || moves[curr + 1] == ',')
            {
                y += 2;
                curr += 2;
            }
            else if (moves[curr + 1] == 'e')
            {
                x++, y++;
                curr += 3;
            }
            else if (moves[curr + 1] == 'w')
            {
                x--, y++;
                curr += 3;
            }
            else
                assert(false);
        }
        else if (moves[curr] == 's')
        {
            if (curr == moves.length() - 1 || moves[curr + 1] == ',')
            {
                y -= 2;
                curr += 2;
            }
            else if (moves[curr + 1] == 'e')
            {
                x++, y--;
                curr += 3;
            }
            else if (moves[curr + 1] == 'w')
            {
                x--, y--;
                curr += 3;
            }
            else
                assert(false);
        }
        else
        {
            assert(false);
        }

        distance = get_dist(x, y);
        if (distance > max_distance)
            max_distance = distance;
    }
}

int main()
{
    int distance = 0, max_distance = 0;

    do_moves("ne,ne,ne", distance, max_distance);
    assert(distance == 3);

    do_moves("ne,ne,sw,sw", distance, max_distance);
    assert(distance == 0);

    do_moves("ne,ne,s,s", distance, max_distance);
    assert(distance == 2);
    
    do_moves("se,sw,se,sw,sw", distance, max_distance);
    assert(distance == 3);

    std::string input;
    std::ifstream f("input.txt");
    if (f >> input)
    {
        do_moves(input, distance, max_distance);

        auto part1 = distance;
        std::cout << "Part 1: " << part1 << std::endl;
        assert(part1 == 682);

        auto part2 = max_distance;
        std::cout << "Part 2: " << part2 << std::endl;
        assert(part2 == 1406);
    }

    return 0;
}
