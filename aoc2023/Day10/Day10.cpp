#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

#define INPUT_FILE "test.txt"
#define INPUT_FILE "input.txt"

using namespace std;

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

enum Dir
{
    North = 0x1,
    South = 0x2,
    East = 0x4,
    West = 0x8
};

int pipe_to_dirn(char p)
{
    switch (p)
    {
    case '|': return North | South;
    case '-': return East | West;
    case 'L': return North | East;
    case 'J': return North | West;
    case '7': return South | West;
    case 'F': return South | East;
    case '.': return 0;
    case 'O': return 0;
    case 'I': return 0;
    default: assert(false); return 0;
    }
}

void infer_s_pipe(int &row, int &col)
{
    int s_dirns = 0;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
            if (grid[i][j] == 'S')
            {
                if (i > 0)
                {
                    char above = grid[i - 1][j];
                    if (pipe_to_dirn(above) & South) s_dirns |= North;
                }
                if (i < grid.size() - 1)
                {
                    char below = grid[i + 1][j];
                    if (pipe_to_dirn(below) & North) s_dirns |= South;
                }
                if (j > 0)
                {
                    char left = grid[i][j - 1];
                    if (pipe_to_dirn(left) & East) s_dirns |= West;
                }
                if (j < grid[i].size() - 1)
                {
                    char right = grid[i][j + 1];
                    if (pipe_to_dirn(right) & West) s_dirns |= East;
                }

                if (s_dirns == (North | South))
                {
                    grid[i][j] = '|';
                }
                else if (s_dirns == (East | West))
                {
                    grid[i][j] = '-';
                }
                else if (s_dirns == (North | East))
                {
                    grid[i][j] = 'L';
                }
                else if (s_dirns == (North | West))
                {
                    grid[i][j] = 'J';
                }
                else if (s_dirns == (South | West))
                {
                    grid[i][j] = '7';
                }
                else if (s_dirns == (South | East))
                {
                    grid[i][j] = 'F';
                }
                row = i, col = j;
                return;
            }
}

vector<vector<int>> dist;

int dump_dists()
{
    int max = 0;
    for (int i = 0; i < dist.size(); i++)
    {
        for (int j = 0; j < dist[i].size(); j++)
        {
//            cout << dist[i][j] << " ";
            if (dist[i][j] != INT_MAX && dist[i][j] > max) max = dist[i][j];
        }
  //      cout << endl;
    }
    //cout << endl;
    return max;
}

auto fill_next_dist(int curr)
{
    bool updated = false;
    for (int row = 0; row < dist.size(); row++)
        for (int col = 0; col < dist[row].size(); col++)
        {
            if (dist[row][col] != curr) continue;

            auto dirns = pipe_to_dirn(grid[row][col]);

            if (dirns & North)
            {
                if (dist[row - 1][col] > curr + 1)
                {
                    dist[row - 1][col] = curr + 1;
                    updated = true;
                }
            }
            if (dirns & South)
            {
                if (dist[row + 1][col] > curr + 1)
                {
                    dist[row + 1][col] = curr + 1;
                    updated = true;
                }
            }
            if (dirns & East)
            {
                if (dist[row][col + 1] > curr + 1)
                {
                    dist[row][col + 1] = curr + 1;
                    updated = true;
                }
            }
            if (dirns & West)
            {
                if (dist[row][col - 1] > curr + 1)
                {
                    dist[row][col - 1] = curr + 1;
                    updated = true;
                }
            }
        }
    
    return updated;
}

void recur(int row, int col, int dist_so_far)
{
    if (dist[row][col] > dist_so_far)
    {
        dist[row][col] = dist_so_far;
        //dump_dists();


        auto dirns = pipe_to_dirn(grid[row][col]);

        if (dirns & North) recur(row - 1, col, dist_so_far + 1);
        if (dirns & South) recur(row + 1, col, dist_so_far + 1);
        if (dirns & East) recur(row, col + 1, dist_so_far + 1);
        if (dirns & West) recur(row, col - 1, dist_so_far + 1);
    }
}

int s_row, s_col;

auto do_part1()
{
    int row = 0, col = 0;
    infer_s_pipe(row, col);
    s_row = row, s_col = col;

    for (int i = 0; i < grid.size(); i++)
    {
        dist.push_back(vector<int>());
        for (int j = 0; j < grid[i].size(); j++)
            dist.back().push_back(INT_MAX);
    }

    dist[row][col] = 0;
    int i = 0;
    for (i = 0; ; i++)
    {
        if (!fill_next_dist(i))
            break;
    }

    return i;
}

pair<int, int> find_border_tile()
{
    for (int row = 0; row < grid.size(); row++)
    {
        if (grid[row][0] == '.')
        {
            return make_pair(row, 0);
        }
        if (grid[row][grid[row].size() - 1] == '.')
        {
            return make_pair(row, grid[row].size() - 1);
        }
    }

    for (int col = 0; col < grid[0].size(); col++)
    {
        if (grid[0][col] == '.')
        {
            return make_pair(0, col);
        }
        if (grid[grid.size() - 1][col] == '.')
        {
            return make_pair(grid.size() - 1, col);
        }
    }

    return make_pair(-1, -1);
}

void recur(int row, int col)
{
    if (row < 0 || row >= grid.size() || col < 0 || col >= grid[row].size())
        return;

    if (grid[row][col] != '.')
        return;

    grid[row][col] = 'O';

    recur(row - 1, col);
    recur(row + 1, col);
    recur(row, col - 1);
    recur(row, col + 1);

    if (row == grid.size() - 1 && col == 4)
        cout << endl;

    // cracks
    // up
    auto rr = row, cc = col;
    bool found = false;
    for (rr = row - 1; rr >= 0; rr--)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & East))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);

    rr = row, cc = col;
    found = false;
    for (rr = row - 1; rr >= 0; rr--)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & West))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);

    // down
    rr = row, cc = col;
    found = false;
    for (rr = row + 1; rr < grid.size(); rr++)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & East))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);

    rr = row, cc = col;
     found = false;
    for (rr = row + 1; rr < grid.size(); rr++)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & West))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);

    // right
    rr = row, cc = col;
     found = false;
    for (cc = col + 1; cc < grid[row].size(); cc++)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & North))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);

    rr = row, cc = col;
     found = false;
    for (cc = col + 1; cc < grid[row].size(); cc++)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & South))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);

    // left
    rr = row, cc = col;
    found = false;
    for (cc = col - 1; cc >= 0; cc--)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & North))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);

    rr = row, cc = col;
    found = false;
    for (cc = col - 1; cc >= 0; cc--)
    {
        auto dirns = pipe_to_dirn(grid[rr][cc]);
        if (grid[rr][cc] == '.')
        {
            found = true;
            break;
        }
        if (!(dirns & South))
        {
        }
        else
            break;
    }
    if (found)
        recur(rr, cc);
}

int count_inside_tiles() // also marks them with 'I' for debugging
{
    int total = 0;
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '.')
            {
                grid[i][j] = 'I';
                total++;
            }
        }
    return total;
}

void convert_tiles_not_in_loop_to_empty()
{
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (pipe_to_dirn(grid[i][j]) && dist[i][j] == INT_MAX)
            {
                grid[i][j] = '.';
            }
        }
}

auto do_part2()
{
    dump_grid();
    auto row = 0, col = 0;

    // pipe tiles not part of main loop should revert to '.'
    convert_tiles_not_in_loop_to_empty();
    dump_grid();

    for (;;)
    {
        auto a = find_border_tile();
        if (a.first == -1 && a.second == -1)
            break;

        row = a.first, col = a.second;
        recur(row, col);

        dump_grid();
    }

    // find outside cracks from above (not border)
    for (int col = 0; col < grid[0].size(); col++)
    {
        // down
        auto rr = row, cc = col;
        auto found = false;
        for (rr = 0; rr < grid.size(); rr++)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & East))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);

        rr = row, cc = col;
        found = false;
        for (rr = 0; rr < grid.size(); rr++)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & West))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);

        // up
        rr = row, cc = col;
        found = false;
        for (rr = grid.size() - 1; rr >=0 ; --rr)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & East))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);

        rr = row, cc = col;
        found = false;
        for (rr = grid.size() - 1; rr >= 0; --rr)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & West))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);
    }

    for (int row = 0; row < grid.size(); row++)
    {
        // right
        auto rr = row, cc = col;
        auto found = false;
        for (cc = 0; cc < grid.size(); cc++)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & North))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);

        rr = row, cc = col;
        found = false;
        for (cc = 0; cc < grid.size(); cc++)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & South))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);

        // left
        rr = row, cc = col;
        found = false;
        for (cc = grid[row].size() - 1; cc >= 0; --cc)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & North))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);

        rr = row, cc = col;
        found = false;
        for (cc = grid[row].size() - 1; cc >= 0; --cc)
        {
            auto dirns = pipe_to_dirn(grid[rr][cc]);
            if (grid[rr][cc] == '.')
            {
                found = true;
                break;
            }
            if (!(dirns & South))
            {
            }
            else
                break;
        }
        if (found)
            recur(rr, cc);
    }

    auto total = count_inside_tiles();

    dump_grid();

    return total;
}

int count_them(int row, int col)
{
    int total = 0;
    for (int i = 0; i < col; i++)
    {
        auto curr = grid[row][i];
        if (curr == '|' || curr == 'J' || curr == 'L')
            total++;
    }
    return total;
}


int another_part2()
{
    int total = 0;

    convert_tiles_not_in_loop_to_empty(); // optimize later to not do this

    for (auto row = 0; row < grid.size(); row++)
    {
        bool inside = false;

        for (auto col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col] == '|' || grid[row][col] == 'J' || grid[row][col] == 'L')
            {
                inside = !inside;
            }
            else if (grid[row][col] == '.' && inside)
            {
                grid[row][col] = 'I';
                total++;
            }
        }
    }

    dump_grid();

    return total;
}

int main()
{
    read_grid(INPUT_FILE);

    auto part1 = do_part1();
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = another_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7030);
    assert(part2 == 285);
    return 0;
}
