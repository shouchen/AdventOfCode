#include <iostream>
#include <fstream>
#include <queue>
#include <cassert>

auto solve_machine(int ax_diff, int ay_diff, int bx_diff, int by_diff, int x_goal, int y_goal)
{
    auto min_cost = ULLONG_MAX;

    auto x = 0, y = 0, cost = 0;
    for (auto a = 1; a <= 100; a++)
    {
        x += ax_diff, y += ay_diff, cost += 3;

        if (x > x_goal)
            break;

        auto temp_x = x_goal - x, temp_y = y_goal - y;
        if ((temp_x % bx_diff != 0) || (temp_y % by_diff != 0))
            continue;

        auto b = temp_x / bx_diff;
        if (x + b * bx_diff == x_goal && y + b * by_diff == y_goal)
        {
            unsigned long long total_cost = cost + b;
            min_cost = std::min(total_cost, min_cost);
        }
    }

    return min_cost;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::string button, a_or_b_colon, prize_colon;
    auto x = 'X', y = 'Y', plus = '+', comma = ',', equals = '=';
    auto ax_diff = 0, ay_diff = 0, bx_diff = 0, by_diff = 0, x_goal = 0, y_goal = 0;
    auto retval = 0;

    while (file >> button >> a_or_b_colon >> x >> plus >> ax_diff >> comma >> y >> plus >> ay_diff)
    {
        file >> button >> a_or_b_colon >> x >> plus >> bx_diff >> comma >> y >> plus >> by_diff;
        file >> prize_colon >> x >> equals >> x_goal >> comma >> y >> equals >> y_goal;

        auto cost = solve_machine(ax_diff, ay_diff, bx_diff, by_diff, x_goal, y_goal);
        if (cost < ULLONG_MAX)
            retval += solve_machine(ax_diff, ay_diff, bx_diff, by_diff, x_goal, y_goal);
    }

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        ;

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    //assert(part1 == 36250);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
