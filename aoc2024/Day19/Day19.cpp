#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

std::vector<std::string> patterns;

auto is_possible(const std::string &design)
{
    if (design.empty())
        return true;

    for (auto &p : patterns)
        if (design.length() >= p.length() && std::equal(p.begin(), p.end(), design.begin()))
            if (is_possible(design.substr(p.length())))
                return true;

    return false;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, pattern, design;
    auto comma = ',';

    std::getline(file, line);

    std::istringstream iss(line);
    while (iss >> pattern)
    {
        if (pattern.back() == ',') pattern.pop_back();
        patterns.push_back(pattern);
    }

    auto retval = 0;
    while (file >> design)
    {
        if (is_possible(design))
            retval++;
    }

    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 213);

    //auto part2 = do_part2("input.txt");
    //std::cout << "Part Two: " << part2 << std::endl;
    ////assert(part2 == );

    return 0;
}
