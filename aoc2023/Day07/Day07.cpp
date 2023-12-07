#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

std::string order1 = "AKQJT98765432";
std::string order2 = "AKQT98765432J";

enum Type
{
    Fiveofakind, // where all five cards have the same label : AAAAA
    Fourofakind, // where four cards have the same label and one card has a different label : AA8AA
    Fullhouse, // where three cards have the same label, and the remaining two cards share a different label : 23332
    Threeofakind, // where three cards have the same label, and the remaining two cards are each different from any other card in the hand : TTT98
    Twopair, // where two cards share one label, two other cards share a second label, and the remaining card has a third label : 23432
    Onepair, // where two cards share one label, and the other three cards have a different label from the pair and each other : A23A4    
    Highcard // where all cards' labels are distinct: 23456
};

Type get_best_type_with_jokers(std::string hand);

Type get_type(const std::string &hand)
{
    std::map<char, int> m;
    for (int i = 0; i < 5; i++)
        m[hand[i]]++;

    std::map<int, int> s2;
    for (auto &mm : m)
        s2[mm.second]++;

    if (s2.find(5) != s2.end())
        return Fiveofakind;
    if (s2.find(4) != s2.end())
        return Fourofakind;
    if (s2.find(3) != s2.end() && s2.find(2) != s2.end())
        return Fullhouse;
    if (s2.find(3) != s2.end())
        return Threeofakind;
    if (s2[2] == 2 && s2[1] == 1)
        return Twopair;
    if (s2[2] == 1)
        return Onepair;

    return Highcard;
}

Type get_best_type_with_jokers(std::string hand)
{
    Type best = get_type(hand);

    auto pos = hand.find('J');
    if (pos != std::string::npos)
    {
        for (int i = 0; i < order2.size() - 1; i++)
        {
            hand[pos] = order2[i];
            auto temp = get_best_type_with_jokers(hand);
            if (temp < best) best = temp;
        }
    }

    return best;
}

struct HandBid
{
    std::string hand;
    int bid;
};

bool compare_hands1(const HandBid &hb1, const HandBid &hb2)
{
    auto h1 = hb1.hand, h2 = hb2.hand;

    // PART1 Type t1 = get_type(h1), t2 = get_type(h2);
    Type t1 = get_best_type_with_jokers(h1), t2 = get_best_type_with_jokers(h2);

    if (t1 < t2) return true;
    if (t2 < t1) return false;

    for (int i = 0; i < 5; i++)
    {
        auto pos1 = order1.find(h1[i]), pos2 = order1.find(h2[i]);
        if (pos1 < pos2)
            return true;
        if (pos2 < pos1)
            return false;
    }

    return false;
}

bool compare_hands2(const HandBid &hb1, const HandBid &hb2)
{
    auto h1 = hb1.hand, h2 = hb2.hand;

    // PART1 Type t1 = get_type(h1), t2 = get_type(h2);
    Type t1 = get_best_type_with_jokers(h1), t2 = get_best_type_with_jokers(h2);

    if (t1 < t2) return true;
    if (t2 < t1) return false;

    for (int i = 0; i < 5; i++)
    {
        auto pos1 = order2.find(h1[i]), pos2 = order2.find(h2[i]);
        if (pos1 < pos2)
            return true;
        if (pos2 < pos1)
            return false;
    }

    return false;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string hand, word;

    std::vector<HandBid> hbv;

    HandBid hb;
    while (file >> hb.hand >> hb.bid)
    {
        hbv.push_back(hb);

        auto t = get_best_type_with_jokers(hb.hand);
        std::cout << t << std::endl;
    }


    sort(hbv.begin(), hbv.end(), compare_hands1);

    long long total = 0;
    for (int i = 0; i < hbv.size(); i++)
    {
        auto rank = hbv.size() - i;

        total += hbv[i].bid * rank;
    }

    return total;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string hand, word;

    std::vector<HandBid> hbv;

    HandBid hb;
    while (file >> hb.hand >> hb.bid)
    {
        hbv.push_back(hb);

        auto t = get_best_type_with_jokers(hb.hand);
        std::cout << t << std::endl;
    }


    sort(hbv.begin(), hbv.end(), compare_hands2);

    long long total = 0;
    for (int i = 0; i < hbv.size(); i++)
    {
        auto rank = hbv.size() - i;

        total += hbv[i].bid * rank;
    }

    return total;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl; // wrong 249477083 // wrong 249723387

    //assert(part1 == 251058093 );
    //assert(part2 == 249781879);
    return 0;
}
