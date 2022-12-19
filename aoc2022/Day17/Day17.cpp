#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<int> tower;

//vector<int> minus_piece{ 0x1e };
//vector<int> plus_piece{ 0x08, 0x1c, 0x08 };
//vector<int> l_piece{ 0x04, 0x04, 0x1c };
//vector<int> i_piece{ 0x20, 0x20, 0x20, 0x20 };
//vector<int> box_piece{ 0x30, 0x30 };

std::vector<std::vector<int>> shapes =
{
    { 0x1e },
    { 0x08, 0x1c, 0x08 },
    { 0x1c, 0x04, 0x04 },
    { 0x10, 0x10, 0x10, 0x10 },
    { 0x18, 0x18 }
};

std::vector<int> curr_piece;
int curr_piece_row;


void dump()
{
    auto height = tower.size();
    if (curr_piece_row > -1)
        height = std::max(height, curr_piece_row + curr_piece.size());

    for (int i = height - 1; i >= 0; i--)
    {
        std::cout << '|';
        for (auto bit = 0x40; bit; bit >>= 1)
        {
            if (curr_piece_row != -1 && i >= curr_piece_row && i < curr_piece_row + curr_piece.size() && (curr_piece[i - curr_piece_row] & bit))
                std::cout << '@';
            else
                std::cout << ((i < tower.size() && (tower[i] & bit)) ? '#' : '.');
        }
        std::cout << '|' << std::endl;
    }
    std::cout << "+-------+" << std::endl << std::endl;
}

auto collides()
{
    if (curr_piece_row < 0)
        return true;

    for (auto i = 0; i < curr_piece.size(); i++)
    {
        if ((tower.size() >= curr_piece_row + i + 1) &&
            (curr_piece[i] & tower[curr_piece_row + i]))
            return true;
    }

    return false;
}

void apply_jet(char jet)
{
    switch (jet)
    {
    case '<':
        for (int i = 0; i < curr_piece.size(); i++)
            if (curr_piece[i] & 0x40) // at extreme left
                return;

        for (int i = 0; i < curr_piece.size(); i++)
            curr_piece[i] <<= 1;
        if (collides())
            for (int i = 0; i < curr_piece.size(); i++)
                curr_piece[i] >>= 1;
        break;

    case '>':
        for (int i = 0; i < curr_piece.size(); i++)
            if (curr_piece[i] & 0x01) // at extreme right
                return;

        for (int i = 0; i < curr_piece.size(); i++)
            curr_piece[i] >>= 1;
        if (collides())
            for (int i = 0; i < curr_piece.size(); i++)
                curr_piece[i] <<= 1;
        break;

    default:
        assert(false);
    }
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string jets;

    file >> jets;

    auto floor = 0, curr_top = 0, curr_bottom = 0, curr_left = 0, curr_right = 0;

    int curr_shape = 0;
    int jet = 0;
    int rock_count = 0;

    for (;;)
    {
        if (rock_count == 2022)
            return tower.size();

        curr_piece = shapes[curr_shape];
        if (++curr_shape == shapes.size()) curr_shape = 0;
        curr_piece_row = tower.size() + 3;
        //dump();
        std::cout << std::endl;

        for (;;)
        {
            apply_jet(jets[jet]);
            if (++jet == jets.size()) jet = 0;
            //dump();

            curr_piece_row--;
            if (collides())
            {
                curr_piece_row++;
                break;
            }
            //dump();
        }

        while (tower.size() < curr_piece_row + curr_piece.size())
            tower.push_back(0);
        for (int i = 0; i < curr_piece.size(); i++)
            tower[curr_piece_row + i] |= curr_piece[i];
        curr_piece_row = -1;

        rock_count++;

        //dump();
        std::cout << std::endl;
    }
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    //auto part2 = process_data("test.txt");
    //std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3071);
    //assert(part2 == "PQTJRSHWS");
    return 0;
}
