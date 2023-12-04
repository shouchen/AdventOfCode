#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Scratchcard { std::vector<int> v1, v2; int wins = 0, copies = 1; };

std::vector<Scratchcard> cards;

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        cards.push_back(Scratchcard());
        auto &sc = cards.back();

        auto colon = line.find(':'), bar = line.find('|');
        auto n = 0;

        std::istringstream is(line.substr(colon + 1, bar - colon - 1));
        while (is >> n)
            sc.v1.push_back(n);

        is = std::istringstream(line.substr(bar + 1));
        while (is>> n)
        {
            sc.v2.push_back(n);
            if (find(sc.v1.begin(), sc.v1.end(), n) != sc.v1.end())
                sc.wins++;
        }
    }
}

auto do_part1()
{
    auto total = 0;

    for (auto &c : cards)
        if (c.wins)
            total += 1 << (c.wins - 1);

    return total;
}

auto do_part2()
{
    for (auto i = 0; i < cards.size(); i++)
        for (auto j = 1; j <= cards[i].wins; j++)
            cards[i + j].copies += cards[i].copies;

    auto total = 0;

    for (auto &sc : cards)
        total += sc.copies;

    return total;
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 24160);
    assert(part2 == 5659035);
    return 0;
}
