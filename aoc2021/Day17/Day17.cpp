#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

/*
This solution is brute force, choosing reasonable search ranges. No need to
optimize 100%. Since the target area is always lower than the origin ("trench"),
there are three categories of input to consider:

.........................  .........................  .........................
S........................  ........................S  ............S............
.........................  .........................  .........................
.........................  .........................  .........................
..............TTTTTTTTTTT  TTTTTTTTTTT..............  .......TTTTTTTTTTT.......
..............TTTTTTTTTTT  TTTTTTTTTTT..............  .......TTTTTTTTTTT.......
..............TTTTTTTTTTT  TTTTTTTTTTT..............  .......TTTTTTTTTTT.......
..............TTTTTTTTTTT  TTTTTTTTTTT..............  .......TTTTTTTTTTT.......
 (i) target to the right    (ii) target to the left    (iii) target straddles

 Selecting the range for xv:
 * The x velocity never switches left/right, so initial xv must be in the
   direction of the target.
 * An initial step in the x direction that passes the whole target will never
   return so it needs to be less than that initially.
 * Both of the above are satisfied for categories (i), (ii), (iii) using this
   range for xv:  min(1, x_min_target)..max(-1,x_max_target).

Selecting the range for yv:
* The initial yv could be negative or zero (down-pointing), but if it goes below
  the target's in the first step, it's a non-starter, so we can use that for the
  min (negativemost) starting point.
* The initial yv could be positive (up-pointing). Since the projectile traces a
  parabolic arc, it will return to y = 0, at which point, yv will be the
  negative of the initial yv. So, when the projectile reaches y = 0, if yv is
  below the target minimum, it would skip the target in the next step. Any
  larger initial yv also would.
* So, regardless of category (i), (ii), (iii), we can just use this range for
  yv:  y_min_target..y_max_target.
*/

auto x_min_target = 0, x_max_target = 0, y_min_target = 0, y_max_target = 0;

// If target is hit, returns the maximum height reached. If not hit, returns -1.
auto test_velocities(int xv, int yv)
{
    auto x = 0, y = 0, max_height = 0;

    for (;;)
    {
        max_height = std::max(max_height, y);

        x += xv, y += yv;

        if (xv > 0)
            xv--;
        else if (xv < 0)
            xv++;

        yv--;

        if (x >= x_min_target && x <= x_max_target && y >= y_min_target && y <= y_max_target)
            return max_height;

        // if y falls below the minimum target, it will never recover--it's a miss.
        if (y < y_min_target)
            return -1;
    }
}

auto process_input(const std::string &filename)
{
    std::ifstream file("input.txt");
    std::string target, area_colon;
    auto x = 'x', y = 'y', equals = '=', dot = '.', comma = ',';

    file >> target >> area_colon >> x >> equals >> x_min_target >> dot >> dot >>
        x_max_target >> comma >> y >> equals >> y_min_target >> dot >> dot >> y_max_target;

    auto hits = 0, max_height = 0;
    auto xv_start = std::min(1, x_min_target), xv_end = std::max(-1, x_max_target);

    for (auto xv = xv_start; xv <= xv_end; xv++)
    {
        for (auto yv = y_min_target; yv <= -y_min_target; yv++)
        {
            auto height = test_velocities(xv, yv);
            if (height > -1)
            {
                max_height = std::max(max_height, height);
                hits++;
            }
        }
    }

    return std::make_pair(max_height, hits);
}

int main()
{
    auto result = process_input("input.txt");

    std::cout << "Part 1: " << result.first << std::endl;
    std::cout << "Part 2: " << result.second << std::endl;

    assert(result.first == 4851);
    assert(result.second == 1739);
    return 0;
}
