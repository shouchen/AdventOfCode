#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

auto is_safe(const std::vector<int> &report, bool allow_skip, int start = 0, int prev_val = -1, int dirn = 0)
{
    if (start > report.size() - 1)
        return true;

    // case 1: skipping this element
    if (allow_skip && is_safe(report, false, start + 1, prev_val, dirn))
        return true;

    // case 2: not skipping this element
    auto in_range = true;

    if (prev_val > -1) // not first element
    {
        auto delta = report[start] - prev_val;
        if (!dirn) dirn = (delta > 0) ? 1 : -1;

        in_range =
            (dirn == 1 && delta >= 1 && delta <= 3) ||
            (dirn == -1 && delta <= -1 && delta >= -3);
    }

    return in_range && is_safe(report, allow_skip, start + 1, report[start], dirn);
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

        if (is_safe(report, false))
            retval.first++;

        if (is_safe(report, true))
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
