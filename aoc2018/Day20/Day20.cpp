#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <numeric>
#include <algorithm>
#include <cassert>

auto left = 0, right = 0, top = 0, bottom = 0;

struct Dirs
{
    bool north = false, south = false, east = false, west = false;
    Dirs() : north(false), south(false), east(false), west(false) {}
    Dirs(bool north, bool south, bool east, bool west) : north(north), south(south), east(east), west(west) {}
};

std::map<std::pair<int, int>, Dirs> grid;

void dump()
{
    for (int i = 0; i < right - left; i++)
        std::cout << "##";
    std::cout << "###" << std::endl;

    for (int y = top; y <= bottom; y++)
    {
        std::cout << "#";
        for (int x = left; x <= right; x++)
        {
            std::cout << ((x == 0 && y == 0) ? "X" : ".") << (grid[std::make_pair(x, y)].east ? '|' : '#');
        }
        std::cout << std::endl << "#";
        for (int x = left; x <= right; x++)
        {
            std::cout << (grid[std::make_pair(x, y)].south ? '-' : '#') << '#';
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void move_north(int &curr_x, int &curr_y)
{
    grid[std::make_pair(curr_x, curr_y)].north = true;
    grid[std::make_pair(curr_x, --curr_y)].south = true;
}

void do_recur(const char *regex, int curr_x, int curr_y)
{
    left = std::min(left, curr_x);
    right = std::max(right, curr_x);
    top = std::min(top, curr_y);
    bottom = std::max(bottom, curr_y);

char c;
while (c = *regex++)
{
    switch (c)
    {
    case 'N':
        grid[std::make_pair(curr_x, curr_y)].north = true;
        grid[std::make_pair(curr_x, --curr_y)].south = true;
        break;
    case 'S':
        grid[std::make_pair(curr_x, curr_y)].south = true;
        grid[std::make_pair(curr_x, ++curr_y)].north = true;
        break;
    case 'E':
        grid[std::make_pair(curr_x, curr_y)].east = true;
        grid[std::make_pair(++curr_x, curr_y)].west = true;
        break;
    case 'W':
        grid[std::make_pair(curr_x, curr_y)].west = true;
        grid[std::make_pair(--curr_x, curr_y)].east = true;
        break;
    case '(':
        do_recur(regex, curr_x, curr_y);
        break;
    case ')':
        continue;
    case '|': // Skip next alternative and proceed after matching ')'
    {
        int level = 1;
        for (;;)
        {
            c = *regex++;
            if (c == '(') level++;
            if (c == ')') level--;
            if (c == ')' && level == 0)
                break;
        }
        c = *regex++;
    }
    case '$': // should return from the very last one this way
        return;
    default:
        assert(false);
    }

    left = std::min(left, curr_x);
    right = std::max(right, curr_x);
    top = std::min(top, curr_y);
    bottom = std::max(bottom, curr_y);
}
}

struct WorkItem
{
    int x, y;
    unsigned distance;
};

auto part1 = 0U;

void do_part1()
{
    std::queue<WorkItem> q;
    std::map<std::pair<int, int>, unsigned> dist;

    q.push(WorkItem{ 0, 0, 0 });

    while (!q.empty())
    {
        auto wi = q.front();
        q.pop();

        if (dist.find(std::make_pair(wi.x, wi.y)) != dist.end())
            continue;

        dist[std::make_pair(wi.x, wi.y)] = wi.distance;
        part1 = std::max(part1, wi.distance);

        auto dirs = grid[std::make_pair(wi.x, wi.y)];

        if (dirs.north)
            q.push(WorkItem{ wi.x, wi.y - 1, wi.distance + 1 });
        if (dirs.south)
            q.push(WorkItem{ wi.x, wi.y + 1, wi.distance + 1 });
        if (dirs.east)
            q.push(WorkItem{ wi.x + 1, wi.y, wi.distance + 1 });
        if (dirs.west)
            q.push(WorkItem{ wi.x - 1, wi.y, wi.distance + 1 });
    }

    for (auto y = top; y <= bottom; y++)
    {
        for (auto x = left; x <= right; x++)
            std::cout << std::setw(2) << dist[std::make_pair(x, y)] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void process_sequence(const char *sequence, std::pair<int, int> start);

void process_open_parenth(const char *sequence, std::pair<int, int> start)
{
    // parenthesized sequence
    for (;;)
    {
        // between pipes
        for (;;)
        {
            if (*sequence >= 'A' && *sequence <= 'Z')
            {
                std::cout << *sequence;
                sequence++;
                continue;
            }
            else
                break;
        }

        if (*sequence == '(')
            process_open_parenth(++sequence, start);
        else if (*sequence == '|')
            process_sequence(++sequence, start);
    }
}

void process_sequence(const char *sequence, std::pair<int, int> start)
{
    for (;;)
    {
        if (*sequence >= 'A' && *sequence <= 'Z')
        {
            std::cout << *sequence;
            sequence++;
            continue;
        }
        else
            break;
    }

    if (*sequence == '(')
        process_open_parenth(sequence, start);
}

int main()
{
    std::ifstream file("input.txt");
    char caret;
    std::string regex;
    file >> caret >> regex;

    grid[std::make_pair(0, 0)] = Dirs{ false, false, false, false };

    //do_recur(regex.c_str(), 0, 0);
    regex = "ABC(D|E(FG|H|)IJ)KLM$";
    process_sequence(regex.c_str(), std::make_pair(0, 0));

    dump();

    do_part1();

    std::cout << "Part 1: " << part1 << std::endl;

    return 0;
}
// 4501 part 1
// 8623 part 2
