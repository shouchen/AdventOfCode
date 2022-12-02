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

auto get_outcome(Option they_play, Option i_play)
{
    auto x = (2 * unsigned(they_play) + unsigned(i_play) + 1) % 3 * 3;
    return Outcome(x);
}

auto get_my_option(Option they_play, Outcome outcome)
{
    auto x = (unsigned(they_play) + unsigned(outcome) / 3 + 1) % 3 + 1;
    return Option(x);
}

void process_input(const std::string &filename, unsigned &part1, unsigned &part2)
{
    std::ifstream file(filename);
    auto abc = ' ', xyz = ' ';
    
    part1 = part2 = 0;

    while (file >> abc >> xyz)
    {
        auto they_play = map_ABC_to_option(abc), i_play = map_XYZ_to_option(xyz);
        auto outcome = get_outcome(they_play, i_play);
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

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 10718);
    assert(part2 == 14652);
    return 0;
}