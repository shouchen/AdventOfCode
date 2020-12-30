#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <cassert>

struct Rule
{
    char c = ' ';
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

        auto r = 0;
        char colon;

        std::stringstream ss(s);
        ss >> r >> colon;

        int group = 0;
        std::string t;
        while (ss >> t)
        {
            if (t[0] == '"')
            {
                rules[r].c = t[1];
                break;
            }
            else
            {
                rules[r].subrules.push_back(std::vector<int>());
                for (;;)
                {
                    auto temp = atoi(t.c_str());

                    rules[r].subrules[group].push_back(temp);
                    if (!(ss >> t))
                        break;

                    if (t == "|")
                    {
                        group++;
                        rules[r].subrules.push_back(std::vector<int>());
                        if (!(ss >> t))
                            break;
                    }
                }
            }
        }
    }
}

struct Node
{
    std::string s;
    std::list<int> rules;
};

bool recur(const std::string &s, std::list<int> &rule_list)
{
    const Rule &rule = rules[rule_list.front()];

    if (rule.subrules.size() == 0)
    {
        if (s[0] == rule.c)
        {
            Node node{ s.substr(1), rule_list };
            node.rules.pop_front();

            if (node.rules.empty())
            {
                if (node.s.empty())
                    return true;
            }
            else
            {
                if (recur(node.s, node.rules))
                    return true;
            }
        }
    }
    else
    {
        for (auto &subrule : rule.subrules)
        {
            Node node{ s, rule_list };
            node.rules.pop_front();

            for (int i = subrule.size() - 1; i >= 0; --i)
                node.rules.push_front(subrule[i]);

            if (recur(node.s, node.rules))
                return true;
        }
    }

    return false;
}

bool validate2(const std::string &str)
{
    std::list<int> rules{ 0 };
    return recur(str, rules);
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file("input.txt");
    read_rules(file, part2);

    std::string s;
    auto retval = 0;

    while (file >> s)
        if (validate2(s))
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
