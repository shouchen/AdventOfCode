#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>

std::vector<std::vector<int>> datas;

void read_input_lines(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, word;

    while (std::getline(file, line))
    {
        std::istringstream is(line);

        datas.push_back(std::vector<int>());
        int n = 0;
        while (is >> n)
            datas.back().push_back(n);
    }
}

int predict(std::vector<int> &v)
{
    std::vector<std::vector<int>> pyr;
    pyr.push_back(v);

    for (int j = 1; j < 1000; j++)
    {
        std::vector<int> next;
        for (int i = 0; i < pyr[j-1].size() - 1; i++)
            next.push_back(pyr[j-1][i + 1] - pyr[j-1][i]);

        pyr.push_back(next);
        if (next.size() == 1)
            break;
    }

    pyr.back().push_back(pyr.back().front());

    for (int i = pyr.size() - 2; i >= 0; --i)
    {
        pyr[i].push_back(pyr[i].back() + pyr[i + 1].back());
    }
        

    return pyr[0].back();
}

auto do_part1()
{
    auto total = 0LL;
    for (auto &i : datas)
        total += predict(i);

    return total;
}

auto do_part2()
{
    auto total = 0LL;
    for (auto &i : datas)
    {
        std::reverse(i.begin(), i.end());
        total += predict(i);
    }

    return total;
}

int main()
{
    read_input_lines("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 1819125966);
    assert(part2 == 1140);
    return 0;
}

