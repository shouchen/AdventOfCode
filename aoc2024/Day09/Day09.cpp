#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

struct Chunk
{
    int id, blocks;
};

auto sum_of_integers(unsigned long long up_to)
{
    return (up_to * (up_to + 1)) / 2;
}

auto sum_of_integers(int starting, int length)
{
    return sum_of_integers(starting + length - 1) - sum_of_integers(starting - 1);
}

auto compute_checksum(const std::vector<Chunk> &disk)
{
    auto position = 0;
    auto checksum = 0ULL;

    for (const auto &b : disk)
    {
        if (b.id != -1)
            checksum += b.id * sum_of_integers(position, b.blocks);

        position += b.blocks;
    }

    return checksum;
}

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<Chunk> disk;
    auto next_id = 0;
    auto c = ' ';

    while (file >> c)
    {
        disk.push_back({ next_id++, c - '0' });
        if (file >> c)
            disk.push_back({ -1, c - '0' });
    }

    return disk;
}

auto do_part1(const std::string &filename)
{
    auto disk = read_data(filename);
    auto left = 0, right = int(disk.size()) - 1;

    for (;;)
    {
        while (left <= right && disk[left].id != -1)
            if (++left > right)
                return compute_checksum(disk);

        while (right >= left && disk[right].id == -1)
            if (left > --right)
                return compute_checksum(disk);

        if (disk[left].blocks <= disk[right].blocks)
        {
            auto blocks_moving = disk[left].blocks;
            disk[left].id = disk[right].id;
            disk[right].blocks -= blocks_moving;
            disk.insert(disk.begin() + (right + 1), Chunk{ -1, blocks_moving });
        }
        else
        {
            auto blocks_moving = disk[right].blocks;
            disk[left].blocks -= blocks_moving;
            disk.insert(disk.begin() + left, Chunk{ disk[right].id, blocks_moving });
            disk[++right].id = -1;
        }
    }
}

auto do_part2(const std::string &filename)
{
    auto disk = read_data(filename);

    for (auto right = int(disk.size()) - 1; right >= 0; --right)
    {
        if (disk[right].id == -1)
            continue;

        auto blocks_moving = disk[right].blocks;

        for (auto left = 0; left < right; left++)
        {
            if (disk[left].id > -1 || disk[left].blocks < blocks_moving)
                continue;

            disk.insert(disk.begin() + left, Chunk{ disk[right].id, blocks_moving });
            disk[left + 1].blocks -= blocks_moving;
            disk[right + 1].id = -1;
            break;
        }
    }

    return compute_checksum(disk);
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 6360094256423);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 6379677752410);

    return 0;
}
