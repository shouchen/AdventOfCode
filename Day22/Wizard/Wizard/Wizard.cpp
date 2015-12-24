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
        player.mana -= 53;
        playerHasSpent += 53;

        DoEffects();
        enemy.hits -= 4;
    }

    bool CanDoMagicMissile()
    {
        return player.mana >= 53;
    }

    void DoDrain()
    {
        player.mana -= 73;
        playerHasSpent += 73;

        DoEffects();
        enemy.hits -= 2;
        player.hits += 2;
    }

    bool CanDoDrain()
    {
        return player.mana >= 73;
    }

    void DoShield()
    {
        player.mana -= 113;
        playerHasSpent += 113;

        DoEffects();
        player.armor += 7;
        shieldTimer = 6;
    }

    bool CanDoShield()
    {
        return player.mana >= 113 && !shieldTimer;
    }

    void DoPoison()
    {
        player.mana -= 173;
        playerHasSpent += 173;

        DoEffects();
        player.damage += 3;
        poisonTimer = 6;
    }

    bool CanDoPoison()
    {
        return player.mana >= 173 && !poisonTimer;
    }

    void DoRecharge()
    {
        player.mana -= 229;
        playerHasSpent += 229;

        DoEffects();
        rechargeTimer = 5;
    }

    bool CanDoRecharge()
    {
        return player.mana >= 229 && !rechargeTimer;
    }

    void DoEnemyTurn()
    {
        DoEffects();

        if (!PlayerHasWon())
            player.hits -= max(enemy.damage - player.armor, 1);
    }

    bool PlayerHasWon() const
    {
        return enemy.hits <= 0;
    }

    bool PlayerHasLost() const
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
    unsigned playerHasSpent = 0;

    friend void DoTest1();
    friend void DoTest2();
    friend struct MyComparison;
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
//        if (lhs.PlayerHasSpent() == rhs.PlayerHasSpent())
            return (lhs.player.hits - lhs.enemy.hits) < (rhs.player.hits - rhs.enemy.hits);
        
//        return lhs.PlayerHasSpent() > rhs.PlayerHasSpent();
    }
};

void _tmain(int argc, _TCHAR *argv[])
{
    DoTest1();
    DoTest2();

    priority_queue<Game, vector<Game>, MyComparison> queue;
    unsigned leastCostToWin = UINT_MAX;

//    Game game(State{ 10, 0, 0, 250 }, State{ 13, 8, 0, 0 });
//    Game game(State{ 10, 0, 0, 250 }, State{ 14, 8, 0, 0 });
    Game game(State{ 50, 0, 0, 500 }, State{ 58, 9, 0, 0 });
    queue.push(game);

    unsigned temp = -1;
    while (!queue.empty())
    {
        Game game = queue.top();
        queue.pop();

//        cout << game.PlayerHasSpent() << "(" << queue.size() << ")" << endl;
        if (leastCostToWin != temp) cout << (temp = leastCostToWin) << endl;

        // Try all possible moves
        if (game.CanDoMagicMissile())
        {
            auto nextGame = game;
            nextGame.DoMagicMissile();

            if (nextGame.PlayerHasSpent() < leastCostToWin)
            {
                if (nextGame.PlayerHasWon())
                    leastCostToWin = nextGame.PlayerHasSpent();
                else if (!nextGame.PlayerHasLost())
                {
                    nextGame.DoEnemyTurn();

                    if (nextGame.PlayerHasWon())
                        leastCostToWin = nextGame.PlayerHasSpent();
                    else if (!nextGame.PlayerHasLost())
                        queue.push(nextGame);
                }
            }
        }

        if (game.CanDoDrain())
        {
            auto nextGame = game;
            nextGame.DoDrain();

            if (nextGame.PlayerHasSpent() < leastCostToWin)
            {
                if (nextGame.PlayerHasWon())
                    leastCostToWin = nextGame.PlayerHasSpent();
                else if (!nextGame.PlayerHasLost())
                {
                    nextGame.DoEnemyTurn();

                    if (nextGame.PlayerHasWon())
                        leastCostToWin = nextGame.PlayerHasSpent();
                    else if (!nextGame.PlayerHasLost())
                        queue.push(nextGame);
                }
            }
        }

        if (game.CanDoShield())
        {
            auto nextGame = game;
            nextGame.DoShield();

            if (nextGame.PlayerHasSpent() < leastCostToWin)
            {
                if (nextGame.PlayerHasWon())
                    leastCostToWin = nextGame.PlayerHasSpent();
                else if (!nextGame.PlayerHasLost())
                {
                    nextGame.DoEnemyTurn();

                    if (nextGame.PlayerHasWon())
                        leastCostToWin = nextGame.PlayerHasSpent();
                    else if (!nextGame.PlayerHasLost())
                        queue.push(nextGame);
                }
            }
        }

        if (game.CanDoPoison())
        {
            auto nextGame = game;
            nextGame.DoPoison();

            if (nextGame.PlayerHasSpent() < leastCostToWin)
            {
                if (nextGame.PlayerHasWon())
                    leastCostToWin = nextGame.PlayerHasSpent();
                else if (!nextGame.PlayerHasLost())
                {
                    nextGame.DoEnemyTurn();

                    if (nextGame.PlayerHasWon())
                        leastCostToWin = nextGame.PlayerHasSpent();
                    else if (!nextGame.PlayerHasLost())
                        queue.push(nextGame);
                }
            }
        }

        if (game.CanDoRecharge())
        {
            auto nextGame = game;
            nextGame.DoRecharge();

            if (nextGame.PlayerHasSpent() < leastCostToWin)
            {
                if (nextGame.PlayerHasWon())
                    leastCostToWin = nextGame.PlayerHasSpent();
                else if (!nextGame.PlayerHasLost())
                {
                    nextGame.DoEnemyTurn();

                    if (nextGame.PlayerHasWon())
                        leastCostToWin = nextGame.PlayerHasSpent();
                    else if (!nextGame.PlayerHasLost())
                        queue.push(nextGame);
                }
            }
        }
    }

    assert(leastCostToWin > 641 && leastCostToWin != 1415);
    cout << "part one: " << leastCostToWin << endl;
}
