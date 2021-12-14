#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <climits>
#include <algorithm>
#include <cassert>

using LetterPair = std::pair<char, char>;
using PairCounts = std::map<LetterPair, long long>;
using LetterCounts = std::map<char, long long>;
using Mappings = std::vector<std::pair<LetterPair, char>>;

void do_step(PairCounts &pair_counts, Mappings &mappings)
{
    PairCounts pairs_added_in_this_step;

    for (auto &mapping : mappings)
    {
        auto from = mapping.first;
        auto count_iter = pair_counts.find(from);

        if (count_iter != pair_counts.end())
        {
            auto left = from.first, right = from.second, center = mapping.second;
            pairs_added_in_this_step[std::make_pair(left, center)] += count_iter->second;
            pairs_added_in_this_step[std::make_pair(center, right)] += count_iter->second;
            pair_counts.erase(count_iter);
        }
    }

    for (auto &added_pair : pairs_added_in_this_step)
        pair_counts[added_pair.first] += added_pair.second;
}

auto get_max_min_diff(PairCounts &pair_counts, const std::string &polymer)
{
    LetterCounts letter_counts_x2;

    for (auto &pc : pair_counts)
    {
        letter_counts_x2[pc.first.first] += pc.second;
        letter_counts_x2[pc.first.second] += pc.second;
    }

    // every letter got counted twice except front/back - add these again before halving
    letter_counts_x2[polymer.front()]++;
    letter_counts_x2[polymer.back()]++;

    auto max = LLONG_MIN, min = LLONG_MAX;
    for (auto &lc_x2 : letter_counts_x2)
    {
        if (lc_x2.second > max) max = lc_x2.second;
        if (lc_x2.second < min) min = lc_x2.second;
    }

    return (max - min) / 2;
}

auto process_data(const std::string &filename)
{
    std::ifstream file("input.txt");
    std::string polymer, arrow;
    auto from1 = ' ', from2 = ' ', to = ' ';
    Mappings mappings;

    file >> polymer;
    while (file >> from1 >> from2 >> arrow >> to)
        mappings.push_back(std::make_pair(std::make_pair(from1, from2), to));

    PairCounts pair_counts;
    for (auto i = 0; i < polymer.length() - 1; i++)
        pair_counts[std::make_pair(polymer[i], polymer[i + 1])]++;

    for (auto i = 0; i < 10; i++)
        do_step(pair_counts, mappings);

    auto after10 = get_max_min_diff(pair_counts, polymer);

    for (auto i = 0; i < 30; i++)
        do_step(pair_counts, mappings);

    return std::make_pair(after10, get_max_min_diff(pair_counts, polymer));
}

int main()
{
    auto result = process_data("input.txt");

    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;

    assert(result.first == 3906);
    assert(result.second == 4441317262452);
    return 0;
}
