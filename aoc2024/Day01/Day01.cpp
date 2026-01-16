#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<int> left, right;
    std::map<int, int> right_freq;
    std::pair<int, int> retval;
    auto n1 = 0, n2 = 0;

    while (file >> n1 >> n2)
    {
        left.push_back(n1);
        right.push_back(n2);
        right_freq[n2]++;
    }

    for (auto i : left)
        retval.second += i * right_freq[i];

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    for (auto i = 0; i < left.size(); i++)
        retval.first += std::abs(left[i] - right[i]);

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 936063);
    assert(answer.second == 23150395);
    return 0;
}
