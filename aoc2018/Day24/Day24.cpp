#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <numeric>
#include <algorithm>
#include <cassert>

//#define DEBUG_OUTPUT

enum Type { Immune, Infection };
enum AttackType { Radiation = 0x01, Bludgeoning = 0x02, Fire = 0x04, Slashing = 0x08, Cold = 0x10 };

struct Group
{
    Type type;
    int group_num;
    int num_units;
    int hit_points;
    int attack_damage;
    int attack_type;
    int initiative;
    int weakness;
    int immunities;

    Group(Type type, int group_num, int num_units, int hit_points, int attack_damage, int attack_type, int initiative, int weakness, int immunities) :
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
    bool receive_attack(Group &other) // returns true is attack actually changed anything
    {
        auto damage = other.attack_damage * other.num_units;
        if (immunities & other.attack_type) return false;
        if (weakness & other.attack_type) damage *= 2;

        auto num_units_destroyed = damage / hit_points;
        if (num_units_destroyed > num_units) num_units_destroyed = num_units;
        num_units -= num_units_destroyed;
        return num_units_destroyed > 0;

#ifdef DEBUG_OUTPUT
        std::cout << ((other.type == Infection) ? "Infection" : "Immune System") << " group " << other.group_num << " attacks defending group " << group_num << ", killing " << num_units_destroyed << " unit(s)" << std::endl;
#endif
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

#ifdef DEBUG_OUTPUT
            std::cout << ((g->type == Infection) ? "Infection" : "Immune System") << " group " << g->group_num
                << " would deal defending group " << other->group_num << " " << other->damage_would_be_dealt(*g) << " damage" << std::endl;
#endif
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
#ifdef DEBUG_OUTPUT
    std::cout << std::endl;
#endif

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
#ifdef DEBUG_OUTPUT
    std::cout << std::endl;
#endif

    // Prune dead groups
    groups.erase(
        std::remove_if(groups.begin(), groups.end(), [&](const Group *item) { return item->num_units <= 0; }),
        groups.end());

    return attacks_had_effect;
}

unsigned do_part1or2(bool &combat_ended)
{
    combat_ended = false;

    for (;;)
    {
#ifdef DEBUG_OUTPUT
        auto count = 0;
        std::cout << "Immune System:" << std::endl;
        for (auto &g : groups)
        {
            if (g->type == Immune)
                std::cout << "Group " << ++count << " contains " << g->num_units << " unit(s)" << std::endl;
        }

        count = 0;
        std::cout << "Infection:" << std::endl;
        for (auto &g : groups)
        {
            if (g->type == Infection)
                std::cout << "Group " << ++count << " contains " << g->num_units << " unit(s)" << std::endl;
        }
        std::cout << std::endl;
#endif
        // Stalemate
        if (!do_fight())
            return 0;

        bool has_immune = false, has_infection = false;
        for (auto &g : groups)
        {
            if (g->type == Immune) has_immune = true;
            if (g->type == Infection) has_infection = true;
        }

        if (!has_immune || !has_infection)
        {
            combat_ended = true;
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

// Type type; int units;  int hit_points; int attack_damage; int attack_type; int initiative; int weakness; int immunities;

/*
Immune System:
17 units each with 5390 hit points (weak to radiation, bludgeoning) with
an attack that does 4507 fire damage at initiative 2
989 units each with 1274 hit points (immune to fire; weak to bludgeoning,
slashing) with an attack that does 25 slashing damage at initiative 3

Infection:
801 units each with 4706 hit points (weak to radiation) with an attack
that does 116 bludgeoning damage at initiative 1
4485 units each with 2961 hit points (immune to radiation; weak to fire,
cold) with an attack that does 12 slashing damage at initiative 4
*/

void populate_data(int boost = 0)
{
    while (!groups.empty())
    {
        delete groups.back();
        groups.pop_back();
    }

    // Example input
    //groups.push_back(new Group(Immune, 1, 17, 5390, 4507, Fire, 2, Radiation | Bludgeoning, 0));
    //groups.push_back(new Group(Immune, 2, 989, 1274, 25, Slashing, 3, Bludgeoning | Slashing, Fire));
    //groups.push_back(new Group(Infection, 1, 801, 4706, 116, Bludgeoning, 1, Radiation, 0));
    //groups.push_back(new Group(Infection, 2, 4485, 2961, 12, Slashing, 4, Fire | Cold, Radiation));

    // Real input
    // 2743 units each with 4149 hit points with an attack that does 13 radiation damage at initiative 14
    groups.push_back(new Group(Immune, 1, 2743, 4149, 13 + boost, Radiation, 14, 0, 0));
    // 8829 units each with 7036 hit points with an attack that does 7 fire damage at initiative 15
    groups.push_back(new Group(Immune, 2, 8829, 7036, 7 + boost, Fire, 15, 0, 0));
    // 1928 units each with 10700 hit points (weak to cold; immune to fire, radiation, slashing) with an attack that does 50 slashing damage at initiative 3
    groups.push_back(new Group(Immune, 3, 1928, 10700, 50 + boost, Slashing, 3, Cold, Fire | Radiation | Slashing));
    // 6051 units each with 11416 hit points with an attack that does 15 bludgeoning damage at initiative 20
    groups.push_back(new Group(Immune, 4, 6051, 11416, 15 + boost, Bludgeoning, 20, 0, 0));
    // 895 units each with 10235 hit points (immune to slashing; weak to bludgeoning) with an attack that does 92 bludgeoning damage at initiative 10
    groups.push_back(new Group(Immune, 5, 895, 10235, 92 + boost, Bludgeoning, 10, Bludgeoning, Slashing));
    // 333 units each with 1350 hit points with an attack that does 36 radiation damage at initiative 12
    groups.push_back(new Group(Immune, 6, 333, 1350, 36 + boost, Radiation, 12, 0, 0));
    // 2138 units each with 8834 hit points (weak to bludgeoning) with an attack that does 35 cold damage at initiative 11
    groups.push_back(new Group(Immune, 7, 2138, 8834, 35 + boost, Cold, 11, Bludgeoning, 0));
    // 4325 units each with 1648 hit points (weak to cold, fire) with an attack that does 3 bludgeoning damage at initiative 8
    groups.push_back(new Group(Immune, 8, 4325, 1648, 3 + boost, Bludgeoning, 8, Cold | Fire, 0));
    // 37 units each with 4133 hit points (immune to radiation, slashing) with an attack that does 1055 radiation damage at initiative 1
    groups.push_back(new Group(Immune, 9, 37, 4133, 1055 + boost, Radiation, 1, 0, Radiation | Slashing));
    // 106 units each with 3258 hit points (immune to slashing, radiation) with an attack that does 299 cold damage at initiative 13
    groups.push_back(new Group(Immune, 10, 106, 3258, 299 + boost, Cold, 13, 0, Slashing | Radiation));

    // 262 units each with 8499 hit points (weak to cold) with an attack that does 45 cold damage at initiative 6
    groups.push_back(new Group(Infection, 1, 262, 8499, 45, Cold, 6, Cold, 0));
    // 732 units each with 47014 hit points (weak to cold, bludgeoning) with an attack that does 127 bludgeoning damage at initiative 17
    groups.push_back(new Group(Infection, 2, 732, 47014, 127, Bludgeoning, 17, Cold | Bludgeoning, 0));
    // 4765 units each with 64575 hit points with an attack that does 20 radiation damage at initiative 18
    groups.push_back(new Group(Infection, 3, 4765, 64575, 20, Radiation, 18, 0, 0));
    // 3621 units each with 19547 hit points (immune to radiation, cold) with an attack that does 9 cold damage at initiative 5
    groups.push_back(new Group(Infection, 4, 3621, 19547, 9, Cold, 5, 0, Radiation | Cold));
    // 5913 units each with 42564 hit points (immune to radiation, bludgeoning, fire) with an attack that does 14 slashing damage at initiative 9
    groups.push_back(new Group(Infection, 5, 5913, 42564, 14, Slashing, 9, 0, Radiation | Bludgeoning | Fire));
    // 7301 units each with 51320 hit points (weak to radiation, fire; immune to bludgeoning) with an attack that does 11 fire damage at initiative 2
    groups.push_back(new Group(Infection, 6, 7301, 51320, 11, Fire, 2, Radiation | Fire, Bludgeoning));
    // 3094 units each with 23713 hit points (weak to slashing, fire) with an attack that does 14 radiation damage at initiative 19
    groups.push_back(new Group(Infection, 7, 3094, 23713, 14, Radiation, 19, Slashing | Fire, 0));
    // 412 units each with 36593 hit points (weak to radiation, bludgeoning) with an attack that does 177 slashing damage at initiative 16
    groups.push_back(new Group(Infection, 8, 412, 36593, 177, Slashing, 16, Radiation | Bludgeoning, 0));
    // 477 units each with 35404 hit points with an attack that does 146 cold damage at initiative 7
    groups.push_back(new Group(Infection, 9, 477, 35404, 146, Cold, 7, 0, 0));
    // 332 units each with 11780 hit points (weak to fire) with an attack that does 70 slashing damage at initiative 4*/
    groups.push_back(new Group(Infection, 10, 332, 11780, 70, Slashing, 4, Fire, 0));
}

int main()
{
    populate_data();

    bool combat_ended = false;
    auto part1 = do_part1or2(combat_ended);

    auto boost = 1, part2 = 0;
    for (;;)
    {
        populate_data(boost);
        part2 = do_part1or2(combat_ended);
        if (combat_ended && groups[0]->type == Immune)
            break;
        boost++;
    }

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 24318);
    assert(part2 == 1083);
    return 0;
}
