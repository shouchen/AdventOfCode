// Wizard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <cassert>

class Solver
{
public:
    static int Solve(int hpBoss, int damageBoss, int hpPlayer, int mana, bool hardMode)
    {
        // Initialize statics at the start of puzzle solving.
        s_damageBoss = damageBoss;
        s_hardModePoints = hardMode ? 1 : 0;
        s_lowestCost = INT_MAX;

        // Take the first player turn, and recurse through game tree.
        DoPlayerTurn(State { hpBoss, hpPlayer, mana, 0, 0, 0, 0, 0 });
        return s_lowestCost;
    }

private:
    struct State
    {
        int hpBoss, hpPlayer, mana, armor, cost, shieldTimer, poisonTimer, rechargeTimer;
    };

    static void DoPlayerTurn(State &state)
    {
        // Apply any hard mode points and check if Player has lost.
        if ((state.hpPlayer -= s_hardModePoints) <= 0) return;

        // Apply any active efects and see if they killed Boss.
        if (DoEffectsAndCheckIfPlayerWon(state)) return;

        // Cast each possible spell and recurse through resulting game tree.
        if (state.mana < 53 || state.cost + 53 >= s_lowestCost) return;
        DoBossTurn(State{ state.hpBoss - 4, state.hpPlayer, state.mana - 53, 0, state.cost + 53, state.shieldTimer, state.poisonTimer, state.rechargeTimer });

        if (state.mana < 73 || state.cost + 73 >= s_lowestCost) return;
        DoBossTurn(State{ state.hpBoss - 2, state.hpPlayer + 2, state.mana - 73, 0, state.cost + 73, state.shieldTimer, state.poisonTimer, state.rechargeTimer });

        if (state.mana < 113 || state.cost + 113 >= s_lowestCost) return;
        if (!state.shieldTimer)
            DoBossTurn(State{ state.hpBoss, state.hpPlayer, state.mana - 113, 0, state.cost + 113, 6, state.poisonTimer, state.rechargeTimer });

        if (state.mana < 173 || state.cost + 173 >= s_lowestCost) return;
        if (!state.poisonTimer)
            DoBossTurn(State{ state.hpBoss, state.hpPlayer, state.mana - 173, 0, state.cost + 173, state.shieldTimer, 6, state.rechargeTimer });

        if (state.mana < 229 || state.cost + 229 >= s_lowestCost) return;
        if (!state.rechargeTimer)
            DoBossTurn(State{ state.hpBoss, state.hpPlayer, state.mana - 229, 0, state.cost + 229, state.shieldTimer, state.poisonTimer, 5 });
    }

    static void DoBossTurn(State &state)
    {
        // Make sure Boss has not already lost.
        if (CheckIfPlayerWonAndUpdateLowestCost(state)) return;

        // Apply any active efects and see if they killed Boss.
        if (DoEffectsAndCheckIfPlayerWon(state)) return;

        // Now Boss takes his turn (check for Boss win happens at start of Player turn).
        state.hpPlayer -= std::max(s_damageBoss - state.armor, 1);

        // Now Player's turn again.
        DoPlayerTurn(state);
    }

    static bool DoEffectsAndCheckIfPlayerWon(State &state)
    {
        if (state.shieldTimer) { state.shieldTimer--; state.armor = 7; }
        if (state.poisonTimer) { state.hpBoss -= 3; state.poisonTimer--; }
        if (state.rechargeTimer) { state.mana += 101; state.rechargeTimer--; }

        return CheckIfPlayerWonAndUpdateLowestCost(state);
    }

    static bool CheckIfPlayerWonAndUpdateLowestCost(const State &state)
    {
        if (state.hpBoss > 0) return false;

        s_lowestCost = std::min(state.cost, s_lowestCost);
        return true;
    }

    static int s_damageBoss, s_hardModePoints, s_lowestCost;
};

int Solver::s_damageBoss, Solver::s_lowestCost, Solver::s_hardModePoints;


void main(int argc, char *argv[])
{
    const auto hpPlayer = 50, mana = 500, hpBoss = 58, damageBoss = 9;

    auto answer = Solver::Solve(hpBoss, damageBoss, hpPlayer, mana, false);
    std::cout << "part one: " << answer << std::endl;
    assert(answer == 1269);

    answer = Solver::Solve(hpBoss, damageBoss, hpPlayer, mana, true);
    std::cout << "part two: " << answer << std::endl;
    assert(answer == 1309);
}
