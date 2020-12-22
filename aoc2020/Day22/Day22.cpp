#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <cassert>

struct Game
{
    std::deque<int> player[3];  // [0] is unused

    inline auto num_cards_in_hand(int p) { return int(player[p].size()); }

    int play_game(bool part2)
    {
        for (;;)
        {
            if (part2 && is_infinite_loop())
                return 1;

            int play[3];
            for (auto p = 1; p <= 2; p++)
            {
                play[p] = player[p].front();
                player[p].pop_front();
            }

            auto round_winner = 0;

            if (part2 && num_cards_in_hand(1) >= play[1] && num_cards_in_hand(2) >= play[2])
            {
                Game subgame;
                for (auto p = 1; p <= 2; p++)
                    for (auto i = 0; i < play[p]; i++)
                        subgame.player[p].push_back(player[p][i]);

                round_winner = subgame.play_game(true);
            }
            else
            {
                round_winner = (play[1] > play[2]) ? 1 : 2;
            }

            auto round_loser = 3 - round_winner;
            player[round_winner].push_back(play[round_winner]);
            player[round_winner].push_back(play[round_loser]);

            for (auto p = 1; p <= 2; p++)
                if (player[p].empty())
                    return 3 - p;
        }
    }

private:
    std::set<std::vector<int>> seen;

    bool is_infinite_loop()
    {
        std::vector<int> state;

        for (auto c : player[1])
            state.push_back(c);
        state.push_back(-1);
        for (auto c : player[2])
            state.push_back(c);

        auto x = seen.find(state);
        if (x != seen.end())
            return true;

        seen.insert(state);
        return false;
    }
};

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file("input.txt");
    std::string s;
    Game g;
    auto p = 0;
    
    while (getline(file, s))
        if (s.length())
            if (s.substr(0, 6) == "Player")
                p++;
            else
                g.player[p].push_back(atoi(s.c_str()));

    auto winner = g.play_game(part2);
    auto score = 0, multiplier = g.num_cards_in_hand(winner);

    for (auto c : g.player[winner])
        score += c * multiplier--;

    return score;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 34127);
    assert(part2 == 32054);
    return 0;
}
