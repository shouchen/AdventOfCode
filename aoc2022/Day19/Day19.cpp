#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <cassert>

enum Material
{
    Ore, Clay, Obsidian, Geode
};

struct State
{
    int robots[4], material[4], minutes;

    State()
    {
        for (int i = 0; i < 4; i++)
            robots[i] = material[i] = 0;
        minutes = 0;
    }
};

struct Blueprint
{
    // first index = robot to buy, second = cost of each material
    int cost[4][4];
    Blueprint()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                cost[i][j] = 0;
    }
};

std::vector<Blueprint> blueprints;

Material parse_material(const std::string &material)
{
    if (material == "ore") return Ore;
    if (material == "clay") return Clay;
    if (material == "obsidian") return Obsidian;
    assert(material == "geode");
    return Geode;
}

bool purchase(Blueprint &bp, State &state, Material robot)
{
    for (int i = Ore; i <= Geode; i++)
        if (state.material[i] < bp.cost[robot][i]) return false;

    for (int i = Ore; i <= Geode; i++)
        state.material[i] -= bp.cost[robot][i];

    return true;
}

auto max_geodes = -1;
auto max_minutes = 24;

auto is_hopeless(const State &state)
{
    auto mins_left = max_minutes - state.minutes;
    auto new_geodes_with_existing_geobots = mins_left * state.robots[Geode];

    auto n = mins_left - 1;
    auto addl_geodes_if_bought_geobot_every_minute = n * (n + 1) / 2;
    auto total_geodes_at_end = state.material[Geode] + new_geodes_with_existing_geobots + addl_geodes_if_bought_geobot_every_minute;
    if (total_geodes_at_end <= max_geodes)
        return true;

    return false;
}

void recurse(const State &state, int bp_num)
{
    if (state.minutes == max_minutes)
    {
        if (state.material[Geode] > max_geodes)
        {
            std::cout << "geodes = " << state.material[Geode] << std::endl;
            max_geodes = state.material[Geode];
        }
        return;
    }

    // Prune
    if (is_hopeless(state))
    {
        return;
    }

    // pay for optional purchases here: Ore
    State new_state = state;
    if (purchase(blueprints[bp_num - 1], new_state, Geode))
    {
        for (int i = Ore; i <= Geode; i++)
            new_state.material[i] += new_state.robots[i];

        new_state.robots[Geode]++;
        new_state.minutes++;
        recurse(new_state, bp_num);
    }

    new_state = state;
    if (purchase(blueprints[bp_num - 1], new_state, Obsidian))
    {
        for (int i = Ore; i <= Geode; i++)
            new_state.material[i] += new_state.robots[i];

        new_state.robots[Obsidian]++;
        new_state.minutes++;
        recurse(new_state, bp_num);
    }

    new_state = state;
    if (purchase(blueprints[bp_num - 1], new_state, Clay))
    {
        for (int i = Ore; i <= Geode; i++)
            new_state.material[i] += new_state.robots[i];

        new_state.robots[Clay]++;
        new_state.minutes++;
        recurse(new_state, bp_num);
    }

    // OPTIMIZATION: If we produce enough ore already to buy any kind of robot, don't buy any more ore robots
    bool produce_enough_ore_to_buy_any_robot =
        state.robots[Ore] >= blueprints[bp_num - 1].cost[Clay][Ore] &&
        state.robots[Ore] >= blueprints[bp_num - 1].cost[Obsidian][Ore] &&
        state.robots[Ore] >= blueprints[bp_num - 1].cost[Geode][Ore];
    if (!produce_enough_ore_to_buy_any_robot)
    {
        new_state = state;
        if (purchase(blueprints[bp_num - 1], new_state, Ore))
        {
            for (int i = Ore; i <= Geode; i++)
                new_state.material[i] += new_state.robots[i];

            new_state.robots[Ore]++;
            new_state.minutes++;
            recurse(new_state, bp_num);
        }
    }

    // no purchases this time
    new_state = state;
    for (int i = Ore; i <= Geode; i++)
        new_state.material[i] += new_state.robots[i];

    new_state.minutes++;
    recurse(new_state, bp_num);
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string word, blueprint, costs, material1, material2, robot;
    std::string and_str;
    char colon;
    int n1, n2;

    file >> blueprint;

    while (file >> n1)
    {
        file >> colon;

        Blueprint bp;

        while (file >> word)
        {
            if (word == "Blueprint")
                break;
            file >> material1 >> robot >> costs;

            while (file >> n2 >> material2)
            {
                if (material2.back() == '.')
                {
                    material2 = material2.substr(0, material2.length() - 1);
                    bp.cost[parse_material(material1)][parse_material(material2)] = n2;
                    break;
                }
                bp.cost[parse_material(material1)][parse_material(material2)] = n2;
                file >> and_str;
            }
        }

        blueprints.push_back(bp);
    }

    State state;
    state.robots[Ore] = 1;

    auto retval = 0;

    for (auto i = 1; i <= blueprints.size(); i++)
    {
        std::cout << "BLUEPRINT " << i << std::endl;
        max_geodes = 0;
        recurse(state, i);
        retval += i * max_geodes;
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string word, blueprint, costs, material1, material2, robot;
    std::string and_str;
    char colon;
    int n1, n2;

    file >> blueprint;

    while (file >> n1)
    {
        file >> colon;

        Blueprint bp;

        while (file >> word)
        {
            if (word == "Blueprint")
                break;
            file >> material1 >> robot >> costs;

            while (file >> n2 >> material2)
            {
                if (material2.back() == '.')
                {
                    material2 = material2.substr(0, material2.length() - 1);
                    bp.cost[parse_material(material1)][parse_material(material2)] = n2;
                    break;
                }
                bp.cost[parse_material(material1)][parse_material(material2)] = n2;
                file >> and_str;
            }
        }

        blueprints.push_back(bp);
        if (blueprints.size() == 3)
            break;
    }

    max_minutes = 32;

    State state;
    state.robots[Ore] = 1;

    auto retval = 1;

    for (auto i = 1; i <= blueprints.size(); i++)
    {
        std::cout << "BLUEPRINT " << i << std::endl;
        max_geodes = 0;
        recurse(state, i);
        retval *= max_geodes;
    }

    return retval;
}

int main()
{
    //auto part1 = do_part1("input.txt");
    //std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 1653);
    //assert(part2 == 4212);
    return 0;
}
