#include <iostream>
#include <vector>
#include <map>
#include <cassert>

std::vector<int> input{ 0, 14, 1, 3, 7, 9 };

auto process_input(int target)
{
    std::map<int, int> most_recent_index_for_value;
    auto number = 0, next_number = 0;

    for (auto index = 1; index <= target; index++)
    {
        number = (index <= input.size()) ? input[index - 1] : next_number;

        auto historical = most_recent_index_for_value.find(number);
        next_number = (historical == most_recent_index_for_value.end()) ? 0 : index - historical->second;

        most_recent_index_for_value[number] = index;
    }

    return number;
}

int main()
{
    auto part1 = process_input(2020);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_input(30000000);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 763);
    assert(part2 == 1876406);
    return 0;
}
