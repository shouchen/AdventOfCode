#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
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

using namespace std;

struct Monkey
{
    int id;
    vector<long long> items;
    char operation;
    string operand;
    long long divisible_by;
    int true_throw_to, false_throw_to;
};

auto do_part1(const std::string &filename)
{
    vector<Monkey> monkeys;
    map<int, long long> monkey_count;

    std::ifstream file(filename);
    std::string monkey_str, starting, items_colon, temp, new_str, old, test_colon, if_str, true_colon, false_colon, throw_str, to, divisible, by;
    int id;
    char colon, equals;

    while (file >> monkey_str >> id >> colon)
    {
        Monkey monkey{ id };

        file >> starting >> items_colon;
        while (file >> temp)
        {
            if (temp == "Operation:")
                break;

            monkey.items.push_back(atoi(temp.c_str()));
        }

        file >> new_str >> equals >> old >> monkey.operation >> monkey.operand;
        file >> test_colon >> divisible >> by >> monkey.divisible_by;
        file >> if_str >> true_colon >> throw_str >> to >> monkey_str >> monkey.true_throw_to;
        file >> if_str >> false_colon >> throw_str >> to >> monkey_str >> monkey.false_throw_to;

        monkeys.push_back(monkey);
    }

    for (int round = 1; round <= 20; round++)
    {
        for (Monkey &m : monkeys)
        {
            for (auto &i : m.items)
            {
                monkey_count[m.id]++;

                auto temp = i;
                if (m.operation == '*')
                    temp *= ((m.operand == "old") ? temp : atoi(m.operand.c_str()));
                else if (m.operation == '+')
                    temp += ((m.operand == "old") ? temp : atoi(m.operand.c_str()));
                else
                    assert(false);

                temp /= 3;
                if ((temp % m.divisible_by) == 0)
                    monkeys[m.true_throw_to].items.push_back(temp);
                else
                    monkeys[m.false_throw_to].items.push_back(temp);
            }

            m.items.clear();
        }
    }

    vector<long long> counts;
    for (auto &i : monkey_count)
        counts.push_back(i.second);

    sort(counts.begin(), counts.end());

    auto part1 = counts[counts.size() - 1] * counts[counts.size() - 2];
    return part1;
}

auto do_part2(const std::string &filename)
{
    vector<Monkey> monkeys;
    map<int, long long> monkey_count;

    std::ifstream file(filename);
    std::string monkey_str, starting, items_colon, temp, new_str, old, test_colon, if_str, true_colon, false_colon, throw_str, to, divisible, by;
    int id;
    char colon, equals;

    while (file >> monkey_str >> id >> colon)
    {
        Monkey monkey{ id };

        file >> starting >> items_colon;
        while (file >> temp)
        {
            if (temp == "Operation:")
                break;

            monkey.items.push_back(atoi(temp.c_str()));
        }

        file >> new_str >> equals >> old >> monkey.operation >> monkey.operand;
        file >> test_colon >> divisible >> by >> monkey.divisible_by;
        file >> if_str >> true_colon >> throw_str >> to >> monkey_str >> monkey.true_throw_to;
        file >> if_str >> false_colon >> throw_str >> to >> monkey_str >> monkey.false_throw_to;

        monkeys.push_back(monkey);
    }

    long long modulo = 1;
    for (Monkey &m : monkeys)
        modulo *= m.divisible_by;

    for (int round = 1; round <= 10000; round++)
    {
        for (Monkey &m : monkeys)
        {
            for (auto &i : m.items)
            {
                monkey_count[m.id]++;

                auto temp = i;
                if (m.operation == '*')
                    temp *= ((m.operand == "old") ? temp : atoi(m.operand.c_str()));
                else if (m.operation == '+')
                    temp += ((m.operand == "old") ? temp : atoi(m.operand.c_str()));
                else
                    assert(false);

                temp %= modulo;

                if ((temp % m.divisible_by) == 0)
                    monkeys[m.true_throw_to].items.push_back(temp);
                else
                    monkeys[m.false_throw_to].items.push_back(temp);
            }

            m.items.clear();
        }
    }

    vector<long long> counts;
    for (auto &i : monkey_count)
        counts.push_back(i.second);

    sort(counts.begin(), counts.end());

    auto part2 = counts[counts.size() - 1] * counts[counts.size() - 2];
    return part2;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 95472);
    assert(part2 == 17926061332);
    return 0;
}
