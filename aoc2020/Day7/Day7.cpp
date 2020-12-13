#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

std::map<std::string, std::vector<std::pair<std::string, int>>> data;

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string color1, color2, contain, amount, bags, other, color3, color4;

    while (file >> color1 >> color2 >> bags >> contain)
    {
        do
        {
            file >> amount;

            if (amount == "no")
            {
                file >> other >> bags;
            }
            else
            {
                file >> color3 >> color4 >> bags;
                data[color1 + " " + color2].push_back(make_pair(color3 + " " + color4, atoi(amount.c_str())));
            }
        } while (bags[bags.length() - 1] != '.');
    }
}

bool contains_shiny_gold(const std::string &color)
{
    static std::map<std::string, bool> memo;

    if (memo.find(color) == memo.end())
    {
        for (auto &a : data[color])
            if (a.first == "shiny gold" || contains_shiny_gold(a.first))
                return memo[color] = true;

        memo[color] = false;
    }

    return memo[color];
}

auto do_part1()
{
    auto count = 0;
    for (auto &i : data)
        if (contains_shiny_gold(i.first))
            count++;

    return count;
}

int do_part2(const std::string &color = "shiny gold")
{
    static std::map<std::string, int> memo;

    if (memo.find(color) == memo.end())
    {
        auto count = 0;
        for (auto &a : data[color])
            count += a.second * (1 + do_part2(a.first));

        memo[color] = count;
    }

    return memo[color];
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 224);
    assert(part2 == 1488);
    return 0;
}
