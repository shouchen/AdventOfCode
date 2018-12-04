#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cassert>

static const unsigned days_in_month[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

struct TableRow
{
    unsigned guard = 0;
    char minute[60] = { ' ' };
};

std::map<std::pair<unsigned, unsigned>, TableRow> table; // maps m+d -> TableRow

std::map<std::pair<unsigned, unsigned>, unsigned> guard; // m+d -> guard
std::map<std::pair<unsigned, unsigned>, std::string> grid; // grid

std::map<unsigned, unsigned> total_sleep;

void start_shift(const std::string &filename)
{
    std::ifstream file(filename);
    unsigned year, month, day, hour, minute;
    char dash, colon, open_brace, close_brace;
    std::string a;

    unsigned id;

    while (file >> open_brace >> year >> dash >> month >> dash >> day >> hour >> colon >> minute >> close_brace)
    {
        std::string s, begins, shift, asleep, up;
        char pound;

        file >> s;

        if (s == "Guard")
        {
            file >> pound >> id >> begins >> shift;
            if (hour == 23)
            {
                hour = 0;
                minute = 0;
                day++;
                if (day > days_in_month[month])
                {
                    month++;
                    day = 1;
                }
            }

            auto date = std::make_pair(month, day);

            guard[date] = id;
            grid[date] = "                                                            "; // 60

            table[date].guard = id;
        }
        else if (s == "falls")
        {
            file >> asleep;
        }
        else if (s == "wakes")
        {
            file >> up;
        }
    }
}

void sleep_wake(const std::string &filename)
{
    std::ifstream file(filename);
    unsigned year, month, day, hour, minute;
    char dash, colon, open_brace, close_brace;
    std::string a;

    unsigned id;

    while (file >> open_brace >> year >> dash >> month >> dash >> day >> hour >> colon >> minute >> close_brace)
    {
        std::string s, begins, shift, asleep, up;
        char pound;

        file >> s;

        if (s == "Guard")
        {
            file >> pound >> id >> begins >> shift;
        }
        else if (s == "falls")
        {
            file >> asleep;
            grid[std::make_pair(month, day)][minute] = 's';
            table[std::make_pair(month, day)].minute[minute] = 's';
        }
        else if (s == "wakes")
        {
            file >> up;
            grid[std::make_pair(month, day)][minute] = 'w';
            table[std::make_pair(month, day)].minute[minute] = 'w';
        }
    }
}

unsigned do_part1()
{
    for (auto item : grid)
    {
        auto date = item.first;
        auto id = guard[date];
        auto &s = grid[date];

        bool awake = true;
        unsigned count = 0;
        for (int i = 0; i < 60; i++)
        {
            if (s[i] == 'w') awake = true;
            else if (s[i] == 's') awake = false;
            else s[i] = awake ? 'w' : 's';

            if (!awake)
                count++;
        }

        total_sleep[id] += count;
    }

    unsigned highest_sleep = 0;
    unsigned highest_id = 0;
    for (auto item : total_sleep)
    {
        if (item.second > highest_sleep)
        {
            highest_sleep = item.second;
            highest_id = item.first;
        }
    }

    std::cout << highest_sleep << " " << highest_id << std::endl;

    // Now what minute is guard #id asleep the most?
    unsigned asleep_count[60];
    for (int i = 0; i < 60; i++)
        asleep_count[i] = 0;

    for (auto item : grid)
    {
        auto date = item.first;
        if (guard[date] == highest_id)
        {
            for (int i = 0; i < 60; i++)
            {
                if (grid[date][i] == 's')
                    asleep_count[i]++;
            }
        }
    }

    unsigned sleepiest_minute = 0;
    unsigned sleepy_count = 0;
    for (int i = 0; i < 60; i++)
    {
        if (asleep_count[i] > sleepy_count)
        {
            sleepy_count = asleep_count[i];
            sleepiest_minute = i;
        }
    }

    return sleepiest_minute * highest_id;
}

unsigned do_part2()
{
    // build map of (guard) -> count of sleep at each minute
    std::map<unsigned, unsigned[60]> p2;

    for (auto item : grid)
    {
        auto date = item.first;
        auto id = guard[date];

        for (int i = 0; i < 60; i++)
            if (grid[date][i] == 's')
            {
                p2[id][i]++;
            }
    }

    unsigned sleepy_guard = 0;
    unsigned sleepy_guard_sleepiness = 0;
    unsigned sleepy_minute = 0;
    for (auto item : p2)
    {
        for (int i = 0; i < 60; i++)
        {
            if (item.second[i] > sleepy_guard_sleepiness)
            {
                sleepy_guard_sleepiness = item.second[i];
                sleepy_guard = item.first;
                sleepy_minute = i;
            }
        }
    }

    return sleepy_guard * sleepy_minute;
}

int main()
{
    start_shift("input.txt");
    sleep_wake("input.txt");

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 39584);
    assert(part2 == 55053);
    return 0;
}
