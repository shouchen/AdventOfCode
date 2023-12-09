#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>

using Series = std::vector<int>;

auto predict(const std::vector<int> &v)
{
    std::vector<Series> pyr;
    pyr.push_back(v);

    for (int j = 1; j < 1000; j++)
    {
        Series next;
        for (int i = 0; i < pyr[j-1].size() - 1; i++)
            next.push_back(pyr[j-1][i + 1] - pyr[j-1][i]);

        pyr.push_back(next);
        if (next.size() == 1)
            break;
    }

    pyr.back().push_back(pyr.back().front());

    for (auto i = int(pyr.size()) - 2; i >= 0; --i)
        pyr[i].push_back(pyr[i].back() + pyr[i + 1].back());

    return pyr[0].back();
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    auto retval = std::make_pair(0, 0);

    while (std::getline(file, line))
    {
        std::istringstream is(line);
        auto n = 0;
        Series series;

        while (is >> n)
            series.push_back(n);

        retval.first += predict(series);

        std::reverse(series.begin(), series.end());
        retval.second += predict(series);
    }

    return retval;
}

int main()
{
    auto answer = do_part1("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 1819125966);
    assert(answer.second == 1140);
    return 0;
}

