// RolePlayingGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <cassert>

struct Item
{
    const char *name;
    unsigned cost, damage, armor;
};

struct State
{
    State &operator+=(const Item &item)
    {
        damage += item.damage;
        armor += item.armor;
        return *this;
    }

    int hitPoints, damage, armor;
};

Item weaponList[] = { { "Dagger", 8, 4, 0 },      { "Shortsword", 10, 5, 0 }, { "Warhammer", 25, 6, 0 },  { "Longsword", 40, 7, 0 },  { "Greataxe", 74, 8, 0 } };
Item armorList[] =  { { "Leather", 13, 0, 1 },    { "Chainmail", 31, 0, 2 },  { "Splintmail", 53, 0, 3 }, { "Bandedmail", 75, 0, 4 }, { "Platemail", 102, 0, 5 } };
Item ringList[] =   { { "Damage +1", 25, 1, 0 },  { "Damage +2", 50, 2, 0 },  { "Damage +3", 100, 3, 0 },
                      { "Defense +1", 20, 0, 1 }, { "Defense +2", 40, 0, 2 }, { "Defense +3", 80, 0, 3 } };

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
auto minCostToWin = UINT_MAX;
auto maxCostToLose = 0U;

static void DoForRings(State player, unsigned cost, const Item *ring1 = nullptr, const Item *ring2 = nullptr)
{
    if (ring1)
    {
        cost += ring1->cost;
        player += *ring1;
    }

    if (ring2)
    {
        cost += ring2->cost;
        player += *ring2;
    }

    if (DoesPlayerWin(player, enemy))
        minCostToWin = std::min(minCostToWin, cost);
    else
        maxCostToLose = std::max(maxCostToLose, cost);
}

static void DoForArmor(State player, unsigned cost, const Item *armor = nullptr)
{
    if (armor)
    {
        cost += armor->cost;
        player += *armor;
    }

    // Try with no rings
    DoForRings(player, cost);

    // Try with one ring
    for (auto ring : ringList)
        DoForRings(player, cost, &ring);

    // Try with two rings
    for (auto ring1 = 0; ring1 < (sizeof(ringList) / sizeof(ringList[0])); ring1++)
        for (auto ring2 = ring1 + 1; ring2 < (sizeof(ringList) / sizeof(ringList[0])); ring2++)
            DoForRings(player, cost, &ringList[ring1], &ringList[ring2]);
}

static void DoForWeapon(State player, const Item *weapon)
{
    auto cost = weapon->cost;
    player += *weapon;

    // Try with no armor first
    DoForArmor(player, cost);

    // Now try with each possible armor
    for (auto armor : armorList)
        DoForArmor(player, cost, &armor);
}

static void Solve(const State &player)
{
    for (auto weapon : weaponList)
        DoForWeapon(player, &weapon);
}

void _tmain(int argc, _TCHAR *argv[])
{
    Solve(State{ 100, 0, 0 });

    std::cout << "part one: " << minCostToWin << std::endl;
    assert(maxCostToLose == 201);

    std::cout << "part two: " << maxCostToLose << std::endl;
    assert(maxCostToLose == 201);
}
