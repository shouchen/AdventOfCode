#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <cassert>

auto find_min_button_presses1(unsigned goal, const std::vector<unsigned int> &buttons)
{
    struct Elem
    {
        unsigned state = 0x0;
        unsigned presses = 0;
    };

    std::set<unsigned> seen;

    std::queue<Elem> q;
    q.push(Elem());

    while (!q.empty())
    {
        auto elem = q.front(); q.pop();

        if (elem.state == goal)
            return elem.presses;

        if (seen.find(elem.state) != seen.end())
            continue;

        seen.insert(elem.state);

        for (auto button : buttons)
        {
            auto new_state = elem.state ^ button;
            q.push(Elem{ new_state, elem.presses + 1 });
        }
    }

    return 0U;
}

auto find_min_button_presses2(const std::vector<unsigned int> &joltages, const std::vector<unsigned int> &buttons)
{
    struct Elem
    {
        std::vector<unsigned> joltages;
        unsigned presses = 0;
    };

    std::set<std::vector<unsigned>> seen;

    std::queue<Elem> q;
    auto temp = std::vector<unsigned>(joltages.size(), 0);
    q.push(Elem{ temp });

    while (!q.empty())
    {
        auto elem = q.front(); q.pop();

        if (elem.joltages == joltages)
            return elem.presses;

        for (int i = 0; i < elem.joltages.size(); i++)
            if (elem.joltages[i] > joltages[i])
                continue;

        if (seen.find(elem.joltages) != seen.end())
            continue;

        seen.insert(elem.joltages);

        for (auto button : buttons)
        {
            auto new_joltages = std::vector<unsigned>();
            for (auto i = 0; i < elem.joltages.size(); i++)
            {
                if (button & (0x1 << (elem.joltages.size() - 1 - i)))
                    new_joltages.push_back(elem.joltages[i] + 1);
                else
                    new_joltages.push_back(elem.joltages[i]);
            }

            q.push(Elem{ new_joltages, elem.presses + 1 });
        }
    }

    return 0U;
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    std::string word;
    auto comma = ',', parenth = '(', brace = '{', token = ' ';
    auto n = 0, num_indicators = 0;

    auto buttons = std::vector<unsigned int>{};
    auto joltages = std::vector<unsigned int>{};
    auto goal = 0U;
    unsigned retval = 0U;

    while (file >> word)
    {
        if (word.front() == '[')
        {
            goal = 0U;
            num_indicators = int(word.length()) - 2;
            for (auto c : word)
            {
                if (c == '#')
                    goal = (goal <<  1) | 0x1;
                if (c == '.')
                    goal <<= 1;
            }
        }
        else if (word.front() == '(')
        {
            auto button = 0U;
            std::stringstream ss(word);

            ss >> parenth;
            while (ss >> n)
            {
                auto temp = 0x1 << (num_indicators - 1 - n);
                button |= temp;
                ss >> token;
            }

            buttons.push_back(button);
        }
        else // word.front() == '{'
        {
            auto joltage = 0U;
            std::stringstream ss(word);

            ss >> brace;
            while (ss >> n)
            {
                joltages.push_back(n);
                ss >> token;
            }

            retval += part2
                ? find_min_button_presses2(joltages, buttons)
                : find_min_button_presses1(goal, buttons);

            buttons.clear();
            joltages.clear();
        }
    }

    return retval;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;
    //assert(part1 == 449);

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
