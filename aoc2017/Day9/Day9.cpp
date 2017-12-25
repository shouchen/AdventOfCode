#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>

auto total_points = 0U, total_trash = 0U;

void process_rest_of_trash_after_open_bracket(std::ifstream &f)
{
    auto c = ' ';
    while ((f >> c) && (c != '>'))
    {
        if (c == '!')
            f >> c;
        else
            total_trash++;
    }
}

void process_rest_of_group_after_open_brace(std::ifstream &f, int points)
{
    auto c = ' ';
    while ((f >> c) && (c != '}'))
    {
        if (c == '<')
            process_rest_of_trash_after_open_bracket(f);
        else if (c == '{')
            process_rest_of_group_after_open_brace(f, points + 1);
    }

    total_points += points;
}

void solve_problem(const std::string &filename)
{
    std::ifstream f("input.txt");

    auto open_brace = '{';
    f >> open_brace;

    process_rest_of_group_after_open_brace(f, 1);
}

int main()
{
    solve_problem("input.txt");

    std::cout << "Part One: " << total_points << std::endl;
    std::cout << "Part Two: " << total_trash << std::endl;

    assert(total_points == 16869);
    assert(total_trash == 7284);
    return 0;
}
