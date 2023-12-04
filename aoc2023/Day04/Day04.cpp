#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <numeric>
#include <cassert>

struct Scratchcard { int wins = 0, copies = 1; };

auto solve(const std::string &filename)
{
    std::vector<Scratchcard> cards;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        cards.push_back(Scratchcard());

        auto colon = line.find(':'), bar = line.find('|');
        auto n = 0;
        std::set<int> winning;

        auto is = std::istringstream(line.substr(colon + 1, bar - colon - 1));
        while (is >> n)
            winning.insert(n);

        is = std::istringstream(line.substr(bar + 1));
        while (is>> n)
            if (winning.find(n) != winning.end())
                cards.back().wins++;
    }

    for (auto i = 0; i < cards.size(); i++)
        for (auto j = 1; j <= cards[i].wins; j++)
            cards[i + j].copies += cards[i].copies;

    return std::make_pair(
        std::accumulate(
            cards.begin(), cards.end(), 0, [](int a, const Scratchcard &sc) {
                return a + (sc.wins ? (1 << (sc.wins - 1)) : 0);
            }),
        std::accumulate(
            cards.begin(), cards.end(), 0, [](int a, const Scratchcard &sc) {
                return a + sc.copies;
            }));
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