#include <iostream>
#include <fstream>
#include <cassert>

enum class Option { Rock = 1, Paper = 2, Scissors = 3 };
enum class Outcome { Lose = 0, Draw = 3, Win = 6 };

auto map_ABC_to_option(char abc)
{
    static const Option options[] = { Option::Rock, Option::Paper, Option::Scissors };
    return options[abc - 'A'];
}

auto map_XYZ_to_option(char xyz)
{
    static const Option option[] = { Option::Rock, Option::Paper, Option::Scissors };
    return option[xyz - 'X'];
}

auto map_XYZ_to_outcome(char xyz)
{
    static const Outcome outcome[] = { Outcome::Lose, Outcome::Draw, Outcome::Win };
    return outcome[xyz - 'X'];
}

auto get_my_outcome(Option they_play, Option i_play)
{
    switch (they_play)
    {
    case Option::Rock:
        if (i_play == Option::Paper)
            return Outcome::Win;
        if (i_play == Option::Scissors)
            return Outcome::Lose;
        break;
    case Option::Paper:
        if (i_play == Option::Scissors)
            return Outcome::Win;
        if (i_play == Option::Rock)
            return Outcome::Lose;
        break;
    case Option::Scissors:
        if (i_play == Option::Rock)
            return Outcome::Win;
        if (i_play == Option::Paper)
            return Outcome::Lose;
        break;
    }

    return Outcome::Draw;
}

auto get_my_option(Option they_play, Outcome outcome)
{
    switch (they_play)
    {
    case Option::Rock:
        if (outcome == Outcome::Draw)
            return Option::Rock;
        if (outcome == Outcome::Win)
            return Option::Paper;
        break;
    case Option::Paper:
        if (outcome == Outcome::Lose)
            return Option::Rock;
        if (outcome == Outcome::Draw)
            return Option::Paper;
        break;
    case Option::Scissors:
        if (outcome == Outcome::Win)
            return Option::Rock;
        if (outcome == Outcome::Lose)
            return Option::Paper;
        break;
    }

    return Option::Scissors;
}

void process_input(const std::string &filename, unsigned &part1, unsigned &part2)
{
    std::ifstream file(filename);
    auto abc = ' ', xyz = ' ';
    
    part1 = part2 = 0;

    while (file >> abc >> xyz)
    {
        auto they_play = map_ABC_to_option(abc), i_play = map_XYZ_to_option(xyz);
        auto outcome = get_my_outcome(they_play, i_play);
        part1 += unsigned(i_play) + unsigned(outcome);

        outcome = map_XYZ_to_outcome(xyz);
        they_play = map_ABC_to_option(abc), i_play = get_my_option(they_play, outcome);
        part2 += unsigned(i_play) + unsigned(outcome);
    }
}

int main()
{
    auto part1 = 0U, part2 = 0U;
    process_input("input.txt", part1, part2);

    assert(part1 == 10718);
    assert(part2 == 14652);
    return 0;
}