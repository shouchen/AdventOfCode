#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <cassert>

struct PlayerState
{
    int pos, score;
};

auto read_input(const std::string &filename)
{
    std::ifstream file("input.txt");
    std::string player, n, starting, position;
    auto input = std::make_pair(0, 0);

    file >> player >> n >> starting >> position >> input.first;
    file >> player >> n >> starting >> position >> input.second;
    return input;
}

auto roll_die()
{
    static auto die = 0;

    if (++die > 100) die = 1;
    return die;
}

auto do_part1(std::pair<int, int> &pos)
{
    std::array<PlayerState, 2> psa{ PlayerState{pos.first, 0}, PlayerState{pos.second, 0} };
    auto roll_count = 0;

    for (auto curr = 0, other = 1; ; std::swap(curr, other))
    {
        auto total = roll_die() + roll_die() + roll_die();
        roll_count += 3;

        psa[curr].pos = (psa[curr].pos + total - 1) % 10 + 1;
        psa[curr].score += psa[curr].pos;

        if (psa[curr].score >= 1000)
            return psa[other].score * roll_count;
    }
}

long long wins[2];
int ways_to_roll_value[10];

void recur(std::array<PlayerState, 2> psa, int turn = 0, long long ways_to_reach_this_state = 1)
{
    for (auto roll = 3; roll <= 9; roll++)
    {
        auto new_psa = psa;
        auto &ps = new_psa[turn];
        auto new_ways = ways_to_reach_this_state * ways_to_roll_value[roll];

        ps.pos += roll;
        if (ps.pos > 10)
            ps.pos -= 10;

        ps.score += ps.pos;

        if (ps.score >= 21)
            wins[turn] += new_ways;
        else
            recur(new_psa, 1 - turn, new_ways);
    }
}

auto do_part2(std::pair<int, int> &pos)
{
    for (auto i = 1; i <= 3; i++)
        for (auto j = 1; j <= 3; j++)
            for (auto k = 1; k <= 3; k++)
                ways_to_roll_value[i + j + k]++;

    std::array<PlayerState, 2> psa{ PlayerState{pos.first, 0}, PlayerState{pos.second, 0} };
    recur(psa);
    return std::max(wins[0], wins[1]);
}

int main()
{
    auto positions = read_input("input.txt");

    auto part1 = do_part1(positions);
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part2(positions);
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 675024);
    assert(part2 == 570239341223618);
    return 0;
}
