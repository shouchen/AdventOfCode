#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct Range { long long start, len; };
struct Mapping { Range range; long long dest; };

std::vector<std::vector<Mapping>> mapping;
std::string seeds_line;

auto recur(int level, const Range &input)
{
    if (level == mapping.size())
        return input.start;

    for (auto &test : mapping[level])
    {
        auto input_end = input.start + input.len - 1, test_end = test.range.start + test.range.len - 1;
        auto overlap_start = std::max(input.start, test.range.start);
        auto overlap_end = std::min(input_end, test_end);

        if (overlap_start <= overlap_end)
        {
            auto overlap = Range{ overlap_start, overlap_end - overlap_start + 1 };
            overlap.start += test.dest - test.range.start;

            auto min = recur(level + 1, overlap);

            if (input.start < overlap_start)
            {
                auto preoverlap = Range{ input.start, test.range.start - input.start };
                auto preoverlap_end = std::min(input_end, test_end);
                min = std::min(min, recur(level, preoverlap));
            }

            if (input_end > overlap_end)
            {
                auto postoverlap = Range{ test_end + 1, input_end - test_end };
                auto postoverlap_end = std::min(input_end, test_end);
                min = std::min(min, recur(level, postoverlap));
            }

            return min;
        }
    }

    return recur(level + 1, input);
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    std::string line;

    std::getline(file, seeds_line);
    std::getline(file, line);

    while (std::getline(file, line) && !line.empty())
    {
        mapping.push_back(std::vector<Mapping>());

        while (std::getline(file, line) && !line.empty())
        {
            std::istringstream is(line);
            Mapping m;

            is >> m.dest >> m.range.start >> m.range.len;
            mapping.back().push_back(m);
        }
    }

    auto n = 0LL, len = 1LL, min = LLONG_MAX;
    std::istringstream is(seeds_line);
    std::string seeds_colon;

    is >> seeds_colon;

    while (is >> n)
    {
        if (part2)
            is >> len;

        min = std::min(min, recur(0, Range{ n, len }));
    }

    return min;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 484023871);

    auto part2 = do_part("input.txt", true);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 46294175);
    return 0;
}
