#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

struct Number { int num, row, start, end; };
struct Symbol { char type; int row, col; };

std::vector<std::string> grid;
std::vector<Number> numbers;
std::vector<Symbol> symbols;

void read_input(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

void transform__input() // from grid, makes lists of numbers and symbols
{
    for (auto i = 0; i < grid.size(); i++)
    {
        auto &row = grid[i];
        for (auto j = 0; j < row.size(); j++)
        {
            if (row[j] == '.')
                continue;

            if (!isdigit(row[j]))
            {
                symbols.push_back(Symbol{ row[j], i, j });
                continue;
            }

            auto start_row = i;
            auto start_col = j;

            int n = 0;
            while (j < row.size() && isdigit(row[j]))
            {
                n = 10 * n + row[j] - '0';
                j++;
            }

            auto end_col = j - 1;

            auto number = Number{ n, start_row, start_col, end_col };
            numbers.push_back(number);

            if (j < row.size() && !isdigit(row[j]) && row[j] != '.')
            {
                symbols.push_back(Symbol{ row[j], i, j });
                continue;
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
    transform__input();

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1== 553825);
    assert(part2 == 93994191);
    return 0;
}
