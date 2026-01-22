#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cassert>

struct Chunk
{
    int id, blocks;
};

std::list<Chunk> disk;

auto sum_of_integers(unsigned long long up_to)
{
    return (up_to * (up_to + 1)) / 2;
}

auto sum_of_integers(int starting, int length)
{
    return sum_of_integers(starting + length - 1) - sum_of_integers(starting - 1);
}

auto compute_checksum()
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

void read_data(const std::string &filename)
{
    std::ifstream file(filename);
    auto next_id = 0;
    auto c = ' ';

    disk.clear();

    while (file >> c)
    {
        disk.push_back({ next_id++, c - '0' });
        if (file >> c)
            disk.push_back({ -1, c - '0' });
    }
}

auto do_part1(const std::string &filename)
{
    read_data(filename);

    // Look at leftmost free chunk and rightmost non-free chunk.
    // Case 1: Left side is >= than right size: Insert a chunk at left with the ID of the rightmost
    // chunk, then reduce the free space on the left accordingly. Also adjust the right free space.
    // May need to consolidate with adjacent free spaces.
    // Case 2: Left side is < right size: Change left's ID to fill it. Right's ID reduces the used size
    // and either inserts a free block after it or else increases the size of an existing free count.

    auto left = disk.begin();
    auto right = disk.rbegin();
    auto crossed = false;

    for (;;)
    {
        while (left->id != -1/* && left != disk.end()*/)
        {
            left++;
            if (left == right.base())
            {
                crossed = true;
                break;
            }
        }

        while (right->id == -1/* && right != disk.rend()*/)
        {
            right++;
            if (left == right.base())
            {
                crossed = true;
                break;
            }
        }

        if (crossed)
            break;

        if (left->blocks <= right->blocks)
        {
            auto blocks_moving = left->blocks;
            left->id = right->id;
            right->blocks -= blocks_moving;

            // TODO: may sometimes need to just increase existing free space
            disk.insert(right.base(), { -1, blocks_moving });
        }
        else // left has more space than right needs
        {
            auto blocks_moving = right->blocks;
            left->blocks -= blocks_moving;

            // TODO: May need to consolidate
            disk.insert(left, { right->id, blocks_moving });
            right->id = -1;
        }
    }

    return compute_checksum();
}

auto do_part2(const std::string &filename)
{
    read_data(filename);

    for (auto right = disk.rbegin(); right != disk.rend(); right++)
    {
        if (right->id == -1)
            continue;

        for (auto left = disk.begin(); left != right.base(); left++)
        {
            if (left->id > -1 || left->blocks < right->blocks)
                continue;

            disk.insert(left, { right->id, right->blocks });
            left->blocks -= right->blocks;
            right->id = -1;

            break;
        }
    }

    return compute_checksum();
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
