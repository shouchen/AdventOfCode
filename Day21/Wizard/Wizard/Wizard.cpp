// Wizard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <queue>
#include <cassert>

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

    bool DoMagicMissile()
    {
        if (player.mana < 53) return false;
        player.mana -= 53;
        playerHasSpent += 53;

        DoEffects();
        enemy.hits -= 4;
        return true;
    }

    bool DoDrain()
    {
        if (player.mana < 73) return false;
        player.mana -= 73;
        playerHasSpent += 73;

        DoEffects();
        enemy.hits -= 2;
        player.hits += 2;
        return true;
    }

    bool DoShield()
    {
        if (player.mana < 113 || shieldTimer) return false;
        player.mana -= 113;
        playerHasSpent += 113;

        DoEffects();
        player.armor += 7;
        shieldTimer = 6;
        return true;
    }

    bool DoPoison()
    {
        if (player.mana < 173 || poisonTimer) return false;
        player.mana -= 173;
        playerHasSpent += 173;

        DoEffects();
        player.damage += 3;
        poisonTimer = 6;
        return true;
    }

    bool DoRecharge()
    {
        if (player.mana < 229 || rechargeTimer) return false;
        player.mana -= 229;
        playerHasSpent += 229;

        DoEffects();
        rechargeTimer = 5;
        return true;
    }

    void DoEnemyTurn()
    {
        DoEffects();
        player.hits -= max(enemy.damage - player.armor, 1);
    }

    bool PlayerHasWon() const
    {
        return enemy.hits <= 0;
    }

    bool EnemyHasWon() const
    {
        return player.hits <= 0;
    }

    unsigned PlayerHasSpent() const
    {
        return playerHasSpent;
    }

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
    bool playerHasWon = false, playerHasLost = false;
    unsigned playerHasSpent = 0;

    friend void DoTest1();
    friend void DoTest2();
};

void DoTest1()
{
    Game game(State{ 10, 0, 0, 250 }, State{ 13, 8, 0, 0 });

    assert(game.player.hits == 10 && game.player.armor == 0 && game.player.mana == 250);
    assert(game.enemy.hits == 13);
    game.DoPoison();

    assert(game.player.hits == 10 && game.player.armor == 0 && game.player.mana == 77);
    assert(game.enemy.hits == 13);
    game.DoEnemyTurn();

    assert(game.player.hits == 2 && game.player.armor == 0 && game.player.mana == 77);
    assert(game.enemy.hits == 10);
    game.DoMagicMissile();

    assert(game.player.hits == 2 && game.player.armor == 0 && game.player.mana == 24);
    assert(game.enemy.hits == 3);
    game.DoEnemyTurn();

    assert(game.PlayerHasWon());
}

void DoTest2()
{
    Game game(State{ 10, 0, 0, 250 }, State{ 14, 8, 0, 0 });

    assert(game.player.hits == 10 && game.player.armor == 0 && game.player.mana == 250);
    assert(game.enemy.hits == 14);
    game.DoRecharge();

    assert(game.player.hits == 10 && game.player.armor == 0 && game.player.mana == 21);
    assert(game.enemy.hits == 14);
    game.DoEnemyTurn();

    assert(game.player.hits == 2 && game.player.armor == 0 && game.player.mana == 122);
    assert(game.enemy.hits == 14);
    game.DoShield();

    assert(game.player.hits == 2 && game.player.armor == 7 && game.player.mana == 110);
    assert(game.enemy.hits == 14);
    game.DoEnemyTurn();

    assert(game.player.hits == 1 && game.player.armor == 7 && game.player.mana == 211);
    assert(game.enemy.hits == 14);
    game.DoDrain();

    assert(game.player.hits == 3 && game.player.armor == 7 && game.player.mana == 239);
    assert(game.enemy.hits == 12);
    game.DoEnemyTurn();

    assert(game.player.hits == 2 && game.player.armor == 7 && game.player.mana == 340);
    assert(game.enemy.hits == 12);
    game.DoPoison();

    assert(game.player.hits == 2 && game.player.armor == 7 && game.player.mana == 167);
    assert(game.enemy.hits == 12);
    game.DoEnemyTurn();

    assert(game.player.hits == 1 && game.player.armor == 7 && game.player.mana == 167);
    assert(game.enemy.hits == 9);
    game.DoMagicMissile();

    assert(game.player.hits == 1 && game.player.armor == 0 && game.player.mana == 114);
    assert(game.enemy.hits == 2);
    game.DoEnemyTurn();

    assert(game.PlayerHasWon());
}

struct MyComparison
{
    bool operator()(const Game &lhs, const Game &rhs) const
    {
        return lhs.PlayerHasSpent() > rhs.PlayerHasSpent();
    }
};

void _tmain(int argc, _TCHAR *argv[])
{
    DoTest1();
    DoTest2();

    priority_queue<Game, vector<Game>, MyComparison> queue;
    unsigned leastCostToWin = UINT_MAX;

    // Put initial state into priority queue (based on cost so far)
    queue.push(Game(State{ 10, 0, 0, 250 }, State{ 14, 8, 0, 0 }));

    // Pop the cheapest path so far, expand all allowed paths. For wins, check for record
    // and discard if not. For losses, just discard. If cost is already more than record,
    // just discard. If still in play, re-insert node to the priority queue.

    while (!queue.empty())
    {
        cout << queue.size() << endl;

        Game game = queue.top();
        queue.pop();

        // See if terminal case
        if (game.PlayerHasWon())
        {
            leastCostToWin = min(game.PlayerHasSpent(), leastCostToWin);
        }
        else if (!game.EnemyHasWon() && game.PlayerHasSpent() < leastCostToWin)
        {
            // Expand all games and re-insert
            auto nextGame = game;
            if (nextGame.DoMagicMissile())
            {
                if (!nextGame.PlayerHasWon())
                    nextGame.DoEnemyTurn();

                queue.push(nextGame);
            }

            nextGame = game;
            if (nextGame.DoDrain())
            {
                if (!nextGame.PlayerHasWon())
                    nextGame.DoEnemyTurn();

                queue.push(nextGame);
            }

            nextGame = game;
            if (nextGame.DoShield())
            {
                if (!nextGame.PlayerHasWon())
                    nextGame.DoEnemyTurn();

                queue.push(nextGame);
            }

            nextGame = game;
            if (nextGame.DoPoison())
            {
                if (!nextGame.PlayerHasWon())
                    nextGame.DoEnemyTurn();

                queue.push(nextGame);
            }

            nextGame = game;
            if (nextGame.DoRecharge())
            {
                if (!nextGame.PlayerHasWon())
                    nextGame.DoEnemyTurn();

                queue.push(nextGame);
            }
        }
    }

    assert(leastCostToWin > 641);
    cout << "part one: " << leastCostToWin << endl;
}
