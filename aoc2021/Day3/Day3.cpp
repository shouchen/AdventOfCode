#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

auto read_data(const std::string &filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string s;

    while (file >> s)
        lines.push_back(s);

    return lines;
}

auto do_ocr_or_csr_reduction_step(const std::vector<std::string> &lines, int pos, bool ocr)
{
    std::vector<std::string> retval;

    auto ones = (unsigned)std::count_if(lines.cbegin(), lines.cend(), [pos](const std::string &s) { return s[pos] == '1'; });
    auto zeros = lines.size() - ones;

    for (auto &line : lines)
    {
        if ((line[pos] == '1' && (ocr ^ (ones < zeros))) ||
            (line[pos] == '0' && (ocr ^ (ones >= zeros))))
        {
            retval.push_back(line);
        }
    }
    
    return retval;
}

auto do_part1(const std::vector<std::string> &lines)
{
    std::string gamma, epsilon;

    for (auto i = 0U; i < lines[0].length(); i++)
    {
        auto ones = (unsigned)std::count_if(lines.cbegin(), lines.cend(), [i](const std::string &s) { return s[i] == '1'; });
        auto zeros = lines.size() - ones;

        gamma.push_back(ones > zeros ? '1' : '0');
        epsilon.push_back(ones < zeros ? '1' : '0');
    }

    return stoi(gamma, nullptr, 2) * stoi(epsilon, nullptr, 2);
}

auto do_part2(std::vector<std::string> lines, bool ocr)
{
    for (int i = 0; lines.size() > 1; i++)
        lines = do_ocr_or_csr_reduction_step(lines, i, ocr);

    return stoi(lines[0], nullptr, 2);
}

int main()
{
    auto lines = read_data("input.txt");

    auto part1 = do_part1(lines);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2(lines, true) * do_part2(lines, false);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 749376);
    assert(part2 == 2372923);
    return 0;
}
