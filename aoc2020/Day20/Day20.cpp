#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

std::vector<struct Tile *> tiles; // the list of tiles
std::vector<std::pair<int, int>> placement; // where each tile is (or -1, -1)
int grid[12][12]; // stores which tile is in each spot (or 0)

struct Tile
{
    int id = 0;
    std::vector<std::string> matrix;

    void flip()
    {
        for (auto i = 0; i < matrix.size(); i++)
            for (auto l = 0ULL, r = matrix[i].size() - 1; l < r; l++, --r)
                std::swap(matrix[i][l], matrix[i][r]);
    }
    void rotate()
    {
        std::vector<std::string> temp = matrix;
        for (auto i = 0; i < matrix.size(); i++)
            for (auto j = 0; j < matrix[i].size(); j++)
                temp[9 - j][i] = matrix[i][j];

        matrix.swap(temp);
    }
};

void read_data(const std::string &filename)
{
    std::ifstream file("input.txt");
    std::string s;

    while (getline(file, s))
    {
        auto tile = new Tile();
        tile->id = atoi(s.c_str() + 5);

        while (getline(file, s) && s.length())
            tile->matrix.push_back(s);
        tiles.push_back(tile);
    }
}

bool matches_edges(int row, int col)
{
    if (row > 0)
        for (auto i = 0; i < 10; i++)
            if (tiles[grid[row][col]]->matrix[0][i] != tiles[grid[row - 1][col]]->matrix[9][i])
                return false;

    if (col > 0)
        for (auto i = 0; i < 10; i++)
            if (tiles[grid[row][col]]->matrix[i][0] != tiles[grid[row][col - 1]]->matrix[i][9])
                return false;

    return true;
}

auto make_image()
{
    std::vector<std::string> image;

    for (auto line = 0; line < 12; line++)
        for (auto i = 1; i < 9; i++)
        {
            std::string s;
            for (auto col = 0; col < 12; col++)
                for (auto j = 1; j < 9; j++)
                    s.push_back(tiles[grid[line][col]]->matrix[i][j]);

            image.push_back(s);
        }

    return image;
}

auto have_sea_monster_at(std::vector<std::string> &image, int x, int y)
{
    static const std::vector<std::string> sea_monster {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    };

    if (x + sea_monster.size() - 1 >= image.size() || y + sea_monster[0].size() - 1 >= image.size())
        return false;

    for (auto i = 0; i < sea_monster.size(); i++)
        for (auto j = 0; j < sea_monster[i].size(); j++)
            if (sea_monster[i][j] == '#' && image[x + i][y + j] != '#')
                return false;

    return true;
}

auto count_sea_monsters(std::vector<std::string>& image)
{
    auto sm = 0;
    for (auto i = 0; i < image.size(); i++)
        for (auto j = 0; j < image[i].size(); j++)
            sm += have_sea_monster_at(image, i, j);

    return sm;
}

auto check_solution(bool part2)
{
    if (!part2)
        return (long long)
            tiles[grid[0][11]]->id * tiles[grid[11][11]]->id *
            tiles[grid[11][0]]->id * tiles[grid[0][0]]->id;

    auto image = make_image();
    auto sm = count_sea_monsters(image);
    if (sm)
    {
        auto count = 0LL;
        for (auto &s : image)
            for (auto c : s)
                count += (c == '#');
        return count - 15 * sm; // sea monster has 15 #'s
    }

    return -1LL; // no sea monsters, not a solution, discard!
}

auto recurse(int row, int col, bool part2)
{
    if (row >= 12) // all rows filled
        return check_solution(part2);

    if (col >= 12) // all cols on row are filled, go to next row
        return recurse(row + 1, 0, part2);

    // loop over all tiles to fill next open spot
    for (auto i = 0; i < 144; i++)
    {
        if (placement[i].first >= 0) // if not (-1, -1), tile is already used; skip it.
            continue;

        // place this tile
        placement[i] = std::make_pair(row, col);
        grid[row][col] = i;

        // try out all flips and rotates
        for (auto j = 0; j < 2; j++, tiles[i]->flip())
            for (auto k = 0; k < 4; k++, tiles[i]->rotate())
                if (matches_edges(row, col))
                {
                    auto retval = recurse(row, col + 1, part2);
                    if (retval >= 0)
                        return retval; // found solution, end all recursion
                }

        placement[i] = std::make_pair(-1, -1);
    }

    return -1LL;
}

auto do_part(const std::string &filename, bool part2)
{
    placement.clear();
    placement.resize(tiles.size(), std::make_pair(-1, -1)); // unplace all tiles
    memset(grid, 0, sizeof(grid));

    return recurse(0, 0, part2);
}

int main()
{
    read_data("input.txt");

    auto part1 = do_part("input.txt", false);
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 13983397496713);
    assert(part2 == 2424);
    return 0;
}
