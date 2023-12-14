#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <ctype.h>
#include <algorithm>
#include <numeric>
#include <regex>
#include <cassert>

using namespace std;

//void read_input(const std::string &filename)
//{
//    std::ifstream file(filename);
//    std::string line;
//
//    for (auto row = 0; std::getline(file, line); row++)
//    {
//        std::regex number("\\d+"), symbol("([^\\d\\.])");
//
//        for (auto i = std::sregex_iterator(line.begin(), line.end(), number); i != std::sregex_iterator(); i++)
//            numbers.push_back(
//                Number{ std::stoi(i->str()), row, int(i->position()), int(i->position() + i->length() - 1) });
//
//        for (auto i = std::sregex_iterator(line.begin(), line.end(), symbol); i != std::sregex_iterator(); i++)
//            symbols.push_back(
//                Symbol{ i->str()[0], row, int(i->position()) });
//    }
//}

#pragma region read-by-lines
void read_input_lines(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, word;

    while (std::getline(file, line))
    {
        std::cout << line << std::endl;

        std::istringstream is(line);
        while (is >> word)
            std::cout << word << std::endl;
    }
}
#pragma endregion

#pragma region read-by-chars
void read_input_chars(const std::string &filename)
{
    std::ifstream file(filename);
    auto c = ' ';

    while (file >> c)
    {
        std::cout << c << std::endl;
    }
}
#pragma endregion

#pragma region regex
#include <regex>
void parse_games()
{
    std::ifstream file("input.txt");
    std::string line;

    while (std::getline(file, line))
    {
        std::regex e(R"(Game (\d):(?: (\d)* (red|blue|green)(,|;)){2}.*)");

        std::smatch sm;
        auto result = std::regex_match(line, sm, e);
        std::cout << std::endl;
    }
}
#pragma endregion

#pragma region grid
std::vector<std::string> grid;
void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}
void dump_grid()
{
    for (auto &row : grid)
        std::cout << row << std::endl;
    cout << endl;
}
#pragma endregion

#pragma region sparse-grids
std::map<std::pair<int, int>, char> grid2;
std::map<std::pair<std::pair<int, int>, int>, char> grid3;
#pragma endregion

#pragma region math
auto gcd(long long a, long long b)
{
    if (b == 0) return a;
    return gcd(b, a % b);
}

auto lcm(std::vector<int> v)
{
    long long ans = v[0];

    for (auto i = 1; i < v.size(); i++)
        ans = v[i] * ans / gcd(v[i], ans);

    return ans;
}
#pragma endregion

auto do_part1()
{
    // tilt N
    for (int col = 0; col < grid[0].size(); col++)
    {
        for (;;)
        {
            bool moved = false;
            for (int row = 0; row < grid.size() - 1; row++)
            {
                if (grid[row][col] == '.' && grid[row + 1][col] == 'O')
                {
                    grid[row][col] = 'O', grid[row + 1][col] = '.';
                    moved = true;
                }
            }

            if (!moved)
                break;
        }
    }

    dump_grid();

    long long total = 0;

    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == 'O')
            {
                total += grid.size() - i;
            }
        }

    return total;
}


long long next_avail = 0;
map<vector<string>, long long> seen;

int periodStart = -1;


auto do_part2()
{
    for (long long cycle = 1; cycle < 200
        ; cycle++)
    {
        // tilt N
        for (int col = 0; col < grid[0].size(); col++)
        {
            for (;;)
            {
                bool moved = false;
                for (int row = 0; row < grid.size() - 1; row++)
                {
                    if (grid[row][col] == '.' && grid[row + 1][col] == 'O')
                    {
                        grid[row][col] = 'O', grid[row + 1][col] = '.';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }


        // tilt W
        for (int row = 0; row < grid.size(); row++)
        {
            for (;;)
            {
                bool moved = false;
                for (int col = 0; col < grid[0].size() - 1; col++)
                {
                    if (grid[row][col] == '.' && grid[row][col + 1] == 'O')
                    {
                        grid[row][col] = 'O', grid[row][col + 1] = '.';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }

        // tilt S
        for (int col = 0; col < grid[0].size(); col++)
        {
            for (;;)
            {
                bool moved = false;
                for (int row = 0; row < grid.size() - 1; row++)
                {
                    if (grid[row][col] == 'O' && grid[row + 1][col] == '.')
                    {
                        grid[row][col] = '.', grid[row + 1][col] = 'O';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }

        // tilt E
        for (int row = 0; row < grid.size(); row++)
        {
            for (;;)
            {
                bool moved = false;
                for (int col = 0; col < grid[0].size() - 1; col++)
                {
                    if (grid[row][col] == 'O' && grid[row][col + 1] == '.')
                    {
                        grid[row][col] = '.', grid[row][col + 1] = 'O';
                        moved = true;
                    }
                }

                if (!moved)
                    break;
            }
        }

//        if (seen[grid] == 0)
//        {
//            seen[grid] = next_avail++;
//        }
//        else
//        {
////            cout << "CYCLE " << cycle << " was first at " << seen[grid] << endl;
//
//            //cout << seen[grid] << " " << ((cycle - 9) % 7 + 2) << endl;;
//            
//            // CYCLE n was at (n - 9) % 7 + 2;
//
//    
//        }

        long long total = 0;

        for (int i = 0; i < grid.size(); i++)
            for (int j = 0; j < grid[0].size(); j++)
            {
                if (grid[i][j] == 'O')
                {
                    total += grid.size() - i;
                }
            }

        cout << cycle << " = " << total << endl;

                if (seen.find(grid) == seen.end())
                {
                    periodStart = seen[grid];

                    seen[grid] = cycle;
                }
                else
                {
                    cout << endl;
                    cout << "CYCLE " << cycle << " was first seen at " << seen[grid] << endl;
        
                    //cout << seen[grid] << " " << ((cycle - 9) % 7 + 2) << endl;;
                    
                    // CYCLE n was at (n - 9) % 7 + 2;

                    // EXAMPLE period = 7  (9 -2)
                    // (n - 2) % 7 + 2
                    // EXAMPLE 1000000000 --> 5
                    // 5 --> 64, 12 -> 64
        
            
                }
    }

    //auto cycle_len = 7;
    //ans = cycl

    //auto x = (1000000000 - 105) % 39 + 105;
    //cout << x << endl;





#if 0
        CYCLE 9 was at 2
        CYCLE 10 was at 3
        CYCLE 11 was at 4
        CYCLE 12 was at 5
        CYCLE 13 was at 6
        CYCLE 14 was at 7
        CYCLE 15 was at 8
        CYCLE 16 was at 2
        CYCLE 17 was at 3
        CYCLE 18 was at 4
        CYCLE 19 was at 5
        CYCLE 20 was at 6
        CYCLE 21 was at 7
#endif


    dump_grid();

    long long total = 0;

    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == 'O')
            {
                total += grid.size() - i;
            }
        }

    return total;
}

#define INPUT_FILE "test.txt"
#define INPUT_FILE "input.txt"

int main()
{
    read_grid(INPUT_FILE);

    //auto part1 = do_part1();
    //std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    //assert(part1 == );
    //assert(part2 == 100531);
    return 0;
}


//auto x = (1000000000 - 105) % 39 + 105 ----> 142   (3100531)
//cout << x << endl;
