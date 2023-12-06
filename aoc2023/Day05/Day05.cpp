#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Range { long long start, len; };
struct Mapping { Range range; long long dest; };

std::vector<Mapping> mapping[7];
std::string seeds_line;

long long recur(int level, Range input)
{
    auto min = LLONG_MAX;

    if (input.len == 0)
        return min;

    if (level == 4)
    {
        std::cout << "debug" << std::endl;
    }

    //  if no move levels, return the lowest beginning range of all
    if (level == 7)
    {
        std::cout << "terminus " << input.start << std::endl;
        return input.start;
    }

    for (auto &test : mapping[level])
    {
        auto input_end = input.start + input.len - 1, test_end = test.range.start + test.range.len - 1;

        auto overlap_start = std::max(input.start, test.range.start);
        auto overlap_end = std::min(input_end, test_end);
        if (overlap_start <= overlap_end)
        {
            auto overlap = Range{ overlap_start, overlap_end - overlap_start + 1 };
            overlap.start += test.dest - test.range.start;
            auto temp = recur(level + 1, overlap);
            min = std::min(min, temp);

            if (input.start < overlap_start)
            {
                auto preoverlap = Range{ input.start, test.range.start - input.start };
                auto preoverlap_end = std::min(input_end, test_end);
                auto temp = recur(level, preoverlap);
                min = std::min(min, temp);
            }

            if (input_end > overlap_end)
            {
                auto postoverlap = Range{ test_end + 1, input_end - test_end };
                auto postoverlap_end = std::min(input_end, test_end);
                auto temp = recur(level, postoverlap);
                min = std::min(min, temp);
            }

            return min;
        }
    }

    auto temp = recur(level + 1, input);
    return std::min(min, temp);
}

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    std::getline(file, seeds_line);
    std::getline(file, line); // blank line after seeds

    for (int i = 0; i < 7; i++)
    {
        std::getline(file, line); // discard map heading
        while (std::getline(file, line) && !line.empty())
        {
            std::istringstream is(line);
            Mapping m;

            is >> m.dest >> m.range.start >> m.range.len;
            mapping[i].push_back(m);
        }
    }
}

auto do_part(bool part2)
{
    auto n = 0LL, len = 1LL, min = LLONG_MAX;
    std::string seeds_colon;
    std::istringstream is(seeds_line);

    is >> seeds_colon;

    while (is >> n)
    {
        if (part2)
            is >> len;

        auto temp = recur(0, Range{ n, len });
        min = std::min(min, temp);
    }

    return min;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part(false);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 484023871);

    auto part2 = do_part(true);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 46294175);
    return 0;
}
