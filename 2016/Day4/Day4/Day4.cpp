#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <cassert>

class mycomparison
{
public:
    mycomparison() {}

    bool operator()(const std::pair<char, unsigned> &lhs, const std::pair<char, unsigned> &rhs) const
    {
        if (lhs.second < rhs.second) return true;
        if (lhs.second > rhs.second) return false;

        return (lhs.first > rhs.first);
    }
};

std::string decrypt(const std::string &name, unsigned rots)
{
    std::string retval;

    for (char c : name)
    {
        if (c == '-') retval.push_back(' ');
        else retval.push_back((c - 'a' + rots) % 26 + 'a');
    }

    return retval;
}

unsigned return_sector_if_real(const std::string &room)
{
    // Parse
    auto digit = room.find_first_of("0123456789");
    auto lbracket = room.find("["), rbracket = room.find("]");

    auto name = room.substr(0, digit - 1);
    auto sector = atoi(room.substr(digit, lbracket - digit).c_str());
    auto checksum = room.substr(lbracket + 1, rbracket - lbracket - 1);

    // Validate
    unsigned count[26];
    for (auto i = 0; i < 26; i++) count[i] = 0;

    for (auto c : name)
    {
        if (c != '-')
            count[c - 'a']++;
    }

    // Heapify the collected data
    std::priority_queue<std::pair<char, unsigned>, std::vector<std::pair<char, unsigned>>, mycomparison> heap;

    for (auto i = 0; i < 26; i++)
    {
        auto pair = std::pair<char, unsigned>((char)('a' + i), count[i]);
        heap.push(pair);
    }

    std::string temp;

    // Pull out the top five
    for (auto i = 0; i < 5; i++)
    {
        temp += heap.top().first;
        heap.pop();
    }

    if (checksum != temp) return 0;

    // Decrypted list of names and their sectors. For Part Two, visually look for
    // "northpole object storage" in the output (since this string wasn't exactly
    // specified in the problem).
    std::cout << decrypt(name, sector) << " (" << sector << ")" << std::endl;

    return sector;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    unsigned total = 0;

    while (f >> line)
    {
        total += return_sector_if_real(line);
    }

    std::cout << std::endl << "Part One: " << total << std::endl;

    assert(total == 158835);
    return 0;
}
