#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <array>
#include <queue>
#include <algorithm>

auto process_data(const std::string &filename, bool part2)
{
    std::array<std::deque<char>, 10> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line[1] != '1')
        for (auto i = 1, stack = 1; i < line.length(); i += 4, stack++)
            if (line[i] != ' ')
                data[stack].push_back(line[i]);

    std::string move, from, to;
    auto quantity = 0, from_stack = 0, to_stack = 0;

    while (file >> move >> quantity >> from >> from_stack >> to >> to_stack)
    {
        std::string removed_creates;

        for (auto i = 0; i < quantity; i++)
        {
            removed_creates.push_back(data[from_stack].front());
            data[from_stack].pop_front();
        }

        if (part2)
            std::reverse(removed_creates.begin(), removed_creates.end());

        for (auto j = removed_creates.begin(); j != removed_creates.end(); j++)
            data[to_stack].push_front(*j);
    }

    std::string retval;
    for (auto i = 1; i < data.size(); i++)
        retval.push_back(data[i].front());

    return retval;
}

int main()
{
    auto part1 = process_data("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_data("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == "HBTMTBSDC");
    assert(part2 == "PQTJRSHWS");
    return 0;
}
