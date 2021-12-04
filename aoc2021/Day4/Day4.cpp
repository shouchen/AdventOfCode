#include <iostream>
#include <fstream>
#include <strstream>
#include <array>
#include <vector>
#include <set>
#include <cassert>

using Card = std::array<std::array<int, 5>, 5>;

std::vector<Card> cards;
std::vector<int> calls;

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string s;
    file >> s;

    std::istrstream ss(s.c_str());
    auto n = 0;
    auto comma = ',';

    while (ss >> n)
    {
        calls.push_back(n);
        ss >> comma;
    }

    while (file)
    {
        Card card;

        for (auto row = 0; row < 5; row++)
            for (auto col = 0; col < 5; col++)
                if (file >> n)
                    card[row][col] = n;
                else
                    break;
        
        cards.push_back(card);
    }
}

auto is_winner(const Card &card)
{
    for (auto i = 0; i < 5; i++)
        if (card[i][0] == -1 && card[i][1] == -1 && card[i][2] == -1 && card[i][3] == -1 && card[i][4] == -1 ||
            card[0][i] == -1 && card[1][i] == -1 && card[2][i] == -1 && card[3][i] == -1 && card[4][i] == -1)
        {
            return true;
        }

    return false;
}

auto total(const Card &card)
{
    auto sum = 0;
    for (auto row = 0; row < 5; row++)
        for (auto col = 0; col < 5; col++)
            if (card[row][col] != -1)
                sum += card[row][col];

    return sum;
}

auto play_game()
{
    auto part1 = 0, part2 = 0;
    std::set<Card *> winners;

    for (auto call : calls)
        for (auto &card : cards)
            if (winners.find(&card) == winners.end())
            {
                for (auto row = 0; row < 5; row++)
                    for (auto col = 0; col < 5; col++)
                        if (card[row][col] == call)
                            card[row][col] = -1;

                if (is_winner(card))
                {
                    if (winners.size() == 0)
                        part1 = total(card) * call;

                    winners.insert(&card);

                    if (winners.size() == cards.size())
                    {
                        part2 = total(card) * call;
                        return std::make_pair(part1, part2);
                    }
                }
            }

    return std::make_pair(part1, part2);
}

int main()
{
    read_data("input.txt");

    auto result = play_game();
    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 11774);
    assert(result.second == 4495);
    return 0;
}
