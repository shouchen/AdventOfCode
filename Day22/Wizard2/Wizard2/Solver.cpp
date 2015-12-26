#include "stdafx.h"
#include "Solver.h"
#include <iostream>
#include <algorithm>

using namespace std;

int Solver::s_bossDamage = 9;
int Solver::s_lowestCost = INT_MAX;
bool Solver::s_hardMode = false;
const int Solver::s_spellCost[5] = { 53, 73, 113, 173, 229 };

int Solver::Solve(int playerhp, int mana, int bosshp, int bossdam, bool hardMode)
{
    Solver::s_bossDamage = bossdam;
    Solver::s_hardMode = hardMode;
    Solver::s_lowestCost = INT_MAX;

    State state{ { hardMode ? (playerhp - 1) : playerhp, mana }, { bosshp }, { 0, 0, 0 }, 0 };

    for (int firstSpell = MagicMissile; firstSpell <= Recharge; firstSpell++)
    {
        cout << ".";
        Round(state, static_cast<Spell>(firstSpell));
    }

    return Solver::s_lowestCost;
}

bool Solver::Round(State state, Spell spell)
{
    if (!PlayerTurnActionFirstPart(state, spell))
        return false;

    if (PlayerTurnActionSecondPart(state, spell))
        return true;

    // Boss's turn
    int armor = 0;

    ApplyEffects(state, spell, &armor);
    if (state.boss.hp <= 0)
    {
        if (state.cost < Solver::s_lowestCost) Solver::s_lowestCost = state.cost;
        return true;
    }

    state.player.hp -= max(Solver::s_bossDamage - armor, 1);
    if (Solver::s_hardMode) state.player.hp--;

    if (state.player.hp <= 0)
        return false;

    // Player's turn begins
    ApplyEffects(state, spell, nullptr);
    if (state.boss.hp <= 0)
    {
        if (state.cost < Solver::s_lowestCost) Solver::s_lowestCost = state.cost;
        return true;
    }

    for (int nextSpell = 0; nextSpell < 5; nextSpell++)
        if (state.player.mana >= s_spellCost[nextSpell])
            Round(state, static_cast<Spell>(nextSpell));

    return false;
}
