#include "stdafx.h"
#include <iostream>
#include <vector>
#include <cassert>

#define MAX 3001330

struct Base
{
    struct Node
    {
        unsigned initialSpot;
        unsigned presents;
        Node *next, *prev;
    };

    Node *head = nullptr, *current = nullptr, *other = nullptr;
    unsigned count = 0;

    virtual void PostProcess() = 0;

    void Initialize()
    {
        for (auto i = 1U; i <= MAX; i++)
            Append(new Node{ i, 1 });
    }

    virtual void Append(Node *node)
    {
        if (head)
        {
            node->next = head;
            node->prev = head->prev;

            head->prev->next = node;
            head->prev = node;

            count++;
        }
        else
        {
            head = current = other = node;
            current->next = current;
            current->prev = current;
            count = 1;
        }
    }

    void Remove(Node *node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        count--;
    }

    unsigned Solve()
    {
        Initialize();

        do
        {
            current->presents += other->presents;
            PostProcess();
        } while (current->presents < MAX);

        return current->initialSpot;
    }
};

struct Part1 : public Base
{
    void Append(Node *node) override
    {
        Base::Append(node);

        if (count == 2)
            other = node;
    }

    void PostProcess() override
    {
        Remove(other);
        current = current->next;
        other = current->next;
    }
} part1;

struct Part2 : public Base
{
    void Append(Node *node) override
    {
        Base::Append(node);

        if ((count & 0x1) == 0)
            other = other->next;
    }

    void PostProcess() override
    {
        Node *nextOpposite = other->next;
        if (count & 0x1)
            nextOpposite = nextOpposite->next;

        Remove(other);
        current = current->next;
        other = nextOpposite;
    }
} part2;

int main()
{
    auto answer1 = part1.Solve();
    std::cout << "Part One: " << answer1 << std::endl;

    auto answer2 = part2.Solve();
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == 1808357);
    assert(answer2 == 1407007);
    return 0;
}
