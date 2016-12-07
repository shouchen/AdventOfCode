#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <cassert>

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

void parse(const std::string &room, std::string &encrypted_name, unsigned &sector, std::string &checksum)
{
    auto digit = room.find_first_of("0123456789");
    auto lbracket = room.find("["), rbracket = room.find("]");

    encrypted_name = room.substr(0, digit - 1);
    sector = atoi(room.substr(digit, lbracket - digit).c_str());
    checksum = room.substr(lbracket + 1, rbracket - lbracket - 1);
}

bool is_valid(const std::string encrypted_name, const std::string checksum)
{
    class mycomparison
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
    for (auto i = 0; i < 26; i++) count[i] = 0;

    for (auto c : encrypted_name)
        if (c != '-')
            count[c - 'a']++;

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

    return checksum == temp;
}

bool process_room(const std::string &room, unsigned &sector, std::string &name)
{
    std::string encrypted_name, checksum;
    parse(room, encrypted_name, sector, checksum);

    if (!is_valid(encrypted_name, checksum)) return false;

    name = decrypt(encrypted_name, sector);
    return true;
}

int main()
{
    std::ifstream f("input.txt");
    std::string room, name;
    auto total = 0U, sector = 0U, northpole_object_storage = 0U;

    while (f >> room)
    {
        if (process_room(room, sector, name))
        {
            total += sector;
            std::cout << name << " (" << sector << ")" << std::endl;

            // Note: Exact name not specified in the problem, but found by perusing
            // the "decrypted" names in the above program output.
            if (name == "northpole object storage") northpole_object_storage = sector;
        }
    }

    std::cout << std::endl << "Part One: " << total << std::endl;
    std::cout << "Part Two: " << northpole_object_storage << std::endl;

    assert(total == 158835);
    assert(northpole_object_storage == 993);
    return 0;
}
