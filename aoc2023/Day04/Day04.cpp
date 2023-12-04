#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

struct Scratchcard { std::vector<int> v1; std::vector<int> v2; int wins = 0, copies = 1; };
std::map<int, std::pair<Scratchcard, int>> scs;  // cards and copies

void read_data(const std::string &filename)
{
    auto total = 0;

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        auto p1 = line.find(':'), p2 = line.find('|');
        auto s1 = line.substr(p1 + 1, p2 - p1 - 1), s2 = line.substr(p2 + 1);

        Scratchcard sc;
        int n;

        std::istringstream is(s1);
        while (is >> n)
            sc.v1.push_back(n);

        std::istringstream is2(s2);
        while (is2 >> n)
            sc.v2.push_back(n);

        for (auto q2 : sc.v2)
            if (find(sc.v1.begin(), sc.v1.end(), q2) != sc.v1.end())
                sc.wins++;

        auto card_no = atoi(line.substr(5).c_str());
        scs[card_no] = std::make_pair(sc, 1);
    }
}

auto do_part1(const std::string &filename)
{
    auto total = 0;

    for (auto &qq : scs)
    {
        auto w = qq.second.first.wins;
        if (w)
            total += 1 << (w - 1);
    }

    return total;
}

auto do_part2()
{
    for (int i = 1; i <= scs.size(); i++)
    {
        auto &sc = scs[i].first;

        for (int k = 0; k < scs[i].second; k++)
            for (int j = 1; j <= sc.wins; j++)
            {
                scs[i + j].second++;
                scs[i + j].first.copies++;
            }
    }

    auto total = 0;

    for (auto &mmm : scs)
        total += mmm.second.first.copies;

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
