#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include <cassert>

struct TableRow
{
    unsigned guard = 0;
    char minute[60] = { ' ' };
};

std::map<std::pair<unsigned, unsigned>, TableRow> table; // maps month+day -> TableRow

void read_input(const std::string &filename)
{
    static const unsigned days_in_month[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

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
                hour = minute = 0;
                day++;
                if (day > days_in_month[month])
                {
                    month++;
                    day = 1;
                }
            }

            table[std::make_pair(month, day)].guard = id;
        }
        else if (s == "falls")
        {
            file >> asleep;
            table[std::make_pair(month, day)].minute[minute] = 's';
        }
        else if (s == "wakes")
        {
            file >> up;
            table[std::make_pair(month, day)].minute[minute] = 'w';
        }
    }

    for (auto row : table)
    {
        bool awake = true;
        for (auto &c : table[row.first].minute)
        {
            if (c == 'w') awake = true;
            else if (c == 's') awake = false;
            else c = awake ? 'w' : 's';
        }
    }
}

unsigned do_part1()
{
    std::map<unsigned, unsigned> total_sleep_per_guard;
    for (auto item : table)
    {
        auto &table_row = item.second;
        auto id = table_row.guard;

        for (auto c : table_row.minute)
            if (c == 's')
                total_sleep_per_guard[id]++;
    }

    auto sleepiest1 = std::max_element(
        total_sleep_per_guard.begin(), 
        total_sleep_per_guard.end(),
        [&](std::pair<const unsigned, unsigned> const &lhs,
            std::pair<const unsigned, unsigned> const &rhs) {
        return lhs.second < rhs.second;
        });
    auto sleepiest_guard = sleepiest1->first;

    // Get cumulative data for the sleepiest guard
    std::array<unsigned, 60> asleep_count;
    for (auto item : table)
    {
        auto date = item.first;
        if (table[date].guard == sleepiest_guard)
        {
            for (int i = 0; i < 60; i++)
            {
                if (table[date].minute[i] == 's')
                    asleep_count[i]++;
            }
        }
    }

    // Now, what minute is the sleepiest guard the sleepiest?
    auto sleepiest2 = std::max_element(asleep_count.begin(), asleep_count.end());
    auto sleepiest_minute = static_cast<unsigned>(std::distance(asleep_count.begin(), sleepiest2));

    return sleepiest_minute * sleepiest_guard;
}

unsigned do_part2()
{
    std::map<std::pair<unsigned, unsigned>, unsigned> guard_and_minute_to_sleep_count;
    for (auto &item : table)
    {
        auto date = item.first;
        auto id = table[date].guard;

        for (int i = 0; i < 60; i++)
            if (table[date].minute[i] == 's')
                guard_and_minute_to_sleep_count[std::make_pair(id, i)]++;
    }

    auto sleepiest = std::max_element(
        guard_and_minute_to_sleep_count.begin(),
        guard_and_minute_to_sleep_count.end(),
        [&](std::pair<const std::pair<unsigned, unsigned>, unsigned> const &lhs, 
            std::pair<const std::pair<unsigned, unsigned>, unsigned> const &rhs) {
            return lhs.second < rhs.second;
        });

    auto sleepy_guard = sleepiest->first.first;
    auto sleepy_minute = sleepiest->first.second;
    return sleepy_guard * sleepy_minute;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 39584);
    assert(part2 == 55053);
    return 0;
}
