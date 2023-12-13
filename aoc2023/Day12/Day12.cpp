#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

auto solve(std::string s, std::vector<int> v)
{
    auto max_run_len = *std::max_element(v.begin(), v.end());
    auto memo = std::vector<std::vector<std::vector<long long>>>(
        s.size(),
        std::vector<std::vector<long long>>(
            v.size() + 1,
            std::vector<long long>(max_run_len + 1, -1)));

    auto recur = [&](auto &self, int s_index, int v_index, int curr_run_len) -> long long
    {
        if (s_index >= s.size())
            return (v_index == v.size()) ? 1 : 0;

        auto &retval = memo[s_index][v_index][curr_run_len];
        if (retval != -1)
            return retval;

        retval = 0;
        auto curr = s[s_index], next = s[s_index + 1];

        if ((curr == '.' || curr == '?') && curr_run_len == 0)
            retval += self(self, s_index + 1, v_index, 0);

        if ((curr == '#' || curr == '?') && v_index < v.size())
        {
            if (curr_run_len < v[v_index] - 1)
                retval += self(self, s_index + 1, v_index, curr_run_len + 1);
            else if (s_index == s.size() - 1 || next != '#')
                retval += self(self, s_index + 2, v_index + 1, 0);
        }

        return retval;
    };
    
    return recur(recur, 0, 0, 0);
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    std::string s;
    auto total = 0LL;

    while (file >> s)
    {
        std::vector<int> v;
        auto comma = ',';
        auto n = 0;

        for (;;)
        {
            file >> n;
            v.push_back(n);

            if (file.peek() == ',')
                file >> comma;
            else
                break;
        }

        if (part2)
        {
            s = s + "?" + s + "?" + s + "?" + s + "?" + s;

            auto original = v;
            for (auto i = 0; i < 4; i++)
                v.insert(v.end(), original.begin(), original.end());
        }

        total += solve(s, v);
    }

    return total;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7032);
    assert(part2 == 1493340882140);
    return 0;
}
