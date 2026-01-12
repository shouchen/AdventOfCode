#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <ranges>
#include <algorithm>
#include <cassert>

auto find_min_button_presses1(unsigned goal, const std::vector<unsigned int> &buttons)
{
    struct Elem
    {
        unsigned state = 0x0;
        unsigned presses = 0;
    };

    std::set<unsigned> seen;

    std::queue<Elem> q;
    q.push(Elem());

    while (!q.empty())
    {
        auto elem = q.front(); q.pop();

        if (elem.state == goal)
            return elem.presses;

        if (seen.find(elem.state) != seen.end())
            continue;

        seen.insert(elem.state);

        for (auto button : buttons)
        {
            auto new_state = elem.state ^ button;
            q.push(Elem{ new_state, elem.presses + 1 });
        }
    }

    return 0U;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string word;
    auto comma = ',', parenth = '(', brace = '{', token = ' ';
    auto n = 0, num_indicators = 0;

    auto buttons = std::vector<unsigned int>{};
    auto joltages = std::vector<unsigned int>{};
    auto goal = 0U;
    unsigned retval = 0U;

    while (file >> word)
    {
        if (word.front() == '[')
        {
            goal = 0U;
            num_indicators = int(word.length()) - 2;
            for (auto c : word)
            {
                if (c == '#')
                    goal = (goal <<  1) | 0x1;
                if (c == '.')
                    goal <<= 1;
            }
        }
        else if (word.front() == '(')
        {
            auto button = 0U;
            std::stringstream ss(word);

            ss >> parenth;
            while (ss >> n)
            {
                auto temp = 0x1 << (num_indicators - 1 - n);
                button |= temp;
                ss >> token;
            }

            buttons.push_back(button);
        }
        else // word.front() == '{'
        {
            auto joltage = 0U;
            std::stringstream ss(word);

            ss >> brace;
            while (ss >> n)
            {
                joltages.push_back(n);
                ss >> token;
            }

            retval += find_min_button_presses1(goal, buttons);

            buttons.clear();
            joltages.clear();
        }
    }

    return retval;
}

////////////////////////////////////////////////////////////////////////////////
// PART 2
////////////////////////////////////////////////////////////////////////////////

// implements an approach like the one suggested here:
// https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/

struct cache_hash
{
    auto operator() (const std::vector<int> &v) const
    {
        auto retval = 0;
        for (auto i : v) retval = retval * 100 + i;
        return retval;
    }
};

struct parity_hash
{
    auto operator() (const std::vector<int> &v) const
    {
        auto retval = 0;
        for (auto i : v) retval = (retval << 1) + (i & 0x1);
        return retval;
    }
};

using Button = std::vector<int>;
using Buttons = std::vector<Button>;
using Joltages = std::vector<int>;

using ParityMap = std::vector<int>;
using ParityMaps = std::unordered_map<ParityMap, std::unordered_map<std::vector<int>, int, cache_hash>, parity_hash>;
using Cache = std::unordered_map<std::vector<int>, std::tuple<bool, int>, cache_hash>;

auto compute_parity_maps(int num_counters, const Buttons &buttons)
{
    ParityMaps parity_maps;

    // loop over all possible button press combos
    for (auto i = 0; i < (1 << buttons.size()); i++)
    {
        std::bitset<16> bits(i);
        auto result = std::vector<int>(num_counters, 0);

        // result will hold the number of times each indicator is affected by this button press combo
        for (auto j = 0; j < buttons.size(); j++)
            if (bits[j])
                for (auto k : buttons[j])
                    result[k]++;

        // parity_map is a vector that holds 0 or 1 for each button in this combo - 0 means an even number of button presses
        // which wouldn't change the parity of any indicator; 1 means an odd number of button presses which would parities
        ParityMap parity_map = result |
            std::views::transform([](auto x) {return x & 0x1; }) |
            std::ranges::to<std::vector<int>>();

        // parity_maps will contain the minimum number of button presses to get this parity_map result
        auto num_button_presses = int(bits.count());
        if (!parity_maps[parity_map].contains(result) || num_button_presses < parity_maps[parity_map][result])
            parity_maps[parity_map][result] = num_button_presses;
    }

    return parity_maps;
}

auto find_min_button_presses2(const Joltages &joltages, const Buttons &buttons, const ParityMaps &parity_maps, Cache &cache)
{
    if (cache.contains(joltages))
        return cache[joltages];

    if (std::ranges::all_of(joltages, [](const auto x) {return x == 0; }))
        return cache[joltages] = { true, 0 };

    if (std::ranges::any_of(joltages, [](const auto x) {return x < 0; }))
        return cache[joltages] = { false, 0 }; // overshot goal

    Joltages current_parity;
    for (const auto c : joltages)
        current_parity.push_back(c % 2);

    if (!parity_maps.contains(current_parity))
        return cache[joltages] = { false, 0 };

    auto min_flips = INT_MAX;
    for (const auto &[pattern, num_flips] : parity_maps.at(current_parity))
    {
        // check validity
        if (pattern > joltages)
            continue;

        // subtract pattern from joltages
        auto next_joltages = joltages;
        for (auto i = 0; i < pattern.size(); i++)
            next_joltages[i] -= pattern[i];

        // after subtracting matching parity pattern, result is all even - halve and recurse
        for (auto &i : next_joltages)
            i >>= 1;

        const auto [reached_goal, count] = find_min_button_presses2(next_joltages, buttons, parity_maps, cache);
        if (reached_goal)
            min_flips = std::min(min_flips, num_flips + 2 * count);
    }

    return cache[joltages] = (min_flips == INT_MAX)
        ? std::make_pair(false, 0) : std::make_pair(true, min_flips);
};

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string word;
    auto n = 0, total = 0;
    auto parenth = '(', comma_or_close_parenth = ',', comma_or_close_brace = '}';;

    std::string lights;
    Buttons buttons;
    std::vector<int> joltages;

    while (file >> word)
    {
        if (word.front() == '[')
        {
            lights = word.substr(1, word.length() - 2);
            buttons.clear();
            joltages.clear();
        }
        else if (word.front() == parenth)
        {
            std::stringstream ss(&word[1]);
            buttons.push_back(Button());

            while (ss >> n >> comma_or_close_parenth)
                buttons.back().push_back(n);
        }
        else // word.front() == '{'
        {
            std::stringstream ss(&word[1]);

            while (ss >> n >> comma_or_close_brace)
                joltages.push_back(n);

            // Solve for this machine (input line)
            Cache cache;
            const auto parity_maps = compute_parity_maps(int(joltages.size()), buttons);
            const auto [reached_goal, count] = find_min_button_presses2(joltages, buttons, parity_maps, cache);
            total += count;
        }
    }

    return total;
}

int main(int argc, char *argv[])
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 449);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 17848);

    return 0;
}
