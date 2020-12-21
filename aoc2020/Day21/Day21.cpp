#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>

using Rule = std::pair<std::set<std::string>, std::set<std::string>>;

std::vector<Rule> rules;
std::set<std::string> ingredients, allergens;
std::map<std::string, std::set<std::string>> possible_ingredients;

// if an allergen can only be due to one ingredient, remove that ingredient
// from the other allergens as a possible cause - function returns true if it
// should be called again as reduction may not be complete
auto reduce_allergen_map()
{
    auto retval = false;

    for (auto &a : allergens)
    {
        if (possible_ingredients[a].size() == 1)
        {
            auto ingredient_to_eliminate = *possible_ingredients[a].begin();

            for (auto &other : allergens)
                if (other != a)
                    possible_ingredients[other].erase(ingredient_to_eliminate);
        }
        else
            retval = true;
    }

    return retval;
}

void map_antigens_to_ingredients()
{
    // everything is possible at first
    for (auto &a : allergens)
        for (auto &i : ingredients)
            possible_ingredients[a].insert(i);

    // consider each allergen in rule, and each possible ingredient within it -
    // if that ingredient isn't in the same rule, remove it as a possibility
    for (auto &r : rules)
        for (auto &a : r.second)
            for (auto p = possible_ingredients[a].begin(); p != possible_ingredients[a].end();)
                if (r.first.find(*p) == r.first.end())
                    possible_ingredients[a].erase(p++);
                else
                    p++;

    while (reduce_allergen_map());
}

void process_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string s;
    Rule rule;

    while (file >> s)
    {
        if (s == "(contains")
        {
            while (file >> s)
            {
                auto popped = s.back();
                s.pop_back();

                rule.second.insert(s);
                allergens.insert(s);

                if (popped == ')')
                    break;
            }
            rules.push_back(rule);
            rule.first.clear();
            rule.second.clear();
        }
        else
        {
            rule.first.insert(s);
            ingredients.insert(s);
        }
    }

    map_antigens_to_ingredients();
}

auto do_part1()
{
    auto retval = 0;
    for (auto &r : rules)
    {
        for (auto &i : r.first)
        {
            auto present = false;
            for (auto &a : allergens)
                if (possible_ingredients[a].find(i) != possible_ingredients[a].end())
                {
                    present = true;
                    break;
                }

            if (!present)
                retval++;
        }
    }

    return retval;
}

auto do_part2()
{
    std::string retval;

    for (auto &a : allergens)
    {
        if (possible_ingredients[a].size() == 1)
        {
            if (retval.length())
                retval.push_back(',');

            retval += *possible_ingredients[a].begin();
        }
    }

    return retval;
}

int main()
{
    process_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 2798);
    assert(part2 == "gbt,rpj,vdxb,dtb,bqmhk,vqzbq,zqjm,nhjrzzj");
    return 0;
}
