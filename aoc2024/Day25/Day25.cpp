#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

using Schematic = std::vector<int>;

std::vector<Schematic> keys, locks;

auto process_entity(std::vector<std::string> entity)
{
    Schematic heights{ -1, -1, -1, -1, -1 };

    for (auto &r : entity)
        for (auto i = 0; i < 5; i++)
            if (r[i] == '#')
                heights[i]++;

    auto is_key = entity[0][0] == '.';
    std::vector<Schematic> &schematic_to_update = is_key ? keys : locks;
    schematic_to_update.push_back(heights);
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> entity;

    while (std::getline(file, line))
    {
        if (line.length() == 0)
        {
            process_entity(entity);

            if (!std::getline(file, line))
                break;

            entity.clear();
        }

        entity.push_back(line);
    }

    process_entity(entity);

    auto retval = 0;
    for (auto &k : keys)
        for (auto &l : locks)
        {
            bool fit = true;
            for (int i = 0; i < 5; i++)
                if (k[i] + l[i] > 5)
                {
                    fit = false;
                    break;
                }

            if (fit)
                retval++;
        }

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
