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
