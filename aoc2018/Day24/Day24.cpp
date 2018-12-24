#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

enum Type { ImmuneSystem, Infection };
enum AttackType { Radiation = 0x01, Bludgeoning = 0x02, Fire = 0x04, Slashing = 0x08, Cold = 0x10 };

struct Group
{
    Type type;
    int group_num;
    int num_units;
    int hit_points;
    int attack_damage;
    AttackType attack_type;
    int initiative;
    int weakness;
    int immunities;

    Group(Type type, int group_num, int num_units, int hit_points, int attack_damage, AttackType attack_type, int initiative, int weakness, int immunities) :
        type(type), group_num(group_num), num_units(num_units), hit_points(hit_points), attack_damage(attack_damage), attack_type(attack_type), initiative(initiative), weakness(weakness), immunities(immunities)
    {
    }
    int get_effective_power() const { return num_units * attack_damage; }
    int damage_would_be_dealt(Group &other)
    {
        if (immunities & other.attack_type) return 0;
        auto damage = other.attack_damage * other.num_units;
        if (weakness & other.attack_type) damage *= 2;
        return damage;
    }
    bool receive_attack(Group &other) // returns true if attack actually changed anything
    {
        auto damage = other.attack_damage * other.num_units;
        if (immunities & other.attack_type) return false;
        if (weakness & other.attack_type) damage *= 2;

        auto num_units_destroyed = damage / hit_points;
        if (num_units_destroyed > num_units) num_units_destroyed = num_units;
        num_units -= num_units_destroyed;
        return num_units_destroyed > 0;
    }
};

std::vector<Group *> groups;

bool do_fight() // returns false if nothing changed
{
    // 1. Target Selection

    // Sort by effective power, then initiative to determine selection order.
    std::sort(groups.begin(), groups.end(), [&](const Group *lhs, const Group *rhs) -> bool {
        auto ep1 = lhs->get_effective_power(), ep2 = rhs->get_effective_power();
        if (ep1 > ep2) return true; else if (ep2 > ep1) return false;
        return (lhs->initiative > rhs->initiative);
    });

    // For each group, choose a target group (or none).
    std::map<Group *, Group *> attacked_by;
    for (auto &g : groups)
    {
        std::map<Group *, int> potential_damage;
        potential_damage.clear();
        for (auto other : groups)
        {
            if (other->type == g->type) continue; // group can't attack its own kind (nor itself)
            if (attacked_by.find(other) != attacked_by.end()) continue; // same group can't be attacked by multiple groups
            if (other->immunities & g->attack_type) continue; // this group has immunity, don't attack it
            potential_damage[other] = other->damage_would_be_dealt(*g);
        }

        if (!potential_damage.empty())
        {
            int highest_damage = -1;
            Group *best_target = nullptr;
            for (auto &it : potential_damage)
            {
                if (it.second > highest_damage)
                {
                    highest_damage = it.second;
                    best_target = it.first;
                }
                else if (it.second == highest_damage && it.first->get_effective_power() > best_target->get_effective_power())
                {
                    highest_damage = it.second;
                    best_target = it.first;
                }
            }

            attacked_by[best_target] = g;
        }
    }

    // 2. Attacking
    // Build list of descending order of initiative to launch attacks
    std::vector<Group *> attack_order;
    for (auto &it : attacked_by)
        attack_order.push_back(it.second);

    // Sort by attacker initiative
    std::sort(attack_order.begin(), attack_order.end(), [&](const Group *lhs, const Group *rhs) -> bool {
        return lhs->initiative > rhs->initiative;
    });

    // Actually do the attacks
    bool attacks_had_effect = false;
    for (auto attacker : attack_order)
    {
        if (attacker->hit_points > 0)
        {
            Group *attackee = nullptr;
            for (auto it : attacked_by)
            {
                if (it.second == attacker)
                    attackee = it.first;
            }

            if (attackee)
                attacks_had_effect |= attackee->receive_attack(*attacker);
        }
    }

    // Prune dead groups
    groups.erase(
        std::remove_if(groups.begin(), groups.end(), [&](const Group *item) { return item->num_units <= 0; }),
        groups.end());

    return attacks_had_effect;
}

void populate_data(int boost = 0)
{
    while (!groups.empty())
    {
        delete groups.back();
        groups.pop_back();
    }

    // Problem input -- small enough to hand-parse
    groups.push_back(new Group(ImmuneSystem, 1, 2743, 4149, 13 + boost, Radiation, 14, 0, 0));
    groups.push_back(new Group(ImmuneSystem, 2, 8829, 7036, 7 + boost, Fire, 15, 0, 0));
    groups.push_back(new Group(ImmuneSystem, 3, 1928, 10700, 50 + boost, Slashing, 3, Cold, Fire | Radiation | Slashing));
    groups.push_back(new Group(ImmuneSystem, 4, 6051, 11416, 15 + boost, Bludgeoning, 20, 0, 0));
    groups.push_back(new Group(ImmuneSystem, 5, 895, 10235, 92 + boost, Bludgeoning, 10, Bludgeoning, Slashing));
    groups.push_back(new Group(ImmuneSystem, 6, 333, 1350, 36 + boost, Radiation, 12, 0, 0));
    groups.push_back(new Group(ImmuneSystem, 7, 2138, 8834, 35 + boost, Cold, 11, Bludgeoning, 0));
    groups.push_back(new Group(ImmuneSystem, 8, 4325, 1648, 3 + boost, Bludgeoning, 8, Cold | Fire, 0));
    groups.push_back(new Group(ImmuneSystem, 9, 37, 4133, 1055 + boost, Radiation, 1, 0, Radiation | Slashing));
    groups.push_back(new Group(ImmuneSystem, 10, 106, 3258, 299 + boost, Cold, 13, 0, Slashing | Radiation));

    groups.push_back(new Group(Infection, 1, 262, 8499, 45, Cold, 6, Cold, 0));
    groups.push_back(new Group(Infection, 2, 732, 47014, 127, Bludgeoning, 17, Cold | Bludgeoning, 0));
    groups.push_back(new Group(Infection, 3, 4765, 64575, 20, Radiation, 18, 0, 0));
    groups.push_back(new Group(Infection, 4, 3621, 19547, 9, Cold, 5, 0, Radiation | Cold));
    groups.push_back(new Group(Infection, 5, 5913, 42564, 14, Slashing, 9, 0, Radiation | Bludgeoning | Fire));
    groups.push_back(new Group(Infection, 6, 7301, 51320, 11, Fire, 2, Radiation | Fire, Bludgeoning));
    groups.push_back(new Group(Infection, 7, 3094, 23713, 14, Radiation, 19, Slashing | Fire, 0));
    groups.push_back(new Group(Infection, 8, 412, 36593, 177, Slashing, 16, Radiation | Bludgeoning, 0));
    groups.push_back(new Group(Infection, 9, 477, 35404, 146, Cold, 7, 0, 0));
    groups.push_back(new Group(Infection, 10, 332, 11780, 70, Slashing, 4, Fire, 0));
}

unsigned do_part1(bool *combat_ended = nullptr, unsigned boost = 0)
{
    populate_data(boost);

    if (combat_ended)
        *combat_ended = false;

    for (;;)
    {
        // Stalemate
        if (!do_fight())
            return 0;

        bool has_immune_system = false, has_infection = false;
        for (auto &g : groups)
        {
            if (g->type == ImmuneSystem) has_immune_system = true;
            if (g->type == Infection) has_infection = true;
        }

        if (!has_immune_system || !has_infection)
        {
            if (combat_ended)
                *combat_ended = true;
            break;
        }
    }

    unsigned total = 0;
    for (auto g : groups)
    {
        total += g->num_units;
    }
    return total;
}

unsigned do_part2()
{
    bool combat_ended = false;
    auto boost = 1, retval = 0;
    for (;;)
    {
        retval = do_part1(&combat_ended, boost);
        if (combat_ended && groups[0]->type == ImmuneSystem)
            return retval;
        boost++;
    }
}

int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 24318);
    assert(part2 == 1083);
    return 0;
}
