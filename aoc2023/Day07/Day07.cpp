#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

enum Type
{
    FiveOfAKind,
    FourOfAKind,
    FullHouse,
    ThreeOfAKind,
    TwoPair,
    OnePair,
    HighCard
};

struct HandBid
{
    std::string hand;
    Type type = HighCard;
    int bid = 0;
};

std::string order1 = "AKQJT98765432", order2 = "AKQT98765432J";

Type get_type(std::string hand)
{
    std::map<char, int> m;
    for (auto c : hand)
        m[c]++;

    std::map<int, int> s2;
    for (auto &mm : m)
        s2[mm.second]++;

    if (s2.find(5) != s2.end())
        return FiveOfAKind;
    if (s2.find(4) != s2.end())
        return FourOfAKind;
    if (s2.find(3) != s2.end() && s2.find(2) != s2.end())
        return FullHouse;
    if (s2.find(3) != s2.end())
        return ThreeOfAKind;
    if (s2[2] == 2 && s2[1] == 1)
        return TwoPair;
    if (s2[2] == 1)
        return OnePair;

    return HighCard;
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

bool compare_hands(const HandBid &hb1, const HandBid &hb2, const std::string &order)
{
    auto h1 = hb1.hand, h2 = hb2.hand;

    if (hb1.type < hb2.type) return true;
    if (hb2.type < hb1.type) return false;

    for (int i = 0; i < 5; i++)
    {
        auto pos1 = order.find(h1[i]), pos2 = order.find(h2[i]);
        if (pos1 < pos2) return true;
        if (pos2 < pos1) return false;
    }

    return false;
}

bool compare_hands1(const HandBid &hb1, const HandBid &hb2)
{
    return compare_hands(hb1, hb2, order1);
}

bool compare_hands2(const HandBid &hb1, const HandBid &hb2)
{
    return compare_hands(hb1, hb2, order2);
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    std::vector<HandBid> hbv;
    HandBid hb;
    auto retval = 0LL;

    while (file >> hb.hand >> hb.bid)
    {
        hb.type = part2 ? get_best_type_with_jokers(hb.hand) : get_type(hb.hand);
        hbv.push_back(hb);
    }

    sort(hbv.begin(), hbv.end(), part2 ? compare_hands2 : compare_hands1);

    for (auto i = 0; i < hbv.size(); i++)
    {
        auto rank = hbv.size() - i;
        retval += hbv[i].bid * rank;
    }

    return retval;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 251058093);
    assert(part2 == 249781879);
    return 0;
}
