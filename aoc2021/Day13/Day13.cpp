#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cassert>

std::map<std::pair<int, int>, char> grid;
int max_x = INT_MIN, max_y = INT_MIN;

void fold_up(int axis)
{
    int from = axis + 1, to = axis - 1;
    while (to >= 0)
    {
        for (int x = 0; x <= max_x; x++)
            if (grid[std::make_pair(x, from)] == '#')
                grid[std::make_pair(x, to)] = '#';
        from++;
        to--;
    }

    for (int y = axis; y <= max_y; y++)
        for (int x = 0; x <= max_x; x++)
            grid.erase(std::make_pair(x, y));

    max_y = axis - 1;
}

void fold_left(int axis)
{
    int from = axis + 1, to = axis - 1;
    while (to >= 0)
    {
        for (int y = 0; y <= max_y; y++)
            if (grid[std::make_pair(from, y)] == '#')
                grid[std::make_pair(to, y)] = '#';
        from++;
        to--;
    }

    for (int x = axis; x <= max_x; x++)
        for (int y = 0; y <= max_y; y++)
            grid.erase(std::make_pair(x, y));

    max_x = axis - 1;
}

void dump()
{
    for (int y = 0; y <= max_y; y++)
    {
        for (int x = 0; x <= max_x; x++)
            std::cout << ((grid[std::make_pair(x, y)] == '#') ? '#' : '.');

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int count()
{
    int count = 0;
    for (int y = 0; y <= max_y; y++)
    {
        for (int x = 0; x <= max_x; x++)
        {
            if (grid[std::make_pair(x, y)] == '#')
                count++;
        }
    }
    return count;
}

auto process_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, fold, along;
    auto part1 = -1, x = 0, y = 0, amount = 0;
    auto comma = ',', axis = 'x', equals = '=';

    while (getline(file, line) && line.length())
    {
        std::istringstream ss(line);
        ss >> x >> comma >> y;

        grid[std::make_pair(x, y)] = '#';

        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;
    }

    while (getline(file, line))
    {
        std::istringstream ss(line);
        ss >> fold >> along >> axis >> equals >> amount;

        if (axis == 'y')
            fold_up(amount);
        else
            fold_left(amount);

        if (part1 == -1)
            part1 = count();
    }

    return part1;
}

int main()
{
    auto part1 = process_input("input.txt");

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << std::endl;
    dump();

    assert(part1 == 781);
    // Part two visually displays "PERCGJPB"
    return 0;
}
