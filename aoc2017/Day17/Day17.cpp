#include "stdafx.h"
#include <iostream>
#include <cassert>

struct Node
{
    int value;
    Node *prev, *next;
};

unsigned do_part1(unsigned input, unsigned n)
{
    auto list = new Node{ 0, nullptr, nullptr };
    list->prev = list;
    list->next = list;

    auto curr = list;

    Node *new_node = nullptr;

    for (auto i = 1; i <= 2017; i++)
    {
        auto loop_max = input % i;
        for (auto j = 0; j < loop_max; j++)
            curr = curr->next;

        new_node = new Node { i, curr->prev, curr->next };
        curr->prev->next = new_node;
        curr->next->prev = new_node;

        curr = new_node;
    }

    return new_node->next->value;
}

unsigned do_part2(unsigned input, unsigned n)
{
    auto curr = 0U, size = 1U, ret_val = 0U;

    for (auto i = 1U; i <= n; i++)
    {
        curr = (curr + input) % size;
        if (curr == 0)
            ret_val = i;

        curr++;
        size++;
    }

    return ret_val;
}

int main()
{
    const auto input = 344U;

    auto part1 = do_part1(input, 2017);
    std::cout << part1 << std::endl;

    auto part2 = do_part2(input, 50000000);
    std::cout << part2 << std::endl;

    assert(part1 == 996);
    assert(part2 == 1898341);
    return 0;
}

