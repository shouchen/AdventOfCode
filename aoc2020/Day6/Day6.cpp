#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto retval = std::make_pair(0LL, 0LL);

    while (!file.eof())
    {
        std::vector<std::string> group;
        std::map<char, int> letter_count;
        std::string s;

        while (std::getline(file, s) && !s.empty())
        {
            group.push_back(s);
            for (auto c : s)
                letter_count[c]++;
        }

        retval.first += letter_count.size();

        for (auto c = 'a'; c <= 'z'; c++)
            if (letter_count[c] == group.size())
                retval.second++;
    }

    return retval;
}

int main()
{
    auto result = process_input("input.txt");
    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 6170);
    assert(result.second == 2947);
    return 0;
}
