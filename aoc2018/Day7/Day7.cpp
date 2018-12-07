#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <list>
#include <cassert>

std::map<char, std::vector<char>> graph;
std::map<char, std::vector<char>> graph_back;
std::set<char> all;

std::string do_part1()
{
    std::ifstream file("input.txt");

    auto line = 'A', comma = ',';
    auto x = 0U, y = 0U;
    std::string step, must, be, finished, before, can, begin;
    char first_step, second_step;

    //Step C must be finished before step A can begin.
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
        std::cout << lowest << std::endl;

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
    char letter = ' ';
    unsigned time = 0;
};

std::string do_part2()
{
    std::ifstream file("input.txt");

    auto line = 'A', comma = ',';
    auto x = 0U, y = 0U;
    std::string step, must, be, finished, before, can, begin;
    char first_step, second_step;

    //Step C must be finished before step A can begin.
    while (file >> step >> first_step >> must >> be >> finished >> before >> step >> second_step >> can >> begin)
    {
        graph[first_step].push_back(second_step);
        graph_back[second_step].push_back(first_step);
        all.insert(first_step);
        all.insert(second_step);
    }

    //std::set<char> next;
    std::string path;

    //for (auto item : all)
    //{
    //    if (graph_back.find(item) == graph_back.end())
    //        next.insert(item);
    //}

    int seconds = 0;
    LetterTime worker[2];
    std::set<char> taken;

    for (;;)
    {
        for (int w = 0; w < 2; w++)
        {
            if (worker[w].time > 0)
            {
                worker[w].time--;
                if (worker[w].time == 0)
                {
                    path.push_back(worker[w].letter);
                    std::cout << path << std::endl;

                    //// Add all the next as options from the current one visited
                    //auto curr = worker[w].letter;
                    //for (auto &item : graph[curr])
                    //{
                    //    next.insert(item);
                    //}
                }
                else
                    continue;
            }

            for (char mynext = 'A'; mynext <= 'F'; mynext++)
            {
                bool prereqs = true;
                for (auto pre : graph_back[mynext])
                {
                    if (path.find(pre) == -1)
                    {
                        prereqs = false;
                        break;
                    }
                }

                if (!prereqs)
                    continue;

                if (taken.find(mynext) != taken.end())
                    continue;

                taken.insert(mynext);
                //next.erase(mynext);

                worker[w].letter = mynext;
                worker[w].time = /*60 +*/ (mynext - 'A' + 1);
                std::cout << "@second " << seconds << " worker " << w << " takes " << mynext << std::endl;
                break;
            }
        }
        // Stop when no more
        if (path.length() == 6)
            break;

        seconds++;
    }

    return path;
}

int main()
{
    //auto part1 = do_part1();
    auto part2 = do_part2();

    //std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == "BITRAQVSGUWKXYHMZPOCDLJNFE");
    assert(part2 == "BITRAQVSGUWKXYHMZPOCDLJNFE"); // 1847 too high
    return 0;
}
