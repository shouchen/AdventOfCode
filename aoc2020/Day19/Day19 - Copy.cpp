#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <stack>
#include <regex>
#include <algorithm>
#include <cassert>

struct Rule
{
    int num = -1; // just for debugging
    char c = ' '; // c == ' ' means use subrules
    std::vector<std::vector<int>> subrules; // zero if rule is char literal, else 1 or two
};

std::map<int, Rule> rules;

struct Node
{
    std::string s;
    std::list<int> rules;
};

void read_rules(std::istream &file, bool part2)
{
    rules.clear();

    //std::ifstream file("input.txt");
    std::string s;
    auto r = 0, n = 0;
    char colon;

    while (getline(file, s) && s.length())
    {
        if (part2)
        {
            if (s.substr(0, 2) == "8:")
                s = "8: 42 | 42 8";
            else if (s.substr(0, 3) == "11:")
                s = "11: 42 31 | 42 11 31";
        }

        std::stringstream ss(s);
        ss >> r >> colon;
        rules[r].num = r; // create rule

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
                    rules[temp].num = temp;

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

bool validate(const std::string &str)
{
    std::stack<Node *> q;

    Node *node = new Node{ str, {0} };
    q.push(node);

    while (!q.empty())
    {
        Node *curr = q.top();
        q.pop();

        if (curr->rules.empty())
        {
            auto done = curr->s.empty();
            delete curr;
            if (!done)
                continue;

            while (!q.empty())
            {
                delete q.top();
                q.pop();
            }

            return true;
        }

        int first_rule = curr->rules.front();
        const Rule &rule = rules[first_rule];

        if (rule.subrules.size() == 0)
        {
            if (curr->s[0] == rule.c)
            {
                node = new Node{ curr->s.substr(1), curr->rules };
                node->rules.pop_front();

                q.push(node);
            }
        }
        else if (rule.subrules.size() == 1)
        {
            node = new Node{ curr->s, curr->rules };
            node->rules.pop_front();

            for (int i = rule.subrules[0].size() - 1; i >= 0; --i)
                node->rules.push_front(rule.subrules[0][i]);

            q.push(node);
        }
        else if (rule.subrules.size() == 2)
        {
            node = new Node{ curr->s, curr->rules };
            node->rules.pop_front();

            for (int i = rule.subrules[0].size() - 1; i >= 0; --i)
                node->rules.push_front(rule.subrules[0][i]);

            q.push(node);

            node = new Node{ curr->s, curr->rules };
            node->rules.pop_front();

            for (int i = rule.subrules[1].size() - 1; i >= 0; --i)
                node->rules.push_front(rule.subrules[1][i]);

            q.push(node);
        }

        delete curr;
    }

    return false;
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
