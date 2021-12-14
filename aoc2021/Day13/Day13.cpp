#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <string>
#include <algorithm>
#include <cassert>

using Dots = std::set<std::pair<int, int>>;

auto process_input(const std::string &filename)
{
    Dots dots;
    std::ifstream file(filename);
    std::string line, fold, along;
    auto part1 = -1, x = 0, y = 0, crease = 0;
    auto comma = ',', axis = 'x', equals = '=';

    while (getline(file, line) && line.length())
    {
        std::istringstream ss(line);
        ss >> x >> comma >> y;

        dots.insert(std::make_pair(x, y));
    }

    while (getline(file, line))
    {
        std::istringstream ss(line);
        ss >> fold >> along >> axis >> equals >> crease;

        Dots next;
        for (auto dot : dots)
        {
            if (axis == 'x' && dot.first > crease)
                dot.first = crease * 2 - dot.first;
            else if (axis == 'y' && dot.second > crease)
                dot.second = crease * 2 - dot.second;

            next.insert(dot);
        }
        dots = next;

        if (part1 == -1)
            part1 = int(dots.size());
    }

    return std::make_pair(part1, dots);
}

std::ostream &operator<<(std::ostream &o, const Dots &dots)
{
    auto max_x = INT_MIN, max_y = INT_MIN;
    for (auto &dot : dots)
        max_x = std::max(dot.first, max_x), max_y = std::max(dot.second, max_y);

    for (auto y = 0; y <= max_y; y++)
    {
        for (auto x = 0; x <= max_x; x++)
            o << (dots.find(std::make_pair(x, y)) == dots.end() ? '.' : '#');

        o << std::endl;
    }
    return o;
}

int main()
{
    auto result = process_input("input.txt");

    std::cout << "Part One: " << result.first << std::endl;
    std::cout << "Part Two: " << std::endl << result.second;

    assert(result.first == 781);
    // Part two visually displays "PERCGJPB"
    return 0;
}
