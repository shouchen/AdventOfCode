#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>

struct Monkey
{
    std::vector<long long> items;
    char operation;
    std::string operand;
    long long divisible_by;
    int true_throw_to, false_throw_to;
};

auto process_input(const std::string &filename, bool divide_by_three, int rounds)
{
    std::ifstream file(filename);
    std::string monkey, starting, items_colon, s, new_str, old, test_colon, divisible, by, if_str, true_colon, false_colon, throw_str, to;
    auto id = 0;
    auto colon = ':', equals = '=';
    std::vector<Monkey> monkeys;

    while (file >> monkey >> id >> colon >> starting >> items_colon)
    {
        Monkey m;

        while (file >> s && s != "Operation:")
            m.items.push_back(atoi(s.c_str()));

        file >> new_str >> equals >> old >> m.operation >> m.operand;
        file >> test_colon >> divisible >> by >> m.divisible_by;
        file >> if_str >> true_colon >> throw_str >> to >> monkey >> m.true_throw_to;
        file >> if_str >> false_colon >> throw_str >> to >> monkey >> m.false_throw_to;

        monkeys.push_back(m);
    }

    auto modulo = 1LL;
    for (auto &m : monkeys)
        modulo *= m.divisible_by;

    std::map<int, long long> monkey_inspections;

    while (rounds--)
    {
        for (auto id = 0; id < monkeys.size(); id++)
        {
            Monkey &m = monkeys[id];

            for (auto i : m.items)
            {
                monkey_inspections[id]++;

                auto worry = i;
                auto operand = ((m.operand == "old") ? worry : atoi(m.operand.c_str()));
                worry = (m.operation == '*') ? (worry * operand) : (worry + operand);

                if (divide_by_three)
                    worry /= 3;

                worry %= modulo;

                auto throw_to = (worry % m.divisible_by) ? m.false_throw_to : m.true_throw_to;
                monkeys[throw_to].items.push_back(worry);
            }

            m.items.clear();
        }
    }

    std::vector<long long> counts;
    for (auto &i : monkey_inspections)
        counts.push_back(i.second);

    sort(counts.begin(), counts.end());
    return counts.back() * counts[counts.size() - 2];
}

int main()
{
    auto part1 = process_input("input.txt", true, 20);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_input("input.txt", false, 10000);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 95472);
    assert(part2 == 17926061332);
    return 0;
}
