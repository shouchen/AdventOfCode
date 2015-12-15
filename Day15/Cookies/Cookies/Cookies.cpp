// Cookies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

struct Ingredient
{
    int capacity, durability, flavor, texture, calories;
};

std::vector<Ingredient> ingredient;

int max;

int Recurse(int index, int teaspoons, int exactCalories, int capacity, int durability, int flavor, int texture, int calories)
{
    if (index == ingredient.size() - 1)
    {
        capacity += teaspoons * ingredient[index].capacity;
        durability += teaspoons * ingredient[index].durability;
        flavor += teaspoons * ingredient[index].flavor;
        texture += teaspoons * ingredient[index].texture;
        calories += teaspoons * ingredient[index].calories;

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
        Recurse(index + 1, teaspoons, exactCalories, capacity, durability, flavor, texture, calories);

        capacity += ingredient[index].capacity;
        durability += ingredient[index].durability;
        flavor += ingredient[index].flavor;
        texture += ingredient[index].texture;
        calories += ingredient[index].calories;

        --teaspoons;
    }

    return max;
}

int Solve(int teaspoons, int exactCalories = 0)
{
    max = 0;
    return Recurse(0, teaspoons, exactCalories, 0, 0, 0, 0, 0);
}

void _tmain(int argc, _TCHAR *argv[])
{
    ingredient.push_back(Ingredient { 2, 0, -2, 0, 3 });
    ingredient.push_back(Ingredient { 0, 5, -3, 0, 3 });
    ingredient.push_back(Ingredient { 0, 0, 5, -1, 8 });
    ingredient.push_back(Ingredient { 0, -1, 0, 5, 8 });

    assert(Solve(100, 0) == 21367368);
    assert(Solve(100, 500) == 1766400);

    std::cout << "part one: " << Solve(100) << std::endl;
    std::cout << "part two: " << Solve(100, 500) << std::endl;
}
