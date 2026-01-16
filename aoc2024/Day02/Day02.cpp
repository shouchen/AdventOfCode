#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

auto is_safe(const std::vector<int> &report)
{
    int previous = report[0];
    bool increases = report[1] > report[0];

    for (auto i = 1; i < report.size(); i++)
    {
        auto delta = report[i] - previous;
        if (increases)
        {
            if (delta < 1 || delta > 3)
                return false;
        }
        else
        {
            if (delta > -1 || delta < -3)
                return false;
        }

        previous = report[i];
    }

    return true;
}

auto is_safe2(const std::vector<int> &report, int skip)
{
    std::vector<int> new_report;
    for (int i = 0; i < report.size(); i++)
        if (i != skip)
            new_report.push_back(report[i]);

    return is_safe(new_report);
}

auto is_safe2(const std::vector<int> &report)
{
    for (auto i = 0; i < report.size(); i++)
        if (is_safe2(report, i))
            return true;

    return false;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::pair<int, int> retval;

    while (std::getline(file, line))
    {
        std::vector<int> report;
        std::istringstream ss(line);
        auto n = 0;

        while (ss >> n)
            report.push_back(n);

        if (is_safe(report))
            retval.first++;

        if (is_safe2(report))
            retval.second++;
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 421);
    assert(answer.second == 476);
    return 0;
}
