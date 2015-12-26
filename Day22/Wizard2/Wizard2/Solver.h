#pragma once

enum Spell
{
    MagicMissile, Drain, Shield, Poison, Recharge
};

struct PlayerState
{
    int hp;
    int mana;
};

struct BossState
{
    int hp;
};

struct EffectState
{
    int shield;
    int poison;
    int recharge;
};

struct State
{
    PlayerState player;
    BossState boss;
    EffectState effects;

    int cost;
};

struct Solver
{
    static int Solve(int playerhp, int mana, int bosshp, int bossdam, bool hardMode);

private:
    static inline bool PlayerTurnActionFirstPart(State &state, Spell &spell);
    static inline bool PlayerTurnActionSecondPart(State &state, Spell &spell);
    static inline void ApplyEffects(State &state, Spell &spell, int *armor);
    static bool Round(State state, Spell spell);

    static int s_bossDamage;
    static int s_lowestCost;
    static bool s_hardMode;
    static const int s_spellCost[5];
};

bool Solver::PlayerTurnActionFirstPart(State &state, Spell &spell)
{
    switch (spell)
    {
    case MagicMissile:
        state.boss.hp -= 4;
        break;
    case Drain:
        state.boss.hp -= 2;
        state.player.hp += 2;
        break;
    case Shield:
        if (state.effects.shield > 0) return false;
        state.effects.shield = 6;
        break;
    case Poison:
        if (state.effects.poison > 0) return false;
        state.effects.poison = 6;
        break;
    case Recharge:
        if (state.effects.recharge > 0) return false;
        state.effects.recharge = 5;
        break;
    }

    return true;
}

bool Solver::PlayerTurnActionSecondPart(State &state, Spell &spell)
{
    state.player.mana -= s_spellCost[spell];
    state.cost += s_spellCost[spell];

    if (state.boss.hp <= 0)
    {
        if (state.cost < Solver::s_lowestCost) Solver::s_lowestCost = state.cost;
        return true;
    }

    return false;
}

void Solver::ApplyEffects(State &state, Spell &spell, int *armor)
{
    if (state.effects.recharge > 0)
    {
        state.player.mana += 101;
        state.effects.recharge--;
    }
    if (state.effects.poison > 0)
    {
        state.boss.hp -= 3;
        state.effects.poison--;
    }
    if (state.effects.shield > 0)
    {
        state.effects.shield--;
        if (armor) *armor = 7;
    }
    else
    if (armor) *armor = 0;
}

