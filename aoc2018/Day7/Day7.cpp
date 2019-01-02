#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <cassert>

using namespace std::string_literals;

std::map<char, std::vector<char>> prerequisites;
std::set<char> steps;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    auto step = "step"s, must = "must"s, be = "be"s, finished = "finished"s, before = "before"s, can = "can"s, begin = "begin"s;
    auto comma = ',', first_step = ' ', second_step = ' ';

    while (file >> step >> first_step >> must >> be >> finished >> before >> step >> second_step >> can >> begin)
    {
        prerequisites[second_step].push_back(first_step);
        steps.insert(first_step);
        steps.insert(second_step);
    }
}

auto contains_all_chars(const std::string &s, const std::vector<char> &chars)
{
    for (auto c : chars)
        if (s.find(c) == -1)
            return false;

    return true;
}

auto do_part1()
{
    auto path = ""s;

    while (path.length() < steps.size())
        for (auto next : steps)
            if (path.find(next) == -1 && contains_all_chars(path, prerequisites[next]))
            {
                path.push_back(next);
                break;
            }

    return path;
}

auto do_part2()
{
    struct LetterTime
    {
        char letter = '.';
        unsigned time = 0;
    };

    auto seconds = 0;
    auto path = ""s;
    std::set<char> used;
    std::array<LetterTime, 5> workers;

    while (path.length() < steps.size())
    {
        for (auto &worker : workers)
        {
            if (worker.time > 0)
            {
                if (--worker.time != 0)
                    continue;

                path.push_back(worker.letter);
                worker.letter = '.';
            }

            for (auto next : steps)
            {
                if (used.find(next) == used.end() &&
                    contains_all_chars(path, prerequisites[next]))
                {
                    worker.letter = next;
                    worker.time = 61 + next - 'A';
                    used.insert(next);
                    break;
                }
            }
        }

        seconds++;
    }

    return seconds - 1;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == "BITRAQVSGUWKXYHMZPOCDLJNFE");
    assert(part2 == 869);
    return 0;
}
