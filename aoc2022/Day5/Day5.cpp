#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <queue>

typedef void (MoveFunc)(std::vector<std::deque<char>> &data, int quantity, int from, int to);

void move_part1(std::vector<std::deque<char>> &data, int quantity, int from, int to)
{
    for (auto i = 0; i < quantity; i++)
    {
        auto temp = data[from].front();
        data[from].pop_front();
        data[to].push_front(temp);
    }
}

void move_part2(std::vector<std::deque<char>> &data, int quantity, int from, int to)
{
    std::string qqq;

    for (auto i = 0; i < quantity; i++)
    {
        auto temp = data[from].front();
        data[from].pop_front();
        qqq.push_back(temp);
    }

    for (auto j = qqq.rbegin(); j != qqq.rend(); j++)
        data[to].push_front(*j);
}

auto process_data(const std::string &filename, MoveFunc move_func)
{
    std::vector<std::deque<char>> data;
    for (auto i = 0; i < 1000; i++)
        data.push_back(std::deque<char>());

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line[1] != '1')
        for (auto i = 0; i < line.length(); i += 4)
            if (line[i] == '[')
            {
                char l = line[i + 1];
                data[i / 4 + 1].push_back(l);
            }

    std::string move, from, to;
    int quantity = 0, from_stack = 0, to_stack = 0;
    while (file >> move >> quantity >> from >> from_stack >> to >> to_stack)
        move_func(data, quantity, from_stack, to_stack);

    std::string retval;
    for (auto i = 1; i < 100; i++)
        if (!data[i].empty())
            retval += data[i].front();

    return retval;
}

int main()
{
    auto part1 = process_data("input.txt", move_part1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_data("input.txt", move_part2);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == "HBTMTBSDC");
    assert(part2 == "PQTJRSHWS");
    return 0;
}
