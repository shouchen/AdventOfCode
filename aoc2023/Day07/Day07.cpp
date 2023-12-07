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
    HandBid(const std::string &hand, int bid);

    std::string hand;
    Type type1, type2;
    int bid;
};

HandBid::HandBid(const std::string &hand, int bid) : hand(hand), bid(bid)
{
    std::map<char, int> card_to_count;
    for (auto c : hand)
        card_to_count[c]++;

    std::map<int, int> count_of_counts;
    for (auto &mm : card_to_count)
        count_of_counts[mm.second]++;

    if (count_of_counts[5] == 1)
        type1 = FiveOfAKind;
    else if (count_of_counts[4] == 1)
        type1 = FourOfAKind;
    else if (count_of_counts[3] == 1 && count_of_counts[2] == 1)
        type1 = FullHouse;
    else if (count_of_counts[3] == 1)
        type1 = ThreeOfAKind;
    else if (count_of_counts[2] == 2)
        type1 = TwoPair;
    else if (count_of_counts[2] == 1)
        type1 = OnePair;
    else
        type1 = HighCard;

    type2 = type1;
    auto jokers = card_to_count['J'];

    switch (type2)
    {
    case FourOfAKind:
    case FullHouse:
        if (jokers) type2 = FiveOfAKind;
        break;
    case ThreeOfAKind:
        if (jokers) type2 = FourOfAKind;
        break;
    case TwoPair:
        if (jokers == 2) type2 = FourOfAKind;
        else if (jokers == 1) type2 = FullHouse;
        break;
    case OnePair:
        if (jokers) type2 = ThreeOfAKind;
        break;
    case HighCard:
        if (jokers) type2 = OnePair;
        break;
    }
}

using HandBidComparator = bool (*)(const HandBid &hb1, const HandBid &hb2);

std::vector<HandBid> hbv;

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string hand;
    auto bid = 0;

    while (file >> hand >> bid)
        hbv.push_back(HandBid(hand, bid));
}

bool compare_hands(const HandBid &hb1, const HandBid &hb2, Type HandBid:: *type, std::string order)
{
    if (hb1.*type < hb2.*type) return true;
    if (hb2.*type < hb1.*type) return false;

    for (auto i = 0; i < hb1.hand.length(); i++)
    {
        auto pos1 = order.find(hb1.hand[i]), pos2 = order.find(hb2.hand[i]);
        if (pos1 < pos2) return true;
        if (pos2 < pos1) return false;
    }

    return false;
}

bool compare_hands1(const HandBid &hb1, const HandBid &hb2)
{
    return compare_hands(hb1, hb2, &HandBid::type1, "AKQJT98765432");
}

bool compare_hands2(const HandBid &hb1, const HandBid &hb2)
{
    return compare_hands(hb1, hb2, &HandBid::type2, "AKQT98765432J");
}

auto do_part(HandBidComparator cmp)
{
    auto retval = 0LL;

    sort(hbv.begin(), hbv.end(), cmp);

    for (auto i = 0; i < hbv.size(); i++)
    {
        auto rank = hbv.size() - i;
        retval += hbv[i].bid * rank;
    }

    return retval;
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part(compare_hands1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part(compare_hands2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 251058093);
    assert(part2 == 249781879);
    return 0;
}
