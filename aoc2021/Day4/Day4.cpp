#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>

using namespace std;

typedef map<pair<int, int>, pair<int, bool>> Card; // row, col --> number, occupied

bool is_winner(Card &card, const set<int> &called)
{
    for (auto row = 0; row < 5; row++)
        if (card[make_pair(row, 0)].first && card[make_pair(row, 1)].first && card[make_pair(row, 2)].first && card[make_pair(row, 3)].first && card[make_pair(row, 4)].first)
            return true;

    for (auto col = 0; col < 5; col++)
        if (card[make_pair(0, col)].second && card[make_pair(1, col)].second && card[make_pair(2, col)].second && card[make_pair(3, col)].second && card[make_pair(4, col)].second)
            return true;

    return false;
}

int uncalled_total(Card &card, const set<int> &called)
{
    int sum = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (!card[make_pair(i, j)].second)
                sum += card[make_pair(i, j)].first;

    return sum;
}

void read_data(const std::string &filename, int &part1, int &part2)
{
    part1 = part2 = 0;

    std::ifstream file(filename);
    std::string s;

    vector<int> calls;

    file >> s;
    int n;

    istrstream ss(s.c_str());
    while (ss >> n)
    {
        calls.push_back(n);

        char c;
        if (ss)
            ss >> c;
    }

    vector<Card> cards;

    bool done = false;
    while (file)
    {
        Card card;

        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
            {
                if (file >> n)
                    card[make_pair(i, j)] = make_pair(n, false);
                else
                    done = true;
            }

        if (!done)
            cards.push_back(card);
    }

    // play

    set<int> has_won;
    set<int> called;

    for (int c : calls)
    {
        called.insert(c);

        for (Card &card : cards)
        {
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 5; j++)
                {
                    if (card[make_pair(i,j )].first == c)
                    {
                        card[make_pair(i, j)].second = true;
                    }
                }
        }

        for (auto i = 0U; i < cards.size(); i++)
        {
            Card &card = cards[i];

            if (is_winner(card, called))
            {
                if (part1 == 0)
                    part1 = uncalled_total(card, called) * c;

                has_won.insert(i);
                if (has_won.size() == cards.size())
                {
                    part2 = uncalled_total(card, called) * c;
                    return;
                }
            }
        }
    }
}

int main()
{
    auto part1 = 0, part2 = 0;
    read_data("input.txt", part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 11774);
    assert(part2 == 4495);
    return 0;
}
