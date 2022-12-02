#include <iostream>
#include <fstream>
#include <cassert>

enum class Option { Rock = 1, Paper = 2, Scissors = 3 };
enum class Outcome { Lose = 0, Draw = 3, Win = 6 };

Option map_ABC_to_option(char abc)
{
    static const Option options[] = { Option::Rock, Option::Paper, Option::Scissors };
    return options[abc - 'A'];
}

Option map_XYZ_to_option(char xyz)
{
    static const Option option[] = { Option::Rock, Option::Paper, Option::Scissors };
    return option[xyz - 'X'];
}

Outcome map_XYZ_to_outcome(char xyz)
{
    static const Outcome outcome[] = { Outcome::Lose, Outcome::Draw, Outcome::Win };
    return outcome[xyz - 'X'];
}

Outcome get_my_outcome(Option they_play, Option i_play)
{
    switch (they_play)
    {
    case Option::Rock:
        if (i_play == Option::Paper)
            return Outcome::Win;
        if (i_play == Option::Scissors)
            return Outcome::Lose;
        return Outcome::Draw;
    case Option::Paper:
        if (i_play == Option::Scissors)
            return Outcome::Win;
        if (i_play == Option::Rock)
            return Outcome::Lose;
        return Outcome::Draw;
    case Option::Scissors:
        if (i_play == Option::Rock)
            return Outcome::Win;
        if (i_play == Option::Paper)
            return Outcome::Lose;
        return Outcome::Draw;
    }

    assert(false);
    return Outcome::Draw;
}

Option get_my_option(Option they_play, Outcome outcome)
{
    switch (they_play)
    {
    case Option::Rock:
        if (outcome == Outcome::Lose)
            return Option::Scissors;
        if (outcome == Outcome::Draw)
            return Option::Rock;
        return Option::Paper;
    case Option::Paper:
        if (outcome == Outcome::Lose)
            return Option::Rock;
        if (outcome == Outcome::Draw)
            return Option::Paper;
        return Option::Scissors;
    case Option::Scissors:
        if (outcome == Outcome::Lose)
            return Option::Paper;
        if (outcome == Outcome::Draw)
            return Option::Scissors;
        return Option::Rock;
    }

    assert(false);
    return Option::Rock;
}

int score_me1(char abc, char xyz)
{
    auto they_play = map_ABC_to_option(abc), i_play = map_XYZ_to_option(xyz);
    auto outcome = get_my_outcome(they_play, i_play);
    return unsigned(i_play) + unsigned(outcome);
}

int score_me2(char abc, char xyz)
{
    Outcome outcome = map_XYZ_to_outcome(xyz);
    Option they_play = map_ABC_to_option(abc), i_play = get_my_option(they_play, outcome);
    return unsigned(i_play) + unsigned(outcome);
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto c1 = ' ', c2 = ' ';
    auto total_score = 0;

    while (file >> c1 >> c2)
    {
        auto s = score_me1(c1, c2);
        total_score += s;
    }

    return total_score;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    auto c1 = ' ', c2 = ' ';
    auto total_score = 0;

    while (file >> c1 >> c2)
    {
        auto s = score_me2(c1, c2);
        total_score += s;
    }

    return total_score;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 10718);
    assert(part2 == 14652);
    return 0;
}
