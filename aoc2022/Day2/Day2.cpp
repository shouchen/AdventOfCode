#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <string>
#include <algorithm>
#include <numeric>

int score_me(char c1, char c2)
{
    bool win = (c1 == 'A' && c2 == 'Y' || c1 == 'B' && c2 == 'Z' || c1 == 'C' && c2 == 'X');
    bool tie = (c1 == 'A' && c2 == 'X' || c1 == 'B' && c2 == 'Y' || c1 == 'C' && c2 == 'Z');
    bool lose = !win && !tie;

    int retval = c2 - 'W';
    if (win)
        return retval + 6;
    else if (tie)
        return retval + 3;
    else
        return retval;
}

// x = lose , y = draw z=win
int score_me2(char c1, char c2)
{
    std::string play;

    switch (c1)
    {
    case 'A': // they play rock
        if (c2 == 'X')  // must lose
            play = "scissors";
        else if (c2 == 'Y') // must tie
            play = "rock";
        else  // must win
            play = "paper";
        break;

    case 'B': // they play paper
        if (c2 == 'X')  // must lose
            play = "rock";
        else if (c2 == 'Y') // must tie
            play = "paper";
        else  // must win
            play = "scissors";
        break;

    case 'C': // they play scissors
        if (c2 == 'X')  // must lose
            play = "paper";
        else if (c2 == 'Y')// must tie
            play = "scissors";
        else  // must win
            play = "rock";
        break;
    }

    auto retval = 0;

    if (play == "rock")
        retval = 1;
    else if (play == "paper")
        retval = 2;
    else if (play == "scissors")
        retval = 3;
    else
        assert(false);

    if (c2 == 'X')
        return retval;
    if (c2 == 'Y')
        return retval + 3;
    if (c2 == 'Z')
        return retval + 6;
}

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);

    char c1, c2;

    // A = rock, B = papar, C= scissors  (opponent)
    // X =       Y          Z   (what I should play)

    // sroce for round: score for shape (1-3) + outcome (0 if loss, 3 is draw, 6 if win)

    int total_score = 0;
    while (file >> c1 >> c2)
    {
        auto s = score_me2(c1, c2);
        total_score += s;
    }

    return total_score;
}

auto do_part(std::vector<int> &data, int n)
{
    return std::accumulate(data.end() - n, data.end(), 0);
}

int main()
{
    auto part1 = read_data("input.txt");

    //auto part1 = do_part(data, 1);
    std::cout << "Part One: " << part1 << std::endl;

    //auto part2 = do_part(data, 3);
    //std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == 10718);
    //assert(part2 == 200945);
    return 0;
}
