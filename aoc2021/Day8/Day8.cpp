#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cassert>

auto segments_to_digit(std::string &s, std::map<char, char> &m)
{
    static const std::vector<std::string> segments =
    {
        "abcefg", "cf", "acdeg", "acdfg", "bcdf",
        "abdfg", "abdefg", "acf", "abcdefg", "abcdfg"
    };

    std::string s2;
    for (auto c : s)
        s2.push_back(m[c]);

    sort(s2.begin(), s2.end());

    for (int i = 0; i < 10; i++)
        if (s2 == segments[i])
            return i;

    return -1;
}

auto num_words_containing_letter(std::vector<std::string> &words, char c)
{
    return std::count_if(words.begin(), words.end(),
        [c](auto& w) { return w.find(c) != std::string::npos; });
}

auto num_words_containing_letters(std::vector<std::string> &words, char c1, char c2)
{
    return std::count_if(words.begin(), words.end(),
        [c1, c2](auto &w){ return w.find(c1) != std::string::npos && w.find(c2) != std::string::npos; });
}

auto find_word_with_letter_count(std::vector<std::string> &words, int n)
{
    return *std::find_if(words.begin(), words.end(), [n](auto &w) { return w.length() == n; });
}

auto part1_word_count(std::vector<std::string> &words)
{
    return (unsigned)std::count_if(words.begin(), words.end(),
        [](auto& w) { return w.length() == 2 || w.length() == 3 || w.length() == 4 || w.length() == 7; });
}

auto read_words_into_vector_with_sorted_letters(const std::string &s)
{
    auto ss = std::istrstream(s.c_str());
    std::string word;
    std::vector<std::string> retval;

    while (ss >> word)
    {
        sort(word.begin(), word.end());
        retval.push_back(word);
    }

    return retval;
}

auto process_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, leftstr, rightstr;
    auto part1 = 0, part2 = 0;

    while (std::getline(file, leftstr, '|') && std::getline(file, rightstr))
    {
        auto left = read_words_into_vector_with_sorted_letters(leftstr);
        auto right = read_words_into_vector_with_sorted_letters(rightstr);

        part1 += part1_word_count(right);

        std::map<char, char> puzzle_letter_to_real; 
        auto maps_to_f = '-', maps_to_c = '-', maps_to_a = '-';

        // if 4, 6, or 9 occurrences for a letter, we know it maps to e, b, or f, respectively
        for (auto c = 'a'; c <= 'g'; c++)
        {
            switch (num_words_containing_letter(left, c))
            {
                case 4: puzzle_letter_to_real[c] = 'e'; break;
                case 6: puzzle_letter_to_real[c] = 'b'; break;
                case 9: puzzle_letter_to_real[c] = 'f'; maps_to_f = c; break;
            }
        }

        // the lone two-letter word is cf and we know what maps to f, so the other one maps to c
        auto w = find_word_with_letter_count(left, 2);
        maps_to_c = (w[0] == maps_to_f) ? w[1] : w[0];
        puzzle_letter_to_real[maps_to_c] = 'c';

        // the lone three-letter word is acf, and we know c and f, so the other is a
        w = find_word_with_letter_count(left, 3);
        if (w[0] != maps_to_f && w[0] != maps_to_c)
            maps_to_a = w[0];
        else if (w[1] != maps_to_f && w[1] != maps_to_c)
            maps_to_a = w[1];
        else if (w[2] != maps_to_f && w[2] != maps_to_c)
            maps_to_a = w[2];
        puzzle_letter_to_real[maps_to_a] = 'a';

        // now, just d and g remain:
        // d occurs 6 times when a is in the same word
        // g occurs 7 times when a is in the same word
        for (auto c = 'a'; c <= 'g'; c++)
            if (puzzle_letter_to_real.find(c) == puzzle_letter_to_real.end())
                puzzle_letter_to_real[c] = (num_words_containing_letters(left, maps_to_a, c) == 6) ? 'd' : 'g';
           
        // map each string to a digit
        std::map<std::string, int> r;
        for (auto i = 0; i < 10; i++)
            r[left[i]] = segments_to_digit(left[i], puzzle_letter_to_real);

        auto amount = r[right[0]] * 1000 + r[right[1]] * 100 + r[right[2]] * 10 + r[right[3]];
        part2 += amount;
    }

    return std::make_pair(part1, part2);
}

int main()
{
    auto result = process_data("input.txt");
    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 362);
    assert(result.second == 1020159);
    return 0;
}
