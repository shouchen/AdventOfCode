#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <queue>
#include <map>
#include <stack>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cstdlib>
#include <cassert>

using namespace std;

struct Monkey
{
    string name;
    bool has_value;
    long long value;
    string m1, m2;
    char op;
};

map<string, Monkey> monkeys;

auto read_file_line_by_line(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, m1, m2, word;

    while (getline(file, line))
    {
        cout << line << endl;

        Monkey monkey;

        stringstream ss(line);
        ss >> monkey.name >> word;

        monkey.name = monkey.name.substr(0, monkey.name.length() - 1); // no colon

        if (isdigit(word[0]))
        {
            monkey.has_value = true;
            monkey.value = atoi(word.c_str());
        }
        else
        {
            monkey.has_value = false;
            monkey.m1 = word;
            ss >> monkey.op >> monkey.m2;
        }

        monkeys[monkey.name] = monkey;
    }

    for (;;)
    {
        for (auto &i : monkeys)
        {
            Monkey &m = i.second;
            if (!m.has_value)
            {
                if (monkeys[m.m1].has_value && monkeys[m.m2].has_value)
                {
                    switch (m.op)
                    {
                    case '+': m.value = monkeys[m.m1].value + monkeys[m.m2].value; m.has_value = true; break;
                    case '-': m.value = monkeys[m.m1].value - monkeys[m.m2].value; m.has_value = true; break;
                    case '*': m.value = monkeys[m.m1].value * monkeys[m.m2].value; m.has_value = true; break;
                    case '/': m.value = monkeys[m.m1].value / monkeys[m.m2].value; m.has_value = true; break;
                    default:
                        assert(false);
                    }
                }

                if (i.first == "root")
                {
                    cout << m.value << endl;
                }
            }

        }

    }
}

auto do_part2(const std::string &filename, int humn)
{
    std::ifstream file(filename);
    std::string line, m1, m2, word;
    char op;

    while (getline(file, line))
    {
        Monkey monkey;

        stringstream ss(line);
        ss >> monkey.name >> word;

        monkey.name = monkey.name.substr(0, monkey.name.length() - 1); // no colon

        if (monkey.name == "humn")
        {
            monkey.has_value = true;
            monkey.value = humn;
        }
        else if (isdigit(word[0]))
        {
            monkey.has_value = true;
            monkey.value = atoi(word.c_str());
        }
        else
        {
            monkey.has_value = false;
            monkey.m1 = word;
            ss >> monkey.op >> monkey.m2;
        }

        monkeys[monkey.name] = monkey;
    }

    for (;;)
    {
        bool found_something = false;
        for (auto &i : monkeys)
        {
            Monkey &m = i.second;
            if (!m.has_value)
            {
                if (monkeys[m.m1].has_value && monkeys[m.m2].has_value)
                {
                    switch (m.op)
                    {
                    case '+': m.value = monkeys[m.m1].value + monkeys[m.m2].value; m.has_value = true; break;
                    case '-': m.value = monkeys[m.m1].value - monkeys[m.m2].value; m.has_value = true; break;
                    case '*': m.value = monkeys[m.m1].value * monkeys[m.m2].value; m.has_value = true; break;
                    case '/': m.value = monkeys[m.m1].value / monkeys[m.m2].value; m.has_value = true; break;
                    case '=': return monkeys[m.m1].value == monkeys[m.m2].value;
                    default:
                        assert(false);
                    }

                    if (i.first == "root")
                    {
                        cout << "testing " << monkeys[m.m1].value << " " << monkeys[m.m2].value << endl;
                        return monkeys[m.m1].value == monkeys[m.m2].value;
                    }

                    found_something = true;
                }
            }
        }

        if (!found_something)
            return false;
    }
}

bool recur(const std::string &name, vector<Monkey> &path)
{
    if (name == "humn")
        return true;

    auto m = monkeys[name];
    if (!m.m1.empty())
        if (recur(m.m1, path))
        {
            path.push_back(monkeys[m.m1]);
            return true;
        }
    if (!m.m2.empty())
        if (recur(m.m2, path))
        {
            path.push_back(monkeys[m.m2]);
            return true;
        }

    return false;
}

void figure_it_out()
{
    std::ifstream file("input.txt");
    std::string line, m1, m2, word;
    char op;

    while (getline(file, line))
    {
        Monkey monkey;

        stringstream ss(line);
        ss >> monkey.name >> word;

        monkey.name = monkey.name.substr(0, monkey.name.length() - 1); // no colon

        if (monkey.name == "humn")
        {
            monkey.has_value = false;
            monkey.m1 = "xqrt"; // unused name to ensure doesn't resolve
            monkey.m2 = "trwz"; // unused name to ensure doesn't resolve
        }
        else if (isdigit(word[0]))
        {
            monkey.has_value = true;
            monkey.value = atoi(word.c_str());
        }
        else
        {
            monkey.has_value = false;
            monkey.m1 = word;
            ss >> monkey.op >> monkey.m2;
        }

        monkeys[monkey.name] = monkey;
    }

    for (;;)
    {
        bool found_something = false;
        for (auto &i : monkeys)
        {
            Monkey &m = i.second;
            if (!m.has_value)
            {
                if (monkeys[m.m1].has_value && monkeys[m.m2].has_value)
                {
                    switch (m.op)
                    {
                    case '+': m.value = monkeys[m.m1].value + monkeys[m.m2].value; m.has_value = true; break;
                    case '-': m.value = monkeys[m.m1].value - monkeys[m.m2].value; m.has_value = true; break;
                    case '*': m.value = monkeys[m.m1].value * monkeys[m.m2].value; m.has_value = true; break;
                    case '/': m.value = monkeys[m.m1].value / monkeys[m.m2].value; m.has_value = true; break;
                    case '=': assert(monkeys[m.m1].value == monkeys[m.m2].value); cout << "RIGHT!" << endl; return;
                    default:
                        assert(false);
                    }

                    found_something = true;
                }
            }
        }

        if (!found_something)
            break;
    }

    Monkey &m = monkeys["root"];
    Monkey &left = monkeys[m.m1]; // needs to be 82091308111060 with two factors

    // find a path from root to humn; walk back up tree. At each node, one will be a consequence of "humn" and the other will be a known quantity.
    vector<Monkey> path;
    recur("root", path);
    path.push_back(monkeys["root"]);
    auto value = 0LL;

    for (int i = path.size() - 1; i > 0; i--)
    {
        auto &m = path[i];
        auto &m1 = monkeys[path[i].m1];
        auto &m2 = monkeys[path[i].m2];

        if (m1.has_value)
        {
            if (m.name == "root")
                value = m1.value;
            else if (m.op == '+')
                value -= m1.value;
            else if (m.op == '-')
                value = m1.value - value;
            else if (m.op == '*')
                value /= m1.value;
            else if (m.op == '/')
                value = m1.value / value;
        }
        else if (m2.has_value)
        {
            if (m.name == "root")
                value = m2.value;
            else if (m.op == '+')
                value -= m2.value;
            else if (m.op == '-')
                value += m2.value;
            else if (m.op == '*')
                value /= m2.value;
            else if (m.op == '/')
                value *= m2.value;
        }
        else
            assert(false);
    }

    cout << value << endl;
}

// For my input, the right side (m2) of root is always 82091308111060.
// So I need to make sure left side (m1) of root gets that value somehow

int main()
{
    //read_file_line_by_line("input.txt");

    //auto part1 = process_data("test.txt", false);
    //std::cout << "Part One: " << part1 << std::endl;

    figure_it_out();

    for (int humn = -10000; humn <= 10000; humn++)
    {
        if (do_part2("input.txt", humn))
        {
            cout << humn << endl;
            break;
        }
    }
    //std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 324122188240430);
    //assert(part2 == 3412650897405);
    return 0;
}
