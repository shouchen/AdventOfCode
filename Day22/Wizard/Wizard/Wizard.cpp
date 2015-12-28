// Wizard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <cassert>

using namespace std;

class Solver
{
public:
    static int Solve(int hpBoss, int damageBoss, int hpPlayer, int mana, bool hardMode)
    {
        // Save parameters that don't need to be on the stack (are constant the whole puzzle).
        s_damageBoss = damageBoss;
        s_hardModePoints = hardMode ? 1 : 0;
        s_lowestCost = INT_MAX;

        // Player's first turn--deduct hit points if hard mode, and check if it killed player.
        if (ApplyHardModePointsAndCheckIfPlayerLost(hpPlayer)) return s_lowestCost;

        // First spell.
        ForkNewRoundForEachPossibleSpell(hpBoss, hpPlayer, mana, 0, 0, 0, 0);

        return s_lowestCost;
    }

private:
    inline static void DoEffects(int &shieldTimer, int &poisonTimer, int &rechargeTimer, int &armor, int &hpBoss, int &mana)
    {
        if (shieldTimer) { shieldTimer--; armor = 7; } \
        if (poisonTimer) { hpBoss -= 3; poisonTimer--; } \
        if (rechargeTimer) { mana += 101; rechargeTimer--; } \
    }

    inline static bool CheckIfPlayerWonAndUpdateLowestCost(int hpBoss, int cost)
    {
        if (hpBoss > 0) return false;

        if (cost < s_lowestCost) s_lowestCost = cost; 
        return true;
    }

    inline static void DoBossDamageAgainstPlayer(int armor, int &hpPlayer)
    {
        auto diff = s_damageBoss - armor;
        hpPlayer -= (diff > 1) ? diff : 1;
    }

    inline static bool ApplyHardModePointsAndCheckIfPlayerLost(int &hpPlayer)
    {
        return ((hpPlayer -= s_hardModePoints) <= 0);
    }

    inline static void ForkNewRoundForEachPossibleSpell(int hpBoss, int hpPlayer, int mana, int cost, int shieldTimer, int poisonTimer, int rechargeTimer)
    {
        if (mana < 53 || cost + 53 >= s_lowestCost) return;
        Round(hpBoss - 4, hpPlayer, mana - 53, cost + 53, shieldTimer, poisonTimer, rechargeTimer);

        if (mana < 73 || cost + 73 >= s_lowestCost) return;
        Round(hpBoss - 2, hpPlayer + 2, mana - 73, cost + 73, shieldTimer, poisonTimer, rechargeTimer);

        if (mana < 113 || cost + 113 >= s_lowestCost) return;
        if (!shieldTimer) Round(hpBoss, hpPlayer, mana - 113, cost + 113, 6, poisonTimer, rechargeTimer);

        if (mana < 173 || cost + 173 >= s_lowestCost) return;
        if (!poisonTimer) Round(hpBoss, hpPlayer, mana - 173, cost + 173, shieldTimer, 6, rechargeTimer);

        if (mana < 229 || cost + 229 >= s_lowestCost) return;
        if (!rechargeTimer) Round(hpBoss, hpPlayer, mana - 229, cost + 229, shieldTimer, poisonTimer, 5);
    }

    static void Round(int hpBoss, int hpPlayer, int mana, int cost, int shieldTimer, int poisonTimer, int rechargeTimer)
    {
        if (CheckIfPlayerWonAndUpdateLowestCost(hpBoss, cost)) return;

        // Start of Boss's turn--apply efects and see if that killed him.
        int armor = 0;
        DoEffects(shieldTimer, poisonTimer, rechargeTimer, armor, hpBoss, mana);
        if (CheckIfPlayerWonAndUpdateLowestCost(hpBoss, cost)) return;

        // Boss does his turn; check if it killed Player.
        DoBossDamageAgainstPlayer(armor, hpPlayer);
        if (ApplyHardModePointsAndCheckIfPlayerLost(hpPlayer)) return;

        // Player's turn starting--first do effects and see if Boss gets killed.
        DoEffects(shieldTimer, poisonTimer, rechargeTimer, armor, hpBoss, mana);
        if (CheckIfPlayerWonAndUpdateLowestCost(hpBoss, cost)) return;

        // Now fork for each possible spell the Player could choose, and try those, etc...
        ForkNewRoundForEachPossibleSpell(hpBoss, hpPlayer, mana, cost, shieldTimer, poisonTimer, rechargeTimer);
    }

    static int s_damageBoss, s_hardModePoints, s_lowestCost;
};

int Solver::s_damageBoss, Solver::s_lowestCost, Solver::s_hardModePoints;


void main(int argc, char *argv[])
{
    const int hpPlayer = 50, mana = 500, hpBoss = 58, damageBoss = 9;

    auto answer = Solver::Solve(hpBoss, damageBoss, hpPlayer, mana, false);
    cout << "part one: " << answer << endl;
    assert(answer == 1269);

    answer = Solver::Solve(hpBoss, damageBoss, hpPlayer, mana, true);
    cout << "part two: " << answer << endl;
    assert(answer == 1309);
}
