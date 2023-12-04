#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <ctype.h>
#include <algorithm>
#include <regex>
#include <cassert>

using namespace std;

bool vector_contains(std::vector<int> v, int n)
{
    for (auto i : v)
        if (i == n)
            return true;
    return false;
}

auto do_part1(const std::string &filename)
{
    auto total = 0;

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        auto p1 = line.find(':');
        auto p2 = line.find('|');

        auto s1 = line.substr(p1 + 1, p2 - p1 - 1);
        auto s2 = line.substr(p2 + 1);

        int n;
        vector<int> v1, v2;
        std::istringstream is(s1);
        while (is >> n)
            v1.push_back(n);
        std::istringstream is2(s2);
        while (is2 >> n)
            v2.push_back(n);

        int play = 0;

        for (auto q2 : v2)
            if (vector_contains(v1, q2))
            {
                if (play == 0)
                    play = 1;
                else
                    play *= 2;
            }
            
        total += play;
    }

    

    
    return total;
}

struct Scratchcard { int n; vector<int> v1; vector<int> v2; int wins; };
map<int, pair<Scratchcard, int>> scs;

int count_wins(Scratchcard &sc)
{
    int total = 0;
    for (auto q2 : sc.v2)
        if (vector_contains(sc.v1, q2))
        {
            total++;
        }

    return total;
}

auto do_part2(const std::string &filename)
{
    auto total = 0;

    std::ifstream file(filename);
    std::string line;



    while (std::getline(file, line))
    {
        auto p1 = line.find(':');
        auto p2 = line.find('|');

        auto s1 = line.substr(p1 + 1, p2 - p1 - 1);
        auto s2 = line.substr(p2 + 1);

        int n;
        vector<int> v1, v2;
        std::istringstream is(s1);
        while (is >> n)
            v1.push_back(n);
        std::istringstream is2(s2);
        while (is2 >> n)
            v2.push_back(n);



        Scratchcard sc{ atoi(line.substr(5).c_str()), v1, v2};
        sc.wins = count_wins(sc);
        scs[sc.n] = make_pair(sc, 1);
    }

    for (int i = 1; i < 1000; i++)
    {
        for (int k = 0; k < scs[i].second; k++)
        {
            auto &sc = scs[i].first;
            for (int j = 1; j <= sc.wins; j++)
            {
                scs[i + j].second++;
            }
        }

    }

    total = 0;

    for (auto &mmm : scs)
    {
        total += mmm.second.second;
    }




    return total;
}
int main()
{
    //auto part1 = do_part1("input.txt");
    //std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 24160);
    assert(part2 == 5659035);
    return 0;
}
