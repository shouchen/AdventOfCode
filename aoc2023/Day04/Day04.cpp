#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Scratchcard { std::vector<int> v1; std::vector<int> v2; int wins = 0, copies = 1; };
std::vector<Scratchcard> cards;

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        cards.push_back(Scratchcard());
        auto &sc = cards.back();

        auto p1 = line.find(':'), p2 = line.find('|');
        auto s1 = line.substr(p1 + 1, p2 - p1 - 1), s2 = line.substr(p2 + 1);
        auto n = 0;

        std::istringstream is1(s1);
        while (is1 >> n)
            sc.v1.push_back(n);

        std::istringstream is2(s2);
        while (is2 >> n)
            sc.v2.push_back(n);

        for (auto i : sc.v2)
            if (find(sc.v1.begin(), sc.v1.end(), i) != sc.v1.end())
                sc.wins++;
    }
}

auto do_part1(const std::string &filename)
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

    for (auto &mmm : cards)
        total += mmm.copies;

    return total;
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 24160);
    assert(part2 == 5659035);
    return 0;
}
