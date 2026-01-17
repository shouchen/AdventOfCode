#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

bool is_safe(const std::vector<int> &report, bool allow_skip, int start = 0, int prev_val = -1, int dirn = 0)
{
    if (start > report.size() - 1)
        return true;

    // Case 1: Not skipping this element
    if (prev_val == -1) // First element
    {
        if (is_safe(report, allow_skip, start + 1, report[start], dirn))
            return true;
    }
    else
    {
        auto delta = report[start] - prev_val, save_dirn = dirn;

        if (dirn == 0) // Second element (no direction yet)
        {
            if (delta > 0)
                dirn = 1;
            else if (delta < 0)
                dirn = -1;
        }

        if ((dirn == 1 && delta >= 1 && delta <= 3) ||
            (dirn == -1 && delta <= -1 && delta >= -3))
        {
            if (is_safe(report, allow_skip, start + 1, report[start], dirn))
                return true;
        }

        dirn = save_dirn; // undo direction if we're going to skipped this one 
    }

    // Case 2: Skipping this element
    return allow_skip && is_safe(report, false, start + 1, prev_val, dirn);
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
