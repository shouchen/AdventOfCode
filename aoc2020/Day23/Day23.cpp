#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cassert>

struct Node
{
    int label;
    Node *prev, *next;
} *lookup[1000001], *head;

void push_back(int label)
{
    auto node = new Node{ label };
    lookup[label] = node;

    if (head)
    {
        node->prev = head->prev;
        node->next = head;
        head->prev = head->prev->next = node;
    }
    else
    {
        head->next = head->prev = head = node;
    }
}

void clean_up()
{
    auto curr = head;

    do
    {
        lookup[curr->label] = NULL;
        auto temp = curr;
        curr = curr->next;
        delete temp;
    } while (curr != head);

    head = NULL;
}

void play_game(const std::string &filename, int pad_to, int times)
{
    std::ifstream file(filename);
    char c;
    int hi = 0;

    while (file >> c)
    {
        auto val = c - '0';
        hi = std::max(hi, val);
        push_back(val);
    }

    for (auto i = hi + 1; i <= pad_to; i++)
        push_back(i);

    auto curr = head;

    while (times--)
    {
        // disconnect the next three
        auto move1 = curr->next;
        auto move2 = move1->next;
        auto move3 = move2->next;

        curr->next = move3->next;
        curr->next->prev = curr;

        // find dest
        Node *dest = NULL;
        auto lbl = curr->label;
        for (;;)
        {
            if (--lbl < 1)
                lbl = pad_to;
            if (move1->label != lbl && move2->label != lbl && move3->label != lbl)
                break;
        }
        dest = lookup[lbl];

        // splice in the three
        move1->prev = dest;
        move3->next = dest->next;
        dest->next->prev = move3;
        dest->next = move1;

        // advance
        curr = curr->next;
    }
}

auto do_part1(const std::string &filename)
{
    play_game(filename, 9, 100);

    std::string retval;
    for (auto curr = lookup[1]->next; curr->label != 1; curr = curr->next)
        retval.push_back(curr->label + '0');

    clean_up();
    return retval;
}

auto do_part2(const std::string &filename)
{
    play_game(filename, 1000000, 10000000);

    auto one = lookup[1];
    auto retval = long long(one->next->label) * one->next->next->label;

    clean_up();
    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == "96342875");
    assert(part2 == 563362809504);
    return 0;
}
