#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

struct Item
{
    long long value;
    int original_pos;
};

auto process_input(const std::string &filename, int multiplicand, int num_mixes)
{
    std::vector<Item> input;
    std::ifstream file(filename);
    auto n = 0LL;

    while (file >> n)
        input.push_back(Item{ n * multiplicand, int(input.size()) });

    auto input_size = long long(input.size());

    while (num_mixes--)
    {
        for (auto i = 0; i < input_size; i++)
        {
            auto start = -1LL;
            for (auto j = 0; j < input_size; j++)
                if (input[j].original_pos == i)
                {
                    start = j;
                    break;
                }

            auto moves = input[start].value % (input_size - 1);
            auto end = start + moves;

            if (end <= 0)
                end += input_size - 1;
            else if (end >= input_size)
                end -= input_size - 1;

            auto delta = (end > start) ? 1LL : -1LL;
            for (;start != end; start += delta)
                std::swap(input[start], input[start + delta]);
        }
    }

    auto zero_pos = 0;
    while (input[zero_pos].value != 0)
        zero_pos++;

    return
        input[(zero_pos + 1000LL) % input_size].value +
        input[(zero_pos + 2000LL) % input_size].value +
        input[(zero_pos + 3000LL) % input_size].value;
}

int main()
{
    auto part1 = process_input("input.txt", 1, 1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_input("input.txt", 811589153, 10);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 8372);
    assert(part2 == 7865110481723);
    return 0;
}
