#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <cassert>

using Tower = std::vector<int>;
using Piece = std::vector<int>;

const std::vector<Piece> shapes =
{
    { 0x1e },
    { 0x08, 0x1c, 0x08 },
    { 0x1c, 0x04, 0x04 },
    { 0x10, 0x10, 0x10, 0x10 },
    { 0x18, 0x18 }
};

struct Key
{
    std::array<char, 7> hash;
    char rock_type;
    int jet_index;
};

struct Value
{
    long long num_rocks, tower_height;
};

auto operator<(const Key &lhs, const Key &rhs)
{
    if (lhs.hash < rhs.hash) return true;
    if (lhs.hash > rhs.hash) return false;
    if (lhs.rock_type < rhs.rock_type) return true;
    if (lhs.rock_type > rhs.rock_type) return false;
    return lhs.jet_index < rhs.jet_index;
}

auto collides(Tower &tower, Piece &curr_piece, int curr_piece_row)
{
    if (curr_piece_row < 0)
        return true;

    for (auto i = 0; i < curr_piece.size(); i++)
        if ((tower.size() >= curr_piece_row + i + 1) &&
            (curr_piece[i] & tower[curr_piece_row + i]))
            return true;

    return false;
}

void apply_jet(Tower &tower, const std::string &jets, int &jet, Piece &curr_piece, int curr_piece_row)
{
    auto j = jets[jet];
    if (++jet == jets.length())
        jet = 0;

    Piece new_piece;

    for (auto i = 0; i < curr_piece.size(); i++)
    {
        if (j == '<')
        {
            if (curr_piece[i] & 0x40) // at extreme left, don't move
                return;
            new_piece.push_back(curr_piece[i] << 1);
        }
        else
        {
            if (curr_piece[i] & 0x01) // at extreme right, don't move
                return;
            new_piece.push_back(curr_piece[i] >> 1);
        }
    }

    if (!collides(tower, new_piece, curr_piece_row))
        curr_piece = new_piece;
}

void place_piece_at_rest(Tower &tower, const Piece &piece, int curr_piece_row)
{
    while (tower.size() < curr_piece_row + piece.size())
        tower.push_back(0);
    for (auto i = 0; i < piece.size(); i++)
        tower[curr_piece_row + i] |= piece[i];
}

auto hash_tops(Tower &tower)
{
    auto retval = std::array<char, 7>();
    auto index = 0;

    for (auto bit = 0x40; bit; bit >>= 1)
    {
        auto depth = 0;
        for (int i = tower.size() - 1; i >= 0; i--)
            if (tower[i] & bit)
                break;
            else
                depth++;
        retval[index++] = depth;
    }
    return retval;
}

auto process_input(const std::string &filename, long long times)
{
    std::ifstream file(filename);
    std::string jets;
    file >> jets;

    Tower tower;
    Piece curr_piece;
    auto curr_piece_row = -1, curr_shape = 0, jet = 0;

    std::map<Key, Value> seen;
    auto skip_ahead = -1;
    auto future_full_cycles = -1LL, added_height_per_period = -1LL;

    for (auto rock_count = 1LL; rock_count <= times; rock_count++)
    {
        curr_piece = shapes[curr_shape];

        for (curr_piece_row = tower.size() + 3; ; --curr_piece_row)
        {
            apply_jet(tower, jets, jet, curr_piece, curr_piece_row);

            if (collides(tower, curr_piece, curr_piece_row - 1))
            {
                place_piece_at_rest(tower, curr_piece, curr_piece_row);
                break;
            }
        }

        if (++curr_shape == shapes.size())
            curr_shape = 0;

        if (skip_ahead > -1)
        {
            if (rock_count == skip_ahead)
                return tower.size() + future_full_cycles * added_height_per_period;
        }
        else
        {
            Key key{ hash_tops(tower), curr_shape, jet };
            auto value = seen.find(key);

            if (value != seen.end())
            {
                auto period = rock_count - value->second.num_rocks;
                added_height_per_period = tower.size() - value->second.tower_height;

                future_full_cycles = (times - rock_count) / period;
                auto modulo = (times - rock_count) % period;

                skip_ahead = rock_count + modulo;
            }
            else
                seen[key] = Value{ rock_count, (int)tower.size() };
        }
    }
}

int main()
{
    auto part1 = process_input("input.txt", 2022);
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = process_input("input.txt", 1000000000000);
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == 3071);
    assert(part2 == 1523615160362);
    return 0;
}
