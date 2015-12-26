// Wizard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <queue>
#include <cassert>
#include <string>

using namespace std;

struct State
{
    int hits, damage, armor, mana;
};

class Game
{
public:
    Game(State player, State enemy) :
        player(player), enemy(enemy)
    {
    }

    void DoMagicMissile()
    {
        debug += "m";
        player.mana -= 53;
        playerHasSpent += 53;

        DoEffects();
        enemy.hits -= 4;
    }

    void DoDrain()
    {
        debug += "d";
        player.mana -= 73;
        playerHasSpent += 73;

        DoEffects();
        enemy.hits -= 2;
        player.hits += 2;
    }

    void DoShield()
    {
        debug += "s";
        player.mana -= 113;
        playerHasSpent += 113;

        DoEffects();
        player.armor += 7;
        shieldTimer = 6;
    }

    void DoPoison()
    {
        debug += "p";
        player.mana -= 173;
        playerHasSpent += 173;

        DoEffects();
        player.damage += 3;
        poisonTimer = 6;
    }

    void DoRecharge()
    {
        debug += "r";
        player.mana -= 229;
        playerHasSpent += 229;

        DoEffects();
        rechargeTimer = 5;
    }

    void DoEnemyTurn()
    {
        DoEffects();

        if (!PlayerHasWon())
            player.hits -= max(enemy.damage - player.armor, 1);
    }

    bool CanDoMagicMissile(unsigned leastCostToWin) { return player.mana >= 53 && (playerHasSpent + 53 < leastCostToWin); }
    bool CanDoDrain(unsigned leastCostToWin) { return player.mana >= 73 && (playerHasSpent + 73 < leastCostToWin); }
    bool CanDoShield(unsigned leastCostToWin) { return player.mana >= 113 && !shieldTimer && (playerHasSpent + 113 < leastCostToWin); }
    bool CanDoPoison(unsigned leastCostToWin) { return player.mana >= 173 && !poisonTimer && (playerHasSpent + 173 < leastCostToWin); }
    bool CanDoRecharge(unsigned leastCostToWin) { return player.mana >= 229 && !rechargeTimer && (playerHasSpent + 229 < leastCostToWin); }

    bool PlayerHasWon() const { return enemy.hits <= 0; }
    bool PlayerHasLost() const { return player.hits <= 0; }
    unsigned PlayerHasSpent() const { return playerHasSpent; }

private:
    void DoEffects()
    {
        if (shieldTimer)
        {
            if (--shieldTimer == 0)
                player.armor -= 7;
        }

        if (poisonTimer)
        {
            enemy.hits -= max(player.damage - enemy.armor, 1);
            if (--poisonTimer == 0)
                player.damage -= 3;
        }

        if (rechargeTimer)
        {
            player.mana += 101;
            --rechargeTimer;
        }
    }

    State player, enemy;
    int shieldTimer = 0, poisonTimer = 0, rechargeTimer = 0;
    unsigned playerHasSpent = 0;

public:
    string debug;

    friend void DoTest1();
    friend void DoTest2();
    friend struct MyComparison;
};

#include "Tests.h"
struct MyComparison
{
    bool operator()(const Game &lhs, const Game &rhs) const
    {
        if (lhs.player.hits - lhs.enemy.hits < rhs.player.hits - rhs.enemy.hits) return true;
        if (lhs.player.hits - lhs.enemy.hits > rhs.player.hits - rhs.enemy.hits) return false;

        if (lhs.PlayerHasSpent() > rhs.PlayerHasSpent()) return true;
        if (lhs.PlayerHasSpent() < rhs.PlayerHasSpent()) return false;

        return false;
    }
};

priority_queue<Game, vector<Game>, MyComparison> queue;
unsigned leastCostToWin = UINT_MAX;

void DoPostMove(Game &nextGame)
{
    if (nextGame.PlayerHasSpent() < leastCostToWin)
    {
        if (nextGame.PlayerHasWon())
            leastCostToWin = nextGame.PlayerHasSpent();
        else
        {
            assert(!nextGame.PlayerHasLost());

            nextGame.DoEnemyTurn();

            if (nextGame.PlayerHasWon())
                leastCostToWin = nextGame.PlayerHasSpent();
            else if (!nextGame.PlayerHasLost())
                ::queue.push(nextGame);
        }
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    DoTest1();
    DoTest2();

//    Game game(State{ 10, 0, 0, 250 }, State{ 13, 8, 0, 0 });
//  Game game(State{ 10, 0, 0, 250 }, State{ 14, 8, 0, 0 });
    Game game(State{ 50, 0, 0, 500 }, State{ 58, 9, 0, 0 });
    ::queue.push(game);

    while (!::queue.empty())
    {
        Game game = ::queue.top();
        ::queue.pop();

        if (game.PlayerHasSpent() >= leastCostToWin) continue;

        //cout << game.PlayerHasSpent() << " " << game.debug << endl;

        // Try all possible moves
        if (game.CanDoPoison(leastCostToWin))
        {
            auto nextGame = game;
            nextGame.DoPoison();
            DoPostMove(nextGame);
        }

        if (game.CanDoShield(leastCostToWin))
        {
            auto nextGame = game;
            nextGame.DoShield();
            DoPostMove(nextGame);
        }

        if (game.CanDoRecharge(leastCostToWin))
        {
            auto nextGame = game;
            nextGame.DoRecharge();
            DoPostMove(nextGame);
        }

        if (game.CanDoMagicMissile(leastCostToWin))
        {
            auto nextGame = game;
            nextGame.DoMagicMissile();
            DoPostMove(nextGame);
        }

        if (game.CanDoDrain(leastCostToWin))
        {
            auto nextGame = game;
            nextGame.DoDrain();
            DoPostMove(nextGame);
        }
    }

//    assert(leastCostToWin > 641 && leastCostToWin != 1415);
    cout << "part one: " << leastCostToWin << endl;
}
