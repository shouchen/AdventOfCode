#include "stdafx.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

int do_stuff(const std::string &filename, bool anagrams)
{
    int valid = 0;

    std::ifstream f(filename);
    std::string input;

    while (std::getline(f, input))
    {
        std::stringstream ss(input);

        std::vector<std::string> words;
        std::string word;
        while (ss >> word)
        {
            if (anagrams) std::sort(word.begin(), word.end());
            words.push_back(word);
        }

        bool dupe = false;
        for (auto i = 0U; i < words.size(); i++)
        {
            std::sort(words.begin(), words.end());
            for (auto j = i + 1; j < words.size(); j++)
            {
                if (strcmp(words[i].c_str(), words[j].c_str()) == 0)
                {
                    dupe = true;
                    break;
                }
            }
        }

        if (!dupe) valid++;
    }

    return valid;
}

int main()
{
    assert(do_stuff("input-test.txt", false) == 2);
    assert(do_stuff("input-test2.txt", true) == 3);

    auto part1 = do_stuff("input.txt", false);
    auto part2 = do_stuff("input.txt", true);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 337);
    assert(part2 == 231);
    return 0;
}
