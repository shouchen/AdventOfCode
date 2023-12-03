#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

struct Number { int num, row, start, end; };
struct Symbol { char type; int row, col; };

std::vector<Number> numbers;
std::vector<Symbol> symbols;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    for (auto row = 0; std::getline(file, line); row++)
    {
        for (auto i = 0; i < line.length(); i++)
        {
            if (isdigit(line[i]))
            {
                Number n{ 0, row, i, 0 };

                while (isdigit(line[i]))
                    n.num = 10 * n.num + line[i++] - '0';

                n.end = --i;
                numbers.push_back(n);
            }
            else if (line[i] != '.')
            {
                symbols.push_back(Symbol{ line[i], row, i });
            }
        }
    }
}

bool are_adjacent(Number &n, Symbol &s)
{
    return
        (n.row == s.row - 1 && s.col >= n.start - 1 && s.col <= n.end + 1) || // above
        (n.row == s.row + 1 && s.col >= n.start - 1 && s.col <= n.end + 1) || // below
        (n.row == s.row && n.end == s.col - 1) || // left
        (n.row == s.row && n.start == s.col + 1); // right
}

auto do_part1()
{
    auto total = 0;

    for (auto &n : numbers)
        for (auto &s : symbols)
            if (are_adjacent(n, s))
            {
                total += n.num;
                break;
            }

    return total;
}

auto do_part2()
{
    auto total = 0;

    for (auto &s : symbols)
    {
        if (s.type == '*')
        {
            std::vector<Number> adj;
            for (auto &n : numbers)
                if (are_adjacent(n, s))
                    adj.push_back(n);

            if (adj.size() == 2)
                total += adj[0].num * adj[1].num;
        }
    }

    return total;
}

int main()
{
    read_input("input.txt");

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1== 553825);
    assert(part2 == 93994191);
    return 0;
}
