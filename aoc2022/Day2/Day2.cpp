#include <iostream>
#include <fstream>
#include <cassert>

enum Play { Rock = 1, Paper = 2, Scissors = 3 };
enum Outcome { Lose = 0, Draw = 3, Win = 6 };

auto get_outcome(Play them, Play me)
{
    return Outcome((2 * them + me + 1) % 3 * 3);
}

auto get_play(Play them, Outcome outcome)
{
    return Play((them + outcome / 3 + 1) % 3 + 1);
}

void process_input(const std::string &filename, unsigned &part1, unsigned &part2)
{
    std::ifstream file(filename);
    auto abc = ' ', xyz = ' ';
    
    while (file >> abc >> xyz)
    {
        auto them = Play(abc - 'A' + 1);
        auto me = Play(xyz - 'X' + 1);
        auto outcome = get_outcome(them, me);
        part1 += me + outcome;

        outcome = Outcome((xyz - 'X') * 3);
        me = get_play(them, outcome);
        part2 += me + outcome;
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