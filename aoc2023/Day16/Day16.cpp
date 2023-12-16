#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>

std::vector<std::string> grid;
void read_grid(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
        grid.push_back(line);
}

std::map<std::pair<int,int>, std::set<std::pair<int,int>>> visited;
std::string dirns = "^v><";

void dump_with_progress()
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (visited.find(std::make_pair(i, j)) == visited.end())
            {
                std::cout << grid[i][j];
            }
            else
            {
                if (grid[i][j] == '.')
                {
                    char c = grid[i][j];
                    auto temp = visited[std::make_pair(i, j)];
                    if (temp.find(std::make_pair(-1, 0)) != temp.end()) // N
                    {
                        if (dirns.find(c) == std::string::npos)
                            c = '^';
                        else if (isdigit(c))
                            c++;
                        else
                            c = '2';
                    }
                    if (temp.find(std::make_pair(1, 0)) != temp.end()) // S
                    {
                        if (dirns.find(c) == std::string::npos)
                            c = 'v';
                        else if (isdigit(c))
                            c++;
                        else
                            c = '2';
                    }
                    if (temp.find(std::make_pair(0, 1)) != temp.end()) // E
                    {
                        if (dirns.find(c) == std::string::npos)
                            c = '>';
                        else if (isdigit(c))
                            c++;
                        else
                            c = '2';
                    }
                    if (temp.find(std::make_pair(0, -1)) != temp.end()) // W
                    {
                        if (dirns.find(c) == std::string::npos)
                            c = '<';
                        else if (isdigit(c))
                            c++;
                        else
                            c = '2';
                    }
                    std::cout << c;
                }
                else
                {
                    std::cout << grid[i][j];
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void do_ray(int row, int col, int rdir, int cdir)
{
    if (row < 0 || row >= grid.size() || col < 0 || col >= grid[row].size())
        return;

    auto rc = std::make_pair(row, col), rcdir = std::make_pair(rdir, cdir);
    auto temp = visited.find(rc);
    if (temp != visited.end() && temp->second.find(rcdir) != temp->second.end())
        return;

    visited[rc].insert(rcdir);

    //cout << "  (" << row << ',' << col << ") " << rdir << "," << cdir << endl;
    //dump_with_progress();


    if (grid[row][col] == '.')
    {
        do_ray(row + rdir, col + cdir, rdir, cdir);
    }
    else if (grid[row][col] == '/')
    {
        if (rdir == 0 && cdir == 1) // E
            do_ray(row - 1, col, -1, 0);
        else  if (rdir == 0 && cdir == -1) // W
            do_ray(row + 1, col, 1, 0);
        else if (rdir == -1 && cdir == 0) // N
            do_ray(row, col + 1, 0, 1);
        else  if (rdir == 1 && cdir == 0) // S
            do_ray(row, col - 1, 0, -1);
    }
    else if (grid[row][col] == '\\')
    {
        if (rdir == 0 && cdir == 1) // E
            do_ray(row + 1, col, 1, 0);
        else  if (rdir == 0 && cdir == -1) // W
            do_ray(row - 1, col, -1, 0);
        else if (rdir == -1 && cdir == 0) // N
            do_ray(row, col - 1, 0, -1);
        else if (rdir == 1 && cdir == 0) // S
            do_ray(row, col + 1, 0, 1);
    }
    else if (grid[row][col] == '-')
    {
        if (rdir == 0 && cdir == 1) // E
            do_ray(row + rdir, col + cdir, rdir, cdir);
        else  if (rdir == 0 && cdir == -1) // W
            do_ray(row + rdir, col + cdir, rdir, cdir);
        else if (rdir == -1 && cdir == 0) // N
        {
            do_ray(row, col - 1, 0, -1);
            do_ray(row, col + 1, 0, 1);
        }
        else  if (rdir == 1 && cdir == 0) // S
        {
            do_ray(row, col - 1, 0, -1);
            do_ray(row, col + 1, 0, 1);
        }
    }
    else if (grid[row][col] == '|')
    {
        if (rdir == 0 && cdir == 1) // E
        {
            do_ray(row - 1, col, -1, 0);
            do_ray(row + 1, col, 1, 0);
        }
        else  if (rdir == 0 && cdir == -1) // W
        {
            do_ray(row - 1, col, -1, 0);
            do_ray(row + 1, col, 1, 0);
        }
        else if (rdir == -1 && cdir == 0) // N
            do_ray(row + rdir, col + cdir, rdir, cdir);
        else  if (rdir == 1 && cdir == 0) // S
            do_ray(row + rdir, col + cdir, rdir, cdir);
    }
}

auto do_part1(int row, int col, int rdir, int cdir)
{
    visited.clear();
    do_ray(row, col, rdir, cdir);
    return int(visited.size());
}

auto do_part2()
{
    auto max_energy = 0;

    // N
    for (int col = 0; col < grid[0].size(); col++)
        max_energy = std::max(max_energy, do_part1(int(grid.size()) - 1, col, -1, 0));

    // S
    for (int col = 0; col < grid[0].size(); col++)
        max_energy = std::max(max_energy, do_part1(0, col, -1, 0));

    // E
    for (int row = 0; row < grid.size(); row++)
        max_energy = std::max(max_energy, do_part1(row, 0, 0, 1));

    // W
    for (int row = 0; row < grid.size(); row++)
        max_energy = std::max(max_energy, do_part1(row, int(grid[0].size()) - 1, 0, -1));

    return max_energy;
}

int main()
{
    read_grid("input.txt");

    auto part1 = do_part1(0, 0, 0, 1);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2();
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 7434);
    assert(part2 == 8183);
    return 0;
}
