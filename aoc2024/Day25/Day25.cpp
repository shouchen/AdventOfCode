#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <cassert>

constexpr auto PINS = 5;
using Schematic = std::array<int, PINS>;

auto fits(const Schematic &lock, const Schematic &key)
{
    for (auto i = 0; i < PINS; i++)
        if (lock[i] + key[i] > PINS)
            return false;

    return true;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<Schematic> keys, locks;
    auto c = ' ';

    while (file >> line)
    {
        auto &collection_to_update = (line[0] == '.') ? keys : locks;
        Schematic heights{ 0 };

        for (auto i = 0; i < PINS; i++)
            for (auto j = 0; j < PINS; j++)
            {
                file >> c;
                if (c == '#')
                    heights[j]++;
            }

        collection_to_update.push_back(heights);
        file >> line;
    }

    auto retval = 0;
    for (auto &lock : locks)
        for (auto &key : keys)
            if (fits(lock, key))
                retval++;

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 3395 );

    std::cout << "Part Two: N/A" << std::endl;
    return 0;
}
