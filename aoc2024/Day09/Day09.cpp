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
    auto next_id = 0;
    auto c = ' ';
    std::vector<Chunk> disk;

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
    auto crossed = false;

    for (;;)
    {
        while (left <= right && disk[left].id != -1)
        {
            if (++left > right)
            {
                crossed = true;
                break;
            }
        }

        while (right >= left && disk[right].id == -1)
        {
            if (left > --right)
            {
                crossed = true;
                break;
            }
        }

        if (crossed)
            break;

        if (disk[left].blocks <= disk[right].blocks)
        {
            auto blocks_moving = disk[left].blocks;
            disk[left].id = disk[right].id;
            disk[right].blocks -= blocks_moving;

            // insert a free chunk immediately after 'right'
            disk.insert(disk.begin() + (right + 1), Chunk{ -1, blocks_moving });
        }
        else // left has more space than right needs
        {
            auto blocks_moving = disk[right].blocks;
            disk[left].blocks -= blocks_moving;

            // insert used chunk at position 'left'
            disk.insert(disk.begin() + left, Chunk{ disk[right].id, blocks_moving });

            // insertion before 'left' shifts indices >= left; original right moved to right+1
            right++;
            disk[right].id = -1;
        }
    }

    return compute_checksum(disk);
}

auto do_part2(const std::string &filename)
{
    auto disk = read_data(filename);

    for (auto right = static_cast<int>(disk.size()) - 1; right >= 0; --right)
    {
        if (disk[right].id == -1)
            continue;

        const int right_id = disk[right].id;
        const int right_blocks = disk[right].blocks;

        for (int left = 0; left < right; left++)
        {
            if (disk[left].id > -1 || disk[left].blocks < right_blocks)
                continue;

            // insert a used chunk before left
            disk.insert(disk.begin() + left, Chunk{ right_id, right_blocks });

            // original chunk at left moved to left+1; subtract moved blocks
            disk[left + 1].blocks -= right_blocks;

            // original right chunk shifted to right+1eft due to insertion at left (< right)
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
