// Cookies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>

struct Ingredient
{
    int capacity, durability, flavor, texture, calories;
};

std::vector<Ingredient> ingredients;

int max, exactCalories;

int Recurse(int index, int teaspoons, int capacity, int durability, int flavor, int texture, int calories)
{
    if (index == ingredients.size() - 1)
    {
        capacity += teaspoons * ingredients[index].capacity;
        durability += teaspoons * ingredients[index].durability;
        flavor += teaspoons * ingredients[index].flavor;
        texture += teaspoons * ingredients[index].texture;
        calories += teaspoons * ingredients[index].calories;

        if (capacity > 0 && durability > 0 && flavor > 0 && texture > 0)
        {
            if (!exactCalories || calories == exactCalories)
            {
                auto total = capacity * durability * flavor * texture;
                max = std::max(max, total);
            }
        }

        return max;
    }

    while (teaspoons)
    {
        Recurse(index + 1, teaspoons, capacity, durability, flavor, texture, calories);

        capacity += ingredients[index].capacity;
        durability += ingredients[index].durability;
        flavor += ingredients[index].flavor;
        texture += ingredients[index].texture;
        calories += ingredients[index].calories;

        --teaspoons;
    }

    return max;
}

int Solve(int teaspoons, int exactCalories = 0)
{
    max = 0;
    ::exactCalories = exactCalories;
    return Recurse(0, teaspoons, 0, 0, 0, 0, 0);
}

void ReadInputFile()
{
    std::ifstream f("Input.txt");
    for (std::string line; getline(f, line);)
    {
        auto capacity = atoi(&line[line.find(" capacity ") + 10]);
        auto durability = atoi(&line[line.find(" durability ") + 11]);
        auto flavor = atoi(&line[line.find(" flavor ") + 8]);
        auto texture = atoi(&line[line.find(" texture ") + 9]);
        auto calories = atoi(&line[line.find(" calories ") + 10]);

        ingredients.push_back(Ingredient{ capacity, durability, flavor, texture, calories });
    }

    f.close();
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInputFile();

    assert(Solve(100, 0) == 21367368);
    assert(Solve(100, 500) == 1766400);

    std::cout << "part one: " << Solve(100) << std::endl;
    std::cout << "part two: " << Solve(100, 500) << std::endl;
}
