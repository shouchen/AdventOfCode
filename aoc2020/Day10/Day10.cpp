#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

std::vector<long long> data;

void initialize_data(const std::string &filename)
{
    std::ifstream file(filename);
    auto n = 0LL;

    data.push_back(0);
    while (file >> n)
        data.push_back(n);

    std::sort(data.begin(), data.end());
    data.push_back(data[data.size() - 1] + 3);
}

auto count_paths_from_start_to_end(int start, int end) 
{
    if (start == end)
        return 1U;

    auto count = 0U;
    for (auto i = 1; i <= 3; i++)
        if ((start + i) < data.size() && (data[start + i] - data[start]) <= 3)
            count += count_paths_from_start_to_end(start + i, end);

    return count;
}

// Segment data into runs that are separated by a gap of three. From this, it's possible to know
// how many deltas of one and three there are for part one. Also, for part two, each segment's
// arrangements can be considered in isolation from the other segments.
auto process_data()
{
    auto product = 1ULL;
    auto start = 0, end = 1, threes = 0;
    
    for (auto i = 1; i < data.size(); i++)
    {
        // Segment original list by the three-gaps, and multiply out the # of combos for each segment.
        if (data[i] == data[i - 1] + 3)
        {
            threes++;
            product *= count_paths_from_start_to_end(start, end);
            start = i;
        }

        end = i;
    }

    auto ones = data.size() - 1 - threes;
    return std::make_pair(ones * threes, product);
}

int main()
{
    initialize_data("input.txt");
    auto result = process_data();

    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << result.second << std::endl;

    assert(result.first == 2070);
    assert(result.second == 24179327893504);
    return 0;
}
