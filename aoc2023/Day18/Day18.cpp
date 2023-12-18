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

std::map<std::pair<std::pair<int, int>, int>, int> grid;  // ((x,y),z) --> int

int min_x = INT_MAX, min_y = INT_MAX, min_z = INT_MAX;
int max_x = INT_MIN, max_y = INT_MIN, max_z = INT_MIN;

void set_cell(int x, int y, int z)
{
    min_x = min(min_x, x);
    min_y = min(min_y, y);
    min_z = min(min_z, z);
    max_x = max(max_x, x);
    max_y = max(max_y, y);
    max_z = max(max_z, z);
    grid[make_pair(make_pair(x, y), z)] = 1;
}

void set_cell_dot(int x, int y, int z)
{
    min_x = min(min_x, x);
    min_y = min(min_y, y);
    min_z = min(min_z, z);
    max_x = max(max_x, x);
    max_y = max(max_y, y);
    max_z = max(max_z, z);
    grid[make_pair(make_pair(x, y), z)] = 2;  // 1 == #, 2 == .
}

long long dump_grid()
{
    long long interior = 0;
    for (int i = min_y; i <= max_y; i++)
    {
        for (int j = min_x; j <= max_x; j++)
        {
            if (grid.find(make_pair(make_pair(j, i), -1)) != grid.end())
            {
                if (grid[make_pair(make_pair(j, i), -1)] == 1)
                {
                    //cout << '#';
                    interior++;
                }
                else
                {
                    //cout << '.';
                }
            }
            else
            {
                //cout << '.';
                interior++;
            }
        }
        //std::cout << endl;
    }
    //std::cout << endl;
    return interior;
}

void recur(int x, int y, int z)
{
    if (x < min_x || x > max_x || y < min_y || y > max_y)
        return;
        
    if (grid.find(make_pair(make_pair(x, y), -1)) != grid.end())
        return;

    set_cell_dot(x, y, z);
    recur(x - 1, y, z);
    recur(x + 1, y, z);
    recur(x, y - 1, z);
    recur(x, y + 1, z);
}

void fill_outside()
{
    int z = -1;
    for (int x = min_x; x <= max_x; x++)
    {
        recur(x, min_y, z);
        recur(x, max_y, z);
    }

    for (int y = min_y; y <= max_y; y++)
    {
        recur(min_x, y, z);
        recur(max_x, y, z);
    }
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    auto dir = ' ';
    int len;
    string colorstr;

    int x = 0, y = 0, z = -1;
    int xd = 0, yd = -1, zd = 0;

    grid[make_pair(make_pair(x, y), z)] = 1;


    while (file >> dir >> len >> colorstr)
    {
        if (dir == 'R')
        {
            xd = 1, yd = 0;
        }
        else if (dir == 'D')
        {
            xd = 0, yd = 1;
        }
        else if (dir == 'L')
        {
            xd = -1, yd = 0;
        }
        else if (dir == 'U')
        {
            xd = 0; yd = -1;
        }

        while (len--)
        {
            x += xd, y += yd;
            set_cell(x, y, z);
        }

//        dump_grid();
    }

    fill_outside();
    return dump_grid();
}

vector<pair<long long, long long>> vertices;

long long polygonArea()
{
    long long n = vertices.size();

    long long area = 0;

    // Calculate value of shoelace formula
    int j = n - 1;
    for (int i = 0; i < n; i++)
    {
        area += (vertices[j].first + vertices[i].first) * (vertices[j].second - vertices[i].second);
        j = i;  // j is previous vertex to i
    }

    return abs(area / 2);
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    auto dir = ' ';
    int len;
    string colorstr;

    long long x = 0, y = 0;
    long long boundary_count = 0;

    while (file >> dir >> len >> colorstr)
    {
        len = strtol(colorstr.substr(2, 5).c_str(), NULL, 16);
        if (colorstr[7] == '0')
            dir = 'R';
        else if (colorstr[7] == '1')
            dir = 'D';
        else if (colorstr[7] == '2')
            dir = 'L';
        else if (colorstr[7] == '3')
            dir = 'U';

        boundary_count += len;

        if (dir == 'R')
        {
            x += len;
        }
        else if (dir == 'D')
        {
            y += len;
        }
        else if (dir == 'L')
        {
            x -= len;
        }
        else if (dir == 'U')
        {
            y -= len;
        }

        vertices.push_back({ x, y });
    }

    // 1. count boundary points (above)
    // 2. apply shoelace formula to vertices
    auto area = polygonArea();

    // 3. pick's theorem --> number of interior point
    // S = I + B / 2 - 1
    // where S = Polygon area, I= #points inside, B = boundary points
    
    auto interior = area - boundary_count / 2 + 1;
    
    // 4. add # boundary points
    auto answer = interior + boundary_count;

    return answer;
}

#define INPUT_FILE "test.txt"
#define INPUT_FILE "input.txt"

int main()
{
    auto part1 = do_part1(INPUT_FILE);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2(INPUT_FILE);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 48652);
    assert(part2 == 45757884535661);
    return 0;
}
