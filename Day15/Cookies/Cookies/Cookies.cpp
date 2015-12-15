// Cookies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <climits>

struct Ingredient
{
    int capacity, durability, flavor, texture, calories;
};

void DoIt(int exactCalories = 0)
{
    Ingredient sprinkles{ 2, 0, -2, 0, 3 };
    Ingredient butterscotch{ 0, 5, -3, 0, 3 };
    Ingredient chocolate{ 0, 0, 5, -1, 8 };
    Ingredient candy{ 0, -1, 0, 5, 8 };

    int max = 0;

    for (auto s = 0; s <= 100; s++)
    {
        for (auto b = 0; b <= 100 - s; b++)
        {
            for (auto ch = 0; ch <= 100 - s - b; ch++)
            {
                auto c = 100 - s - b - ch;

                auto capacity = s * sprinkles.capacity + b * butterscotch.capacity + ch * chocolate.capacity + c * candy.capacity;
                auto durability = s * sprinkles.durability + b * butterscotch.durability + ch * chocolate.durability + c * candy.durability;
                auto flavor = s * sprinkles.flavor + b * butterscotch.flavor + ch * chocolate.flavor + c * candy.flavor;
                auto texture = s * sprinkles.texture + b * butterscotch.texture + ch * chocolate.texture + c * candy.texture;
                auto calories = s * sprinkles.calories + b * butterscotch.calories + ch * chocolate.calories + c * candy.calories;

                if (capacity < 0) capacity = 0;
                if (durability < 0) durability = 0;
                if (flavor < 0) flavor = 0;
                if (texture < 0) texture = 0;

                if (!exactCalories || calories == exactCalories)
                {
                    auto total = capacity * durability * flavor * texture;
                    max = std::max(max, total);
                }
            }
        }
    }

    std::cout << max << std::endl;
}

void _tmain(int argc, _TCHAR *argv[])
{
    DoIt();
    DoIt(500);
}
