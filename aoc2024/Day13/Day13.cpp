#include <iostream>
#include <fstream>
#include <cassert>

auto solve_machine(long long ax, long long ay, long long bx, long long by, long long x, long long y)
{
    auto determinant = ax * by - ay * bx;
    assert(determinant); // If zero, there are more possibilities to check - this wasn't in puzzle data.

    // We have a single real solution; use Cramer's rule.
    auto numerator1 = x * by - bx * y, numerator2 = ax * y - x * ay;
    if (numerator1 % determinant || numerator2 % determinant)
        return ULLONG_MAX; // No integral solution.

    auto a = numerator1 / determinant, b = numerator2 / determinant;
    return 3ULL * a + b;
}

auto solve(const std::string &filename, unsigned long long additional_xy = 0)
{
    std::ifstream file(filename);
    std::string line, button, a_or_b_colon, prize_colon;
    auto x_char = 'X', y_char = 'Y', plus = '+', comma = ',', equals = '=';
    auto ax = 0ULL, ay = 00ULL, bx = 00ULL, by = 00ULL, x = 00ULL, y = 00ULL, retval = 0ULL;

    while (file >> button >> a_or_b_colon >> x_char >> plus >> ax >> comma >> y_char >> plus >> ay)
    {
        file >> button >> a_or_b_colon >> x_char >> plus >> bx >> comma >> y_char >> plus >> by;
        file >> prize_colon >> x_char >> equals >> x >> comma >> y_char >> equals >> y;

        auto tokens = solve_machine(ax, ay, bx, by, x + additional_xy, y + additional_xy);
        if (tokens < ULLONG_MAX)
            retval += tokens;
    }

    return retval;
}

int main()
{
    auto part1 = solve("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 36250);

    auto part2 = solve("input.txt", 10000000000000);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 83232379451012);

    return 0;
}
