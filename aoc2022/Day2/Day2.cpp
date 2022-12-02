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

auto score_me1(char abc, char xyz)
{
    auto they_play = map_ABC_to_option(abc), i_play = map_XYZ_to_option(xyz);
    auto outcome = get_my_outcome(they_play, i_play);
    return unsigned(i_play) + unsigned(outcome);
}

auto score_me2(char abc, char xyz)
{
    Outcome outcome = map_XYZ_to_outcome(xyz);
    Option they_play = map_ABC_to_option(abc), i_play = get_my_option(they_play, outcome);
    return unsigned(i_play) + unsigned(outcome);
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

        //part1 += score_me1(c1, c2);
        //part2 += score_me2(c1, c2);
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
