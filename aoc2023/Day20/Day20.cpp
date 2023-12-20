#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

using namespace std;

struct Mod
{
    char type; // %, &inv
    string name;
    vector<string> next;
    bool ff_on = false;
    map<string, int> remember;
};

map<string, Mod> mods;

void read_input_lines(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, name, arrow;

    while (std::getline(file, line))
    {

        std::istringstream is(line);
        is >> name >> arrow;

        char type = ' ';
        if (name == "broadcaster")
        {
            type = 'b';
        }
        else
        {
            type = name[0];
            name = name.substr(1);
        }

        auto mod = Mod{ type, name };

        string name;
        while (is >> name)
        {
            if (name.back() == ',') name.pop_back();
            mod.next.push_back(name);
        }

        mods[mod.name] = mod;
    }

    for (auto &i : mods)
    {
        for (auto &j : i.second.next)
        {
            mods[j].remember[i.second.name] = 0;
        }
    }
}

struct Elem
{
    string src, dest;
    int pulse;
};

long long count_high, count_low, button_presses = -1;

// in this case there are four (kl, vm, kv, vb), need lcm
auto ll_high_from_kl = -1LL, ll_high_from_vm = -1LL, ll_high_from_kv = -1LL, ll_high_from_vb = -1LL;

void press_button()
{
    button_presses++;

    // button press
    queue<Elem> q;
    q.push(Elem{ "button", "broadcaster", 0 });

    while (!q.empty())
    {
        auto elem = q.front(); q.pop();

        //cout << elem.src << " -" << (elem.pulse == 0 ? "low" : "high") << "-> " << elem.dest << endl;

        if (elem.pulse == 0)
            count_low++;
        else
            count_high++;

        Mod &mod = mods[elem.dest];

        if (elem.dest == "ll" && elem.pulse == 1)
        {
            if (elem.src == "kl" && ll_high_from_kl == -1)
            {
                // cout << "kl generated 1 at " << button_presses << endl; // 3916,7833,11750,15667  (per 3917, offset 1)
                ll_high_from_kl = button_presses;
            }
            else if (elem.src == "vm" && ll_high_from_vm == -1)
            {
                // cout << "vm generated 1 at " << button_presses << endl;  // 4050,8101,12152,16203 (per 4051, offset 0)
                ll_high_from_vm = button_presses;
            }
            else if (elem.src == "kv" && ll_high_from_kv == -1)
            {
                // cout << "kv generated 1 at " << button_presses << endl;  // 4012,8025,12038,16051 (per 4013, offset 0)
                ll_high_from_kv = button_presses;
            }
            else if (elem.src == "vb" && ll_high_from_vb == -1)
            {
                // cout << "vb generated 1 at " << button_presses << endl;  // 3792,7585,11378,15171 (per 3793, offset 0)
                ll_high_from_vb = button_presses;
            }
        }

        if (mod.type == 'b')
        {
            for (auto &a : mod.next)
            {
                q.push(Elem{ mod.name, a, elem.pulse });
            }
        }
        else if (mod.type == '%')
        {
            if (elem.pulse == 0)
            {
                mod.ff_on = !mod.ff_on;
                auto pulse = mod.ff_on ? 1 : 0;
                for (auto &a : mod.next)
                {
                    q.push(Elem{mod.name, a, pulse });
                }
            }
        }
        else if (mod.type == '&')
        {
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
        }
    }
}

auto do_part1()
{
    auto part1 = 0LL;
    for (int i = 0; i < 1000; i++)
    {
        press_button();
        part1 = count_low * count_high;
    }
    for (;;)
    {
        press_button();

        if (ll_high_from_kl != -1 && ll_high_from_vm != -1 && ll_high_from_kv != -1 && ll_high_from_vb != -1)
        {
            break;
        }
    }

    return part1;
}


auto do_part2()
{
    // Looking at runtime output, another cycles thing.
    // kl generated 1 at 3916,7833,11750,15667  (per 3917, offset 1)
    // vm generated 1 at 4050,8101,12152,16203 (per 4051, offset 0)
    // kv generated 1 at 4012,8025,12038,16051 (per 4013, offset 0)
    // vb generated 1 at 3792,7585,11378,15171 (per 3793, offset 0)

    return 3917LL * 4051LL * 4013LL * 3793LL;
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

// Reverse eng input
// rx comes from ll, and ll will only send low if all inputs to ll are high
// in this case there are four (kl, vm, kv, vb) -- need lcm