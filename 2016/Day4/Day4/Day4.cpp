#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <cassert>

std::string Decrypt(const std::string &name, unsigned rots)
{
    std::string retval;

    for (char c : name)
        retval.push_back((c == '-') ? ' ' : ((c - 'a' + rots) % 26 + 'a'));

    return retval;
}

void Parse(const std::string &room, std::string &encryptedName, unsigned &sector, std::string &checksum)
{
    auto digit = room.find_first_of("0123456789");
    auto lbracket = room.find("["), rbracket = room.find("]");

    encryptedName = room.substr(0, digit - 1);
    sector = atoi(room.substr(digit, lbracket - digit).c_str());
    checksum = room.substr(lbracket + 1, rbracket - lbracket - 1);
}

bool IsValid(const std::string encryptedName, const std::string checksum)
{
    class MyComparison
    {
    public:
        bool operator()(const std::pair<char, unsigned> &lhs, const std::pair<char, unsigned> &rhs) const
        {
            if (lhs.second < rhs.second) return true;
            if (lhs.second > rhs.second) return false;
            return (lhs.first > rhs.first);
        }
    };

    unsigned count[26];
    for (auto &c : count) c = 0;

    for (auto c : encryptedName)
        if (c != '-')
            count[c - 'a']++;

    // Heapify the collected data
    std::priority_queue<std::pair<char, unsigned>, std::vector<std::pair<char, unsigned>>, MyComparison> heap;
    for (auto i = 0; i < 26; i++)
    {
        auto pair = std::pair<char, unsigned>((char)('a' + i), count[i]);
        heap.push(pair);
    }

    // The top five should match the checksum digits.
    for (auto i = 0; i < 5; i++)
    {
        if (checksum[i] != heap.top().first)
            return false;

        heap.pop();
    }

    return true;
}

bool ProcessRoom(const std::string &room, unsigned &sector, std::string &name)
{
    std::string encryptedName, checksum;
    Parse(room, encryptedName, sector, checksum);

    if (!IsValid(encryptedName, checksum))
        return false;

    name = Decrypt(encryptedName, sector);
    return true;
}

int main()
{
    std::ifstream f("input.txt");
    std::string room, name;
    auto total = 0U, sector = 0U, northpoleObjectStorage = 0U;

    while (f >> room)
    {
        if (ProcessRoom(room, sector, name))
        {
            total += sector;
            std::cout << name << " (" << sector << ")" << std::endl;

            // Note: Exact name not specified in the problem, but found by perusing
            // the "decrypted" names in the above program output.
            if (name == "northpole object storage")
                northpoleObjectStorage = sector;
        }
    }

    std::cout << std::endl << "Part One: " << total << std::endl;
    std::cout << "Part Two: " << northpoleObjectStorage << std::endl;

    assert(total == 158835);
    assert(northpoleObjectStorage == 993);
    return 0;
}
