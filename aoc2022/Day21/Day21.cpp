#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cassert>

struct Monkey
{
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
    std::string line, name, word;

    while (getline(file, line))
    {
        Monkey monkey;

        std::stringstream ss(line);
        ss >> name >> word;

        name.pop_back();

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

        monkeys[name] = monkey;
    }
}

void evaluate(const std::string &name)
{
    auto &m = monkeys[name];
    if (m.has_value || m.m1.empty() || m.m2.empty())
        return;

    evaluate(m.m1);
    evaluate(m.m2);

    auto &left = monkeys[m.m1], &right = monkeys[m.m2];

    if (left.has_value && right.has_value)
    {
        switch (m.op)
        {
        case '+': m.value = left.value + right.value; break;
        case '-': m.value = left.value - right.value; break;
        case '*': m.value = left.value * right.value; break;
        case '/': m.value = left.value / right.value; break;
        }
        m.has_value = true;
    }
}

void complete(const std::string &name, long long value = 0)
{
    auto &m = monkeys[name];
    if (m.has_value)
        return;

    auto &left = monkeys[m.m1], &right = monkeys[m.m2];

    if (left.has_value)
    {
        switch (m.op)
        {
        case '+': value -= left.value; break;
        case '-': value = left.value - value; break;
        case '*': value /= left.value; break;
        case '/': value = left.value / value; break;
        case '=': value = left.value; break;
        }
    }
    else
    {
        switch (m.op)
        {
        case '+': value -= right.value; break;
        case '-': value += right.value; break;
        case '*': value /= right.value; break;
        case '/': value *= right.value; break;
        case '=': value = right.value; break;
        }
    }

    m.value = value;
    m.has_value = true;

    complete(m.m1, value);
    complete(m.m2, value);
}

auto do_part1(const std::string &filename)
{
    read_input(filename);
    evaluate("root");
    return monkeys["root"].value;
}

auto do_part2(const std::string &filename)
{
    read_input(filename);

    monkeys["humn"].has_value = false;
    evaluate("root");

    monkeys["root"].op = '=';
    complete("root");

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
