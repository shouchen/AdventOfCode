// Wizard2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

#define HARDMODE true

int bossdam = 9;
int best = INT_MAX;

enum Spell
{
    MagicMissile, Drain, Shield, Poison, Recharge
};

int spellcost[5] = { 53, 73, 113, 173, 229 };

bool round(int bosshp, int playerhp, int mana, int cost, int s_shield, int s_poison, int s_recharge, Spell spell)
{
    int armor;
    // player turn action
    switch (spell)
    {
    case MagicMissile:
        bosshp -= 4;
        break;
    case Drain:
        bosshp -= 2;
        playerhp += 2;
        break;
    case Shield:
        if (s_shield == 0)
            s_shield = 6;
        else
            return false;
        break;
    case Poison:
        if (s_poison == 0)
            s_poison = 6;
        else
            return false;
        break;
    case Recharge:
        if (s_recharge == 0)
            s_recharge = 5;
        else
            return false;
        break;
    }
    mana -= spellcost[spell];
    cost += spellcost[spell];
    if (bosshp <= 0)
    {
        if (cost < best)
            best = cost;
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
        if (cost < best)
            best = cost;
        return true;
    }

    if (HARDMODE) playerhp--;  // hard mode bleed effect
    if (playerhp <= 0)
        return false;


    // player turn begin
    if (s_shield > 0)
        s_shield--;
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
        if (cost < best)
            best = cost;
        return true;
    }
    for (int nextspell = 0; nextspell < 5; nextspell++)
    {
        if (mana >= spellcost[nextspell])
            round(bosshp, playerhp, mana, cost, s_shield, s_poison, s_recharge, static_cast<Spell>(nextspell));
    }
    return false;
}


int main(void)
{
    for (int firstspell = 0; firstspell < 5; firstspell++)
    {
        printf("(%d) ", firstspell);
        int start = HARDMODE ? (50 - 1) : 50;   // -1 starting hp for hard mode
        round(58, start, 500, 0, 0, 0, 0, static_cast<Spell>(firstspell));
    }
    printf("\n Lowest cost: %d \n", best);
    return 0;
}
