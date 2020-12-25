#include <iostream>
#include <fstream>
#include <cassert>

auto transform(long long subject, long long loop_size)
{
    auto val = 1LL;

    for (auto i = 0; i < loop_size; i++)
        val = (val * subject) % 20201227;

    return val;
}

auto recover_loop_size(long long public_key)
{
    auto retval = 1LL;

    for (auto t = 7; t != public_key; t = (t * 7) % 20201227)
        retval++;

    return retval;
}

int main()
{
    std::ifstream file("input.txt");

    auto door_public_key = 0LL, card_public_key = 0LL;
    file >> door_public_key >> card_public_key;

    auto door_loop_size = recover_loop_size(door_public_key);
    auto card_loop_size = recover_loop_size(card_public_key);

    auto encryption_key1 = transform(door_public_key, card_loop_size);
    auto encryption_key2 = transform(card_public_key, door_loop_size);
    assert(encryption_key1 == encryption_key2);

    auto part1 = encryption_key1;

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: N/A" << std::endl;

    assert(part1 == 16457981);
    return 0;
}
