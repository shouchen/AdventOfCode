#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

std::map<std::string, std::vector<std::string>> m;

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string dirs, line, word, a, b;
    char equals;

    file >> dirs;

    while (file >> a >> equals)
    {
        m[a] = std::vector<std::string>();

        while (file >> b)
        {
            bool done = false;
            if (b[0] == '(') b = b.substr(1);
            if (b.back() == ')')
                done = true;
            if (b.back() == ',' || b.back() == ')')
                b.pop_back();
            m[a].push_back(b);
            if (done)
                break;
        }
    }

    int steps = 0;
    std::string curr = "AAA";

    for (;;)
    {
        for (auto d : dirs)
        {
            steps++;

            if (d == 'L')
                curr = m[curr][0];
            else
                curr = m[curr][1];

            if (curr == "ZZZ")
                return steps;
        }
    }
}

long long gcd(long long a, long long b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

long long findlcm(std::vector<int> v)
{
    // Initialize result
    long long ans = v[0];

    // ans contains LCM of arr[0], ..arr[i]
    // after i'th iteration,
    for (int i = 1; i < v.size(); i++)
        ans = (((v[i] * ans)) /
            (gcd(v[i], ans)));

    return ans;
}


auto do_part2(const std::string &filename)
{
    m.clear();

    std::ifstream file(filename);
    std::string dirs, line, word, a, b;
    char equals;

    file >> dirs;

    while (file >> a >> equals)
    {
        m[a] = std::vector<std::string>();

        while (file >> b)
        {
            bool done = false;
            if (b[0] == '(') b = b.substr(1);
            if (b.back() == ')')
                done = true;
            if (b.back() == ',' || b.back() == ')')
                b.pop_back();
            m[a].push_back(b);
            if (done)
                break;
        }
    }

    int steps = 0;

    std::vector<std::string> curr;
    for (auto &i : m)
    {
        if (i.first.back() == 'A')
            curr.push_back(i.first);
    }

    std::vector<int> cycle_len;
    for (int i = 0; i < curr.size(); i++)
        cycle_len.push_back(-1);

    for (;;)
    {
        for (auto d : dirs)
        {
            steps++;

            if (d == 'L')
            {
                for (int i = 0; i < curr.size(); i++)
                {
                    curr[i] = m[curr[i]][0];
                    if (cycle_len[i] == -1 && curr[i].back() == 'Z')
                        cycle_len[i] = steps;
                }
            }
            else
            {
                for (int i = 0; i < curr.size(); i++)
                {
                    curr[i] = m[curr[i]][1];
                    if (cycle_len[i] == -1 && curr[i].back() == 'Z')
                        cycle_len[i] = steps;
                }
            }

            bool done = true;
            long long product = 1;
            for (auto c : cycle_len)
            {
                if (c == -1)
                {
                    done = false;
                    break;
                }
                //product *= c;
            }

            if (done)
                return findlcm(cycle_len);

            //bool done = true;
            //for (auto &i : curr)
            //    if (i.back() != 'Z')
            //    {
            //        done = done = false;
            //        break;
            //    }

            //if (done)
            //    return steps;
        }
    }
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 11309);
    assert(part2 == 13740108158591);
    return 0;
}
