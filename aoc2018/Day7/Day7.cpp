#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <cassert>

std::string do_part1()
{
    std::map<char, std::vector<char>> graph;
    std::map<char, std::vector<char>> graph_back;
    std::set<char> all;

    std::ifstream file("input.txt");

    auto line = 'A', comma = ',';
    auto x = 0U, y = 0U;
    std::string step, must, be, finished, before, can, begin;
    char first_step, second_step;

    while (file >> step >> first_step >> must >> be >> finished >> before >> step >> second_step >> can >> begin)
    {
        graph[first_step].push_back(second_step);
        graph_back[second_step].push_back(first_step);
        all.insert(first_step);
        all.insert(second_step);
    }

    std::set<char> next;
    std::string path;

    for (auto item : all)
    {
        if (graph_back.find(item) == graph_back.end())
            next.insert(item);
    }

    for (;;)
    {
        // check pre-reqs for each next, and pick lowest qualifying
        char lowest = 'z';
        for (auto item : next)
        {
            bool prereqs = true;
            for (auto pre : graph_back[item])
            {
                if (path.find(pre) == -1)
                {
                    prereqs = false;
                    break;
                }
            }

            if (!prereqs) 
                continue;

            if (item < lowest)
                lowest = item;
        }
        assert(lowest != 'z');

        // Follow the lowest available one
        path.push_back(lowest);
        next.erase(lowest);

        // Add all the next as options from the current one visited
        auto curr = path[path.length() - 1];
        for (auto &item : graph[curr])
        {
            next.insert(item);
        }

        // Stop when no more
        if (next.empty())
            break;
    }

    return path;
}

struct LetterTime
{
    char letter = '.';
    unsigned time = 0;
};

unsigned do_part2()
{
    std::ifstream file("input.txt");

    auto line = 'A', comma = ',';
    auto x = 0U, y = 0U;
    std::string step, must, be, finished, before, can, begin;
    char first_step, second_step;

    std::map<char, std::vector<char>> prerequisites;
    std::set<char> steps;

    while (file >> step >> first_step >> must >> be >> finished >> before >> step >> second_step >> can >> begin)
    {
        prerequisites[second_step].push_back(first_step);
        steps.insert(first_step);
        steps.insert(second_step);
    }

    // Now, process the data that was read

    std::string path;

    auto seconds = 0U;
    LetterTime worker_status[5];

    for (;;)
    {
        for (int w = 0; w < 5; w++)
        {
            if (worker_status[w].time > 0)
            {
                if (--worker_status[w].time != 0)
                    continue;

                path.push_back(worker_status[w].letter);
                worker_status[w].letter = '.';
            }

            for (char mynext = 'A'; mynext <= 'Z'; mynext++)
            {
                if (steps.find(mynext) == steps.end())
                    continue;

                bool prereqs = true;
                for (auto pre : prerequisites[mynext])
                {
                    if (path.find(pre) == -1)
                    {
                        prereqs = false;
                        break;
                    }
                }

                if (!prereqs)
                    continue;

                steps.erase(mynext);

                worker_status[w].letter = mynext;
                worker_status[w].time = 61 + mynext - 'A';
                break;
            }
        }

        if (path.length() == 26)
            return seconds;

        seconds++;
    }
}

int main()
{
    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == "BITRAQVSGUWKXYHMZPOCDLJNFE");
    assert(part2 == 869);
    return 0;
}
