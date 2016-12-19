#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cassert>

class SolverBase
{
public:
    unsigned Solve(unsigned numElves)
    {
        Initialize(numElves);

        do
        {
            current->presents += other->presents;
            PostProcess();
        } while (current->presents < numElves);

        auto retval = current->initialSpot;
        delete current;
        return retval;
    }

protected:
    struct Node
    {
        unsigned initialSpot, presents;
        Node *next, *prev;
    };

    void Initialize(unsigned numElves)
    {
        for (auto i = 1U; i <= numElves; i++)
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

        AdjustOtherAfterAppend();
    }

    void Remove(Node *node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        count--;
    }

    virtual void AdjustOtherAfterAppend() = 0;
    virtual void PostProcess() = 0;

    Node *head = nullptr, *current = nullptr, *other = nullptr;
    unsigned count = 0;
};

class SolverPart1 : public SolverBase
{
protected:
    void AdjustOtherAfterAppend() override
    {
        if (count == 2)
            other = head->next;
    }

    void PostProcess() override
    {
        Remove(other);
        current = current->next;
        other = current->next;
    }
};

struct SolverPart2 : public SolverBase
{
protected:
    void AdjustOtherAfterAppend() override
    {
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
};

int main()
{
    double startTime = clock();

    const unsigned numElves = 3001330;

    SolverPart1 solverPart1;
    auto answer1 = solverPart1.Solve(numElves);
    std::cout << "Part One: " << answer1 << std::endl;

    SolverPart2 solverPart2;
    auto answer2 = solverPart2.Solve(numElves);
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == 1808357);
    assert(answer2 == 1407007);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}
