// Wizard2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cassert>

using namespace std;

enum class Spell { MagicMissile, Drain, Shield, Poison, Recharge };

int bossdam, best, hardmode;

bool Round(int bosshp, int playerhp, int mana, int cost, int s_shield, int s_poison, int s_recharge, Spell spell)
{
    int armor;

    // player turn action
    switch (spell)
    {
    case Spell::MagicMissile:
        bosshp -= 4;
        mana -= 53;
        cost += 53;
        break;
    case Spell::Drain:
        bosshp -= 2;
        playerhp += 2;
        mana -= 73;
        cost += 73;
        break;
    case Spell::Shield:
        if (s_shield > 0) return false;
        s_shield = 6;
        mana -= 113;
        cost += 113;
        break;
    case Spell::Poison:
        if (s_poison > 0) return false;
        s_poison = 6;
        mana -= 173;
        cost += 173;
        break;
    case Spell::Recharge:
        if (s_recharge > 0) return false;
        s_recharge = 5;
        mana -= 229;
        cost += 229;
        break;
    }

    if (bosshp <= 0)
    {
        if (cost < best) best = cost;
        return true;
    }

    // boss turn
    if (s_recharge > 0)
    {
        mana += 101;
        s_recharge--;
    }
    if (s_poison > 0)
    {
        bosshp -= 3;
        s_poison--;
    }
    if (s_shield > 0)
    {
        s_shield--;
        armor = 7;
    }
    else
        armor = 0;

    if (bosshp > 0)
    {
        if ((bossdam - armor) < 1)
            playerhp--;
        else
            playerhp -= (bossdam - armor);
    }
    else
    {
        if (cost < best) best = cost;
        return true;
    }

    if (hardmode) playerhp--;
    if (playerhp <= 0) return false;

    // player turn begin
    if (s_shield > 0) s_shield--;
    if (s_recharge > 0)
    {
        mana += 101;
        s_recharge--;
    }
    if (s_poison > 0)
    {
        bosshp -= 3;
        s_poison--;
    }
    if (bosshp <= 0)
    {
        if (cost < best) best = cost;
        return true;
    }

    if (mana >= 53) Round(bosshp, playerhp, mana, cost, s_shield, s_poison, s_recharge, Spell::MagicMissile);
    if (mana >= 73) Round(bosshp, playerhp, mana, cost, s_shield, s_poison, s_recharge, Spell::Drain);
    if (mana >= 113) Round(bosshp, playerhp, mana, cost, s_shield, s_poison, s_recharge, Spell::Shield);
    if (mana >= 173) Round(bosshp, playerhp, mana, cost, s_shield, s_poison, s_recharge, Spell::Poison);
    if (mana >= 229) Round(bosshp, playerhp, mana, cost, s_shield, s_poison, s_recharge, Spell::Recharge);

    return false;
}

int Solve(int bosshp, int bossdam, int playerhp, int mana, bool hardmode)
{
    ::bossdam = bossdam;
    ::hardmode = hardmode;
    ::best = INT_MAX;

    if (hardmode) playerhp--;

    Round(bosshp, playerhp, mana, 0, 0, 0, 0, Spell::MagicMissile);
    Round(bosshp, playerhp, mana, 0, 0, 0, 0, Spell::Drain);
    Round(bosshp, playerhp, mana, 0, 0, 0, 0, Spell::Shield);
    Round(bosshp, playerhp, mana, 0, 0, 0, 0, Spell::Poison);
    Round(bosshp, playerhp, mana, 0, 0, 0, 0, Spell::Recharge);

    return ::best;
}

void main(int argc, char *argv[])
{
    auto start = chrono::system_clock::now();
    auto answer = Solve(58, 9, 50, 500, false);
    auto end = chrono::system_clock::now();

    chrono::duration<double> seconds = end - start;
    cout << "part one: " << answer << " found in " << seconds.count() << " seconds" << endl;
    assert(answer == 1269);

    start = chrono::system_clock::now();
    answer = Solve(58, 9, 50, 500, true);
    end = chrono::system_clock::now();

    seconds = end - start;
    cout << "part two: " << answer << " found in " << seconds.count() << " seconds" << endl;
    assert(answer == 1309);
}
