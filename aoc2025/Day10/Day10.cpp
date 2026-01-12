#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <ranges>
#include <algorithm>
#include <cassert>

// Note: part 2 implements an approach like the one suggested here:
// https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/

using Button = std::vector<int>;
using Buttons = std::vector<Button>;
using Joltages = std::vector<int>;

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

auto find_min_button_presses1(const std::string &goal, const Buttons &buttons)
{
    struct State
    {
        std::string current_state;
        int s_id, depth;
    };

    std::queue<State> q;
    std::unordered_set<std::string> seen;

    for (auto i = 0; i < buttons.size(); i++)
        q.push({ std::string(goal.size(), '.'), i, 1 });

    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();

        for (const auto i : buttons[curr.s_id])
            curr.current_state[i] = (curr.current_state[i] == '.' ? '#' : '.');

        if (curr.current_state == goal)
            return curr.depth;

        if (seen.contains(curr.current_state)) continue;
        seen.insert(curr.current_state);

        for (auto i = 0; i < buttons.size(); i++)
            q.push({ curr.current_state, i, curr.depth + 1 });
    }

    return -1;
};

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

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string word;
    auto n = 0;
    auto retval = std::make_pair(0, 0);
    auto open_parenth = '(', open_bracket = '[', comma_or_close_parenth = ')', comma_or_close_brace = '}';

    std::string lights;
    Buttons buttons;
    Joltages joltages;

    while (file >> word)
    {
        if (word.front() == open_bracket)
        {
            lights = word.substr(1, word.length() - 2);
            buttons.clear();
            joltages.clear();
        }
        else if (word.front() == open_parenth)
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
            retval.first += find_min_button_presses1(lights, buttons);

            Cache cache;
            const auto parity_maps = compute_parity_maps(int(joltages.size()), buttons);
            const auto [reached_goal, count] = find_min_button_presses2(joltages, buttons, parity_maps, cache);
            retval.second += count;
        }
    }

    return retval;
}

int main(int argc, char *argv[])
{
    auto solution = solve("input.txt");
    std::cout << "Part One: " << solution.first << std::endl;
    std::cout << "Part Two: " << solution.second << std::endl;

    assert(solution.first == 449);
    assert(solution.second == 17848);
    return 0;
}
