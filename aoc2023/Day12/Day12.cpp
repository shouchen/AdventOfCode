#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using Memo = std::vector<std::vector<std::vector<long long>>>;

long long recur(int s_index, int v_index, int curr_run_len, std::string &s, std::vector<int> &v, Memo &memo)
{
    if (s_index >= s.size())
        return (v_index == v.size()) ? 1 : 0;

    auto &retval = memo[s_index][v_index][curr_run_len];
    if (retval != -1)
        return retval;

    retval = 0;

    // Place a '.' (reset curr_run_len)
    if ((s[s_index] == '.' || s[s_index] == '?') && curr_run_len == 0)
        retval += recur(s_index + 1, v_index, 0, s, v, memo);

    // Place a '#' (increment cur / increment v_index and reset cur)
    if ((s[s_index] == '#' || s[s_index] == '?') && v_index < v.size())
    {
        if (curr_run_len + 1 < v[v_index])
            retval += recur(s_index + 1, v_index, curr_run_len + 1, s, v, memo);
        else if (s_index + 1 == s.size() || s[s_index + 1] != '#')
            retval += recur(s_index + 2, v_index + 1, 0, s, v, memo);
    }

    return retval;
}

auto solve(std::string s, std::vector<int> v)
{
    // memo space is small! [size of input str][# blocks + 1][max run len + 1]
    auto max_run_len = *max_element(v.begin(), v.end());
    Memo memo(
        s.size(),
        std::vector<std::vector<long long>>(
            v.size() + 1,
            std::vector<long long>(max_run_len + 1, -1)));

    return recur(0, 0, 0, s, v, memo);
}

auto do_part2(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    auto total = 0LL;
    std::string data;

    while (file >> data)
    {
        std::vector<int> blocks;
        auto comma = ',';
        auto n = 0;

        for (;;)
        {
            file >> n;
            blocks.push_back(n);
            if (file.peek() == ',')
                file >> comma;
            else
                break;
        }

        if (part2)
        {
            auto temp = data;
            data = temp + "?" + temp + "?" + temp + "?" + temp + "?" + temp;

            auto bb = blocks;
            for (auto i = 0; i < 4; i++)
                for (auto b : bb)
                    blocks.push_back(b);
        }

        total += solve(data, blocks);
    }

    return total;
}

int main()
{
    auto part1 = do_part2("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7032);
    assert(part2 == 1493340882140);
    return 0;
}
