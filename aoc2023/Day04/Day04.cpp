#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

struct Card { int wins = 0, copies = 1; };

auto solve(const std::string &filename)
{
    std::vector<Card> cards;
    std::ifstream file(filename);
    std::string line;

    auto retval = std::make_pair(0, 0);

    while (std::getline(file, line))
    {
        cards.push_back(Card());

        auto colon = line.find(':'), bar = line.find('|');
        auto n = 0;
        std::set<int> winning;

        auto is = std::istringstream(line.substr(colon + 1, bar - colon - 1));
        while (is >> n)
            winning.insert(n);

        is = std::istringstream(line.substr(bar + 1));
        while (is >> n)
            if (winning.find(n) != winning.end())
                cards.back().wins++;

        if (cards.back().wins)
            retval.first += 1 << (cards.back().wins - 1);
    }

    for (auto i = 0; i < cards.size(); i++)
    {
        retval.second += cards[i].copies;
        for (auto j = 1; j <= cards[i].wins; j++)
            cards[i + j].copies += cards[i].copies;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 24160);
    assert(answer.second == 5659035);
    return 0;
}