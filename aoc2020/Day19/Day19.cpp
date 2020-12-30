#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <list>
//#include <stack>
#include <cassert>

struct Rule
{
    char c;
    std::vector<std::vector<int>> subrules;
};

std::map<int, Rule> rules;

void read_rules(std::istream &file, bool part2)
{
    rules.clear();

    std::string s;

    while (getline(file, s) && s.length())
    {
        if (part2)
        {
            if (s.substr(0, 2) == "8:")
                s = "8: 42 | 42 8";
            else if (s.substr(0, 3) == "11:")
                s = "11: 42 31 | 42 11 31";
        }

        auto rule = 0;
        auto colon = ':';

        std::stringstream ss(s);
        ss >> rule >> colon;

        std::string token;
        while (ss >> token)
        {
            if (token[0] == '"')
            {
                rules[rule].c = token[1];
                break;
            }

            rules[rule].subrules.push_back(std::vector<int>());

            do
            {
                if (token == "|")
                {
                    rules[rule].subrules.push_back(std::vector<int>());
                    ss >> token;
                }

                rules[rule].subrules.back().push_back(atoi(token.c_str()));
            } while (ss >> token);
        }
    }
}

auto recurse(const char *s, std::list<int> &rule_list)
{
    if (!rule_list.size())
        return !*s;

    auto rule_num = rule_list.front();
    auto &rule = rules[rule_num];

    if (rule.subrules.size() == 0)
    {
        if (*s++ == rule.c)
        {
            rule_list.pop_front();
            if (recurse(s, rule_list))
                return true;
            rule_list.push_front(rule_num);
        }
    }
    else
    {
        rule_list.pop_front();

        for (auto &subrule : rule.subrules)
        {
            rule_list.insert(rule_list.begin(), subrule.begin(), subrule.end());
            if (recurse(s, rule_list))
                return true;
            for (auto &i : subrule)
                rule_list.pop_front();
        }

        rule_list.push_front(rule_num);
    }

    return false;
}

auto validate(const std::string &str)
{
    std::list<int> rules{ 0 };
    return recurse(str.c_str(), rules);
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file("input.txt");
    read_rules(file, part2);

    std::string s;
    auto retval = 0;

    while (file >> s)
        if (validate(s))
            retval++;

    return retval;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 182);
    assert(part2 == 334);
    return 0;
}
