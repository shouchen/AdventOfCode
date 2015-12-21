// RolePlayingGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

struct State
{
    int hitPoints, damage, armor;
};

struct Item
{
    const char *name;
    unsigned cost, damage, armor;
};

Item weaponList[] = { { "Dagger", 8, 4, 0 },      { "Shortsword", 10, 5, 0 }, { "Warhammer", 25, 6, 0 },  { "Longsword", 40, 7, 0 },  { "Greataxe", 74, 8, 0 } };
Item armorList[] =  { { "Leather", 13, 0, 1 },    { "Chainmail", 31, 0, 2 },  { "Splintmail", 53, 0, 3 }, { "Bandedmail", 75, 0, 4 }, { "Platemail", 102, 0, 5 } };
Item ringList[] =   { { "Damage +1", 25, 1, 0 },  { "Damage +2", 50, 2, 0 },  { "Damage +3", 100, 3, 0 },
                      { "Defense +1", 20, 0, 1 }, { "Defense +2", 40, 0, 2 }, { "Defense +3", 80, 0, 3 } };

// return true if player wins
bool DoesPlayerWin(State player, State enemy)
{
    for (;;)
    {
        // Player's Turn
        enemy.hitPoints -= std::max(player.damage - enemy.armor, 1);
        if (enemy.hitPoints <= 0) return true;

        // Enemy's Turn
        player.hitPoints -= std::max(enemy.damage - player.armor, 1);
        if (player.hitPoints <= 0) return false;
    }
}

State enemy{ 103, 9, 2 };
auto minCost = UINT_MAX;
auto maxCost = 0U;

void Step4(State player, unsigned cost, const Item *ring1 = nullptr, const Item *ring2 = nullptr)
{
    if (ring1)
    {
        cost += ring1->cost;
        player.damage += ring1->damage;
        player.armor += ring1->armor;
    }

    if (ring2)
    {
        cost += ring2->cost;
        player.damage += ring2->damage;
        player.armor += ring2->armor;
    }

    if (DoesPlayerWin(player, enemy))
        minCost = min(minCost, cost);
    else
        maxCost = max(maxCost, cost);
}

void Step3(State player, unsigned cost, const Item *armor = nullptr)
{
    if (armor)
    {
        cost += armor->cost;
        player.damage += armor->damage;
        player.armor += armor->armor;
    }

    // Try with no rings
    Step4(player, cost);

    // Try with one ring
    for (auto ring : ringList)
        Step4(player, cost, &ring);

    // Try with two rings
    for (int ring1 = 0; ring1 < (sizeof(ringList) / sizeof(ringList[0])); ring1++)
        for (int ring2 = ring1 + 1; ring2 < (sizeof(ringList) / sizeof(ringList[0])); ring2++)
            Step4(player, cost, &ringList[ring1], &ringList[ring2]);
}

void Step2(State player, const Item *weapon)
{
    auto cost = weapon->cost;
    player.damage += weapon->damage;
    player.armor += weapon->armor;

    // Try with no armor first
    Step3(player, cost);

    // Now try with each possible armor
    for (auto armor : armorList)
        Step3(player, cost, &armor);
}

void Step1(State player)
{
    for (auto weapon : weaponList)
        Step2(player, &weapon);
}

void _tmain(int argc, _TCHAR *argv[])
{
    State player{ 100, 0, 0 };

    Step1(player);

    cout << "part one: " << minCost << endl;
    cout << "part two: " << maxCost << endl;

    assert(minCost == 121);
    assert(maxCost == 201);
}
