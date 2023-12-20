#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <map>
#include <numeric>
#include <cassert>

struct Mod
{
    enum Type { FlipFlop, Conjunction, Broadcaster } type;
    std::string name;
    std::vector<std::string> next;
    bool ff_on = false;
    std::map<std::string, int> remember;
    std::map<std::string, int> first_high;
};

std::map<std::string, Mod> mods;

void read_input_lines(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, name, arrow;

    while (std::getline(file, line))
    {
        std::istringstream is(line);
        is >> name >> arrow;

        auto type = Mod::Type::Broadcaster;

        if (name != "broadcaster")
        {
            type = (name.front() == '%') ? Mod::Type::FlipFlop : Mod::Type::Conjunction;
            name = name.substr(1);
        }

        auto mod = Mod{ type, name };

        std::string name;
        while (is >> name)
        {
            if (name.back() == ',') name.pop_back();
            mod.next.push_back(name);
        }

        mods[mod.name] = mod;
    }

    for (auto &i : mods)
        for (auto &j : i.second.next)
            mods[j].remember[i.second.name] = 0;
}

struct Elem
{
    std::string src, dest;
    int pulse;
};

auto count_high = 0LL, count_low = 0LL, button_presses = 0LL;

void press_button()
{
    button_presses++;

    std::queue<Elem> q;
    q.push(Elem{ "button", "broadcaster", 0 });

    while (!q.empty())
    {
        auto elem = q.front(); q.pop();

        elem.pulse ? count_high++ : count_low++;
        auto &mod = mods[elem.dest];

        if (elem.dest == "ll" && elem.pulse == 1)
            if (mod.first_high.find(elem.src) == mod.first_high.end())
                mod.first_high[elem.src] = button_presses;

        switch (mod.type)
        {
        case Mod::Type::Broadcaster:
            for (auto &a : mod.next)
                q.push(Elem{ mod.name, a, elem.pulse });
            break;

        case Mod::Type::FlipFlop:
            if (elem.pulse == 0)
            {
                mod.ff_on = !mod.ff_on;
                auto pulse = mod.ff_on ? 1 : 0;
                for (auto &a : mod.next)
                    q.push(Elem{ mod.name, a, pulse });
            }
            break;

        case Mod::Type::Conjunction:
            mod.remember[elem.src] = elem.pulse;
            bool all_high = true;
            for (auto &i : mod.remember)
                if (i.second != 1)
                {
                    all_high = false;
                    break;
                }

            for (auto &a : mod.next)
                q.push(Elem{ mod.name, a, all_high ? 0 : 1 });
            break;
        }
    }
}

auto do_part1()
{
    for (auto i = 0; i < 1000; i++)
        press_button();
 
    return count_low * count_high;
}

auto do_part2()
{
    // Observing the configuration of the input data, rx is fed only by ll, which is a conjunction module. It,
    // in turn, is fed by four modules (kl, vm, kv, and vb). Looking at the input and the debug output as the
    // program runs, it's evident that these are counters that hit 1 periodically. The answer for part 2 will
    // be the lcm of all of these since that would cause "rx" to receive a low signal for the first time.

    while (mods["ll"].first_high.size() < mods["ll"].remember.size())
        press_button();

    auto retval = 1LL;
    for (auto i : mods["ll"].first_high)
        retval = std::lcm(retval, i.second);
    return retval;
}

int main()
{
    read_input_lines("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 743090292);
    assert(part2 == 241528184647003);
    return 0;
}
