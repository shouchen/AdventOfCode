// Wizard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <cassert>

using namespace std;

// Compiler ignoring requests to inline, so use this as a poor man's inline. Code needed to be
// highly optimized since this brute force algorithm takes awhile to run. Update! By pruning
// the game tree substantially, these inline hacks are now no longer needed, so they could be
// removed in favor of more readable code.

#define DoEffects() { \
    if (shieldTimer) { shieldTimer--; armor = 7; } \
    if (poisonTimer) { hpBoss -= 3; poisonTimer--; } \
    if (rechargeTimer) { mana += 101; rechargeTimer--; } \
}

#define DoBossDamageAgainstPlayer() { \
    auto diff = s_damageBoss - armor; \
    hpPlayer -= (diff > 1) ? diff : 1; \
}

#define ReturnIfPlayerWonAndUpdateBestCost() { \
    if (hpBoss <= 0) { if (cost < s_lowestCost) s_lowestCost = cost; return; } \
}

#define ApplyHardModePointsAndReturnIfPlayerLost() { \
    if ((hpPlayer -= s_hardModePoints) <= 0) return; \
}

#define ForkNewRoundForEachPossibleSpell() { \
    if (mana >= 53 && cost + 53 < s_lowestCost) { \
        Round(hpBoss - 4, hpPlayer, mana - 53, cost + 53, shieldTimer, poisonTimer, rechargeTimer); \
        if (mana >= 73 && cost + 73 < s_lowestCost) { \
            Round(hpBoss - 2, hpPlayer + 2, mana - 73, cost + 73, shieldTimer, poisonTimer, rechargeTimer); \
            if (mana >= 113 && cost + 113 < s_lowestCost) { \
                if (!shieldTimer) Round(hpBoss, hpPlayer, mana - 113, cost + 113, 6, poisonTimer, rechargeTimer); \
                if (mana >= 173 && cost + 173 < s_lowestCost) { \
                    if (!poisonTimer) Round(hpBoss, hpPlayer, mana - 173, cost + 173, shieldTimer, 6, rechargeTimer); \
                    if (mana >= 229 && cost + 229 < s_lowestCost) { \
                        if (!rechargeTimer) Round(hpBoss, hpPlayer, mana - 229, cost + 229, shieldTimer, poisonTimer, 5); \
                    } \
                } \
            } \
        } \
    } \
}

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
        hpPlayer -= s_hardModePoints;
        if (hpPlayer <= 0) return s_lowestCost;

        // First spell.
        const auto shieldTimer = 0, poisonTimer = 0, rechargeTimer = 0, cost = 0;
        ForkNewRoundForEachPossibleSpell();

        return s_lowestCost;
    }

private:
    static void Round(int hpBoss, int hpPlayer, int mana, int cost, int shieldTimer, int poisonTimer, int rechargeTimer)
    {
        ReturnIfPlayerWonAndUpdateBestCost();

        // Start of Boss's turn--apply efects and see if that killed him.
        int armor = 0;
        DoEffects();
        ReturnIfPlayerWonAndUpdateBestCost();

        // Boss does his turn; check if it killed Player.
        DoBossDamageAgainstPlayer();
        ApplyHardModePointsAndReturnIfPlayerLost();

        // Player's turn starting--first do effects and see if Boss gets killed.
        DoEffects();
        ReturnIfPlayerWonAndUpdateBestCost();

        // Now fork for each possible spell the Player could choose, and try those, etc...
        ForkNewRoundForEachPossibleSpell();
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
