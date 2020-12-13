#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<std::string> data;

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string s;

    while (file >> s)
        data.push_back(s);
}

auto follow_slope(int right, int down)
{
    auto row = 0, col = 0;
    auto count = 0ULL;

    while (row < data.size())
    {
        if (data[row][col] == '#')
            count++;

        col = (col + right) % data[row].length();
        row += down;
    }

    return count;
}

int main()
{
    read_data("input.txt");

    auto part1 = follow_slope(3, 1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = follow_slope(1, 1) * follow_slope(3, 1) * follow_slope(5, 1) * follow_slope(7, 1) * follow_slope(1, 2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 216);
    assert(part2 == 6708199680);
    return 0;
}
