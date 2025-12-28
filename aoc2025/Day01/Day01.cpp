#include <iostream>
#include <fstream>
#include <cassert>

auto solve(const std::string &filename)
{
    auto position = 50, count = 0;
    auto dirn = 'L';
    auto retval = std::make_pair(0, 0);

    std::ifstream file(filename);

    while (file >> dirn)
    {
        file >> count;

        while (count--)
        {
            if (dirn == 'L')
                position = position ? (position - 1) : 99;
            else
                position = (position == 99) ? 0 : (position + 1);

            if (position == 0)
                retval.second++;
        }

        if (position == 0)
            retval.first++;
    }

    return retval;
}

int main()
{
    auto solution = solve("input.txt");
    std::cout << "Part One: " << solution.first << std::endl;
    std::cout << "Part Two: " << solution.second  << std::endl;

    assert(solution.first == 980);
    assert(solution.second == 5961);
    return 0;
}
