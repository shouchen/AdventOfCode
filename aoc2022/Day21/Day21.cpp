#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cassert>

struct Monkey
{
    std::string name;
    bool has_value;
    long long value;
    std::string m1, m2;
    char op;
};

std::map<std::string, Monkey> monkeys;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, word;

    while (getline(file, line))
    {
        Monkey monkey;

        std::stringstream ss(line);
        ss >> monkey.name >> word;

        monkey.name.pop_back(); // no colon

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
}

void evaluate()
{
    for (;;)
    {
        auto found_something = false;
        for (auto &i : monkeys)
        {
            auto &m = i.second;
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
                    }

                    found_something = true;
                }
            }
        }

        if (!found_something)
            break;
    }
}

auto do_part1(const std::string &filename)
{
    monkeys.clear();
    read_input(filename);

    // TODO: This could be simpler as a recursive evaluation
    evaluate();

    return monkeys["root"].value;
}

bool recur(const std::string &name, std::vector<Monkey> &path)
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

auto do_part2(const std::string &filename)
{
    monkeys.clear();
    read_input(filename);

    auto &humn = monkeys["humn"];
    humn.has_value = false;
    humn.m1 = "xqrt"; // unused name to ensure doesn't resolve
    humn.m2 = "trwz"; // unused name to ensure doesn't resolve

    evaluate();

    // find a path from root to humn; walk back up tree. At each node, one will be a consequence of "humn" and the other will be a known quantity.
    std::vector<Monkey> path;
    recur("root", path);
    path.push_back(monkeys["root"]);
    auto value = 0LL;

    for (auto i = path.size() - 1; i > 0; i--)
    {
        auto &m = path[i];
        auto &left = monkeys[path[i].m1];
        auto &right = monkeys[path[i].m2];

        if (left.has_value)
        {
            if (m.name == "root") value = left.value;
            else if (m.op == '+') value -= left.value;
            else if (m.op == '-') value = left.value - value;
            else if (m.op == '*') value /= left.value;
            else if (m.op == '/') value = left.value / value;
        }
        else if (right.has_value)
        {
            if (m.name == "root") value = right.value;
            else if (m.op == '+') value -= right.value;
            else if (m.op == '-') value += right.value;
            else if (m.op == '*') value /= right.value;
            else if (m.op == '/') value *= right.value;
        }
    }

    return value;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 324122188240430);
    assert(part2 == 3412650897405);
    return 0;
}
