#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <array>
#include <string>
#include <cassert>

std::vector<int> deck;

void cut(std::vector<int> &v, int n)
{
    if (n < 0) n += v.size();

    for (int i = 0, j = n - 1; i < j; i++, j--)
    {
        auto temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }

    for (int i = n, j = v.size() - 1; i < j; i++, j--)
    {
        auto temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }

    for (int i = 0, j = v.size() - 1; i < j; i++, j--)
    {
        auto temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

auto deal_into_new_stack(std::vector<int> &v)
{
    std::vector<int> retval;

    while (!v.empty())
    {
        retval.push_back(v.back());
        v.pop_back();
    }

    return retval;
}

auto deal(std::vector<int> &v, int n)
{
    std::vector<int> retval(v.size());

    auto curr = 0;
    for (auto i : v)
    {
        retval[curr] = i;
        curr = (curr + n) % v.size();
    }

    return retval;
}

auto do_part1(int size)
{
    for (auto i = 0; i < size; i++)
        deck.push_back(i);

    std::queue<int> deck2;
    for (auto i : deck)
        deck2.push(i);

    std::ifstream file("input.txt");
    std::string s1, s2, s3;

    while (file >> s1)
    {
        if (s1 == "deal")
        {
            file >> s2 >> s3;
            if (s1 == "deal" && s2 == "with" && s3 == "increment")
            {
                auto n = 0;
                file >> n;
                deck = deal(deck, n);

            }
            else if (s1 == "deal" && s2 == "into" && s3 == "new")
            {
                file >> s1; // stack
                deck = deal_into_new_stack(deck);
            }
        }
        else if (s1 == "cut")
        {
            auto n = 0;
            file >> n;
            cut(deck, n);
        }
    }

    for (auto i = 0; i < int(deck.size()); i++)
        if (deck[i] == 2019)
            return i;

    return -1;
}

int main()
{
    auto part1 = do_part1(10007);
    std::cout << "Part One: " << part1 << std::endl;

    assert(part1 == 6129);
    return 0;
}
