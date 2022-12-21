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
    monkeys.clear();
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

void evaluate(const std::string &name)
{
    auto &m = monkeys[name];
    if (m.has_value)
        return;

    // Part2 exception: This one has no children, nor known value
    if (name == "humn")
        return;

    auto &left_monkey = monkeys[m.m1], &right_monkey = monkeys[m.m2];
    evaluate(m.m1); evaluate(m.m2);

    if (!left_monkey.has_value || !right_monkey.has_value)
        return;

    switch (m.op)
    {
    case '+': m.value = left_monkey.value + right_monkey.value; break;
    case '-': m.value = left_monkey.value - right_monkey.value; break;
    case '*': m.value = left_monkey.value * right_monkey.value; break;
    case '/': m.value = left_monkey.value / right_monkey.value; break;
    }
    m.has_value = true;
}

auto do_part1(const std::string &filename)
{
    read_input(filename);
    evaluate("root");
    return monkeys["root"].value;
}

void complete(const std::string &name, long long value)
{
    auto &m = monkeys[name];

    if (name == "humn")
    {
        m.value = value;
        m.has_value = true;
        return;
    }

    auto &left = monkeys[m.m1], &right = monkeys[m.m2];
    if (name != "root")
    {
        m.value = value;
        m.has_value = true;

        if (left.has_value)
        {
            if (m.op == '+') value -= left.value;
            else if (m.op == '-') value = left.value - value;
            else if (m.op == '*') value /= left.value;
            else if (m.op == '/') value = left.value / value;
        }
        else if (right.has_value)
        {
            if (m.op == '+') value -= right.value;
            else if (m.op == '-') value += right.value;
            else if (m.op == '*') value /= right.value;
            else if (m.op == '/') value *= right.value;
        }
    }

    if (!left.has_value) complete(m.m1, value);
    if (!right.has_value) complete(m.m2, value);

    switch (m.op)
    {
    case '+': m.value = left.value + right.value; break;
    case '-': m.value = left.value - right.value; break;
    case '*': m.value = left.value * right.value; break;
    case '/': m.value = left.value / right.value; break;
    }
    m.has_value = true;
}


auto do_part2(const std::string &filename)
{
    read_input(filename);

    // Alter the input
    auto &humn = monkeys["humn"];
    humn.has_value = false;
    evaluate("root");

    auto &root = monkeys["root"];
    auto value = monkeys[root.m1].has_value ? monkeys[root.m1].value : monkeys[root.m2].value;
    complete("root", value);

    return monkeys["humn"].value;
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
