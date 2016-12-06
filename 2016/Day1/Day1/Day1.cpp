// Day1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <set>
#include <math.h>

auto location = std::pair<int, int>(0, 0);
auto direction = std::pair<int, int>(0, -1);

std::set<std::pair<int, int>> visited;
std::pair<int, int> *first_revisited;

void apply_turn(char turn)
{
    assert(turn == 'L' || turn == 'R');

    std::swap(direction.first, direction.second);
    if (turn == 'R') direction.first = -direction.first; else direction.second = -direction.second;
}

void apply_distance(int distance)
{
    for (int i = 0; i < distance; i++)
    {
        location.first += direction.first;
        location.second += direction.second;

        if (!first_revisited && visited.find(location) != visited.end())
            first_revisited = new std::pair<int, int>(location);

        visited.insert(location);
	}
}

int distance_in_blocks(const std::pair<int, int> &place)
{
    return abs(place.first) + abs(place.second);
}

int main()
{
    visited.insert(std::pair<int, int>(location));

    std::ifstream f;
    f.open("input.txt");

    char turn, comma;
    int distance = 0;

    while (f >> turn >> distance)
    {
        apply_turn(turn);
        apply_distance(distance);
        f >> comma;
    }

    auto part_one = distance_in_blocks(location);
    auto part_two = first_revisited ? distance_in_blocks(*first_revisited) : -1;

    std::cout << "Part One: " << part_one << std::endl;
    std::cout << "Part Two: " << part_two << std::endl;

    assert(part_one == 299);
    assert(part_two == 181);
    return 0;
}
