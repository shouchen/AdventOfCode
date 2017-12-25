#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

int get_dist_from_origin(int x, int y)
{
    x = abs(x), y = abs(y);
    return unsigned((x > y) ? x : (y - x + 1) / 2 + x);
}

void process_input(std::istream &input, unsigned &distance, unsigned &max_distance)
{
    auto x = 0, y = 0;
    distance = max_distance = 0;
    std::string move;

    while (getline(input, move, ','))
    {
        if (move == "n")       y += 2;
        else if (move == "ne") x++, y++;
        else if (move == "se") x++, y--;
        else if (move == "s")  y -= 2;
        else if (move == "sw") x--, y--;
        else if (move == "nw") x--, y++;
        else assert(false);

        distance = get_dist_from_origin(x, y);
        max_distance = std::max(distance, max_distance);
    }
}

int main()
{
    auto distance = 0U, max_distance = 0U;

    process_input(std::istringstream("ne,ne,ne"), distance, max_distance);
    assert(distance == 3);

    process_input(std::istringstream("ne,ne,sw,sw"), distance, max_distance);
    assert(distance == 0);

    process_input(std::istringstream("ne,ne,s,s"), distance, max_distance);
    assert(distance == 2);
    
    process_input(std::istringstream("se,sw,se,sw,sw"), distance, max_distance);
    assert(distance == 3);

    process_input(std::ifstream("input.txt"), distance, max_distance);

    std::cout << "Part 1: " << distance << std::endl;
    std::cout << "Part 2: " << max_distance << std::endl;

    assert(distance == 682);
    assert(max_distance == 1406);
    return 0;
}
