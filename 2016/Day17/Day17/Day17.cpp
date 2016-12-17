#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <ctime>
#include <cassert>

static std::string HashInput(const std::string &input)
{
    std::string retval;
    HCRYPTPROV hProv = NULL;

    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        HCRYPTPROV hHash = NULL;

        if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
        {
            if (CryptHashData(hHash, reinterpret_cast<const BYTE *>(input.c_str()), input.length(), 0))
            {
                BYTE buffer[16];
                DWORD cbHashSize = sizeof(buffer);

                if (CryptGetHashParam(hHash, HP_HASHVAL, buffer, &cbHashSize, 0))
                {
                    static const char hexdigit[] = "0123456789abcdef";

                    for (auto i = 0U; i < cbHashSize; i++)
                    {
                        auto x = buffer[i];
                        retval.push_back(hexdigit[(x >> 4) & 0xf]);
                        retval.push_back(hexdigit[x & 0xf]);
                    }
                }
            }

            CryptDestroyHash(hHash);
        }

        CryptReleaseContext(hProv, 0);
    }

    return retval;
}

struct QueueNode
{
    QueueNode(const std::string moves, unsigned x, unsigned y) : moves(moves), x(x), y(y) {}
    std::string moves;
    unsigned x = 0, y = 0;

    //bool operator<(const QueueNode &rhs) const
    //{
    //    auto leftAStar = moves.length() + (3 - x) + (3 - y);
    //    auto rightAStar = rhs.moves.length() + (3 - rhs.x) + (3 - rhs.y);

    //    if (leftAStar > rightAStar) return true;
    //    if (leftAStar < rightAStar) return false;

    //    if (moves > rhs.moves) return true;
    //    if (moves < rhs.moves) return false;

    //    if (x > rhs.x) return true;
    //    if (x < rhs.x) return false;

    //    if (y > rhs.y) return true;
    //    if (y < rhs.y) return false;

    //    return false;
    //}

    //bool operator>(const QueueNode &rhs) const
    //{
    //    auto leftAStar = moves.length() + (0 - x) + (0 - y);
    //    auto rightAStar = rhs.moves.length() + (0 - rhs.x) + (0 - rhs.y);
    //    
    //    if (leftAStar < rightAStar) return true;
    //    if (leftAStar > rightAStar) return false;
    //    
    //    if (moves < rhs.moves) return true;
    //    if (moves > rhs.moves) return false;
    //    
    //    if (x < rhs.x) return true;
    //    if (x > rhs.x) return false;
    //    
    //    if (y < rhs.y) return true;
    //    if (y > rhs.y) return false;
    //    
    //    return false;
    //}
};

std::queue<QueueNode> queue;
std::queue<QueueNode> queue2;

std::string bestSolution;
unsigned worstSolution = 0;

bool InternalSolvePart1(const std::string &input)
{
    if (queue.empty())
        return true;

    QueueNode node = queue.front();
    queue.pop();

    if (node.x == 3 && node.y == 3)
    {
        if (node.moves.length() < bestSolution.length() || bestSolution.length() == 0)
            bestSolution = node.moves;

        return false;
    }

    // If a better solution was already found, don't proceed.
    if (bestSolution.length() && node.moves.length() >= bestSolution.length())
        return false;

    // Get hash of <input>+pathSoFar;
    auto hash = HashInput(input + node.moves);

    // Schedule that node to be processed if it works.
    if (node.y > 0 && hash[0] > 'a')
        queue.push(QueueNode(node.moves + "U", node.x, node.y - 1U ));

    if (node.y < 3 && hash[1] > 'a')
        queue.push(QueueNode(node.moves + "D", node.x, node.y + 1U ));

    if (node.x > 0 && hash[2] > 'a')
        queue.push(QueueNode(node.moves + "L", node.x - 1U, node.y ));

    if (node.x < 3 && hash[3] > 'a')
        queue.push(QueueNode(node.moves + "R", node.x + 1U, node.y ));

    return false;
}

bool InternalSolvePart2(const std::string &input)
{
    if (queue2.empty())
        return true;

    QueueNode node = queue2.front();
    queue2.pop();

    if (node.x == 3 && node.y == 3)
    {
        if (node.moves.length() > worstSolution)
            worstSolution = node.moves.length();

        return false;
    }

    // Get hash of <input>+pathSoFar;
    auto hash = HashInput(input + node.moves);

    // Schedule that node to be processed if it works.
    if (node.y > 0 && hash[0] > 'a')
        queue2.push(QueueNode(node.moves + "U", node.x, node.y - 1U));

    if (node.y < 3 && hash[1] > 'a')
        queue2.push(QueueNode(node.moves + "D", node.x, node.y + 1U));

    if (node.x > 0 && hash[2] > 'a')
        queue2.push(QueueNode(node.moves + "L", node.x - 1U, node.y));

    if (node.x < 3 && hash[3] > 'a')
        queue2.push(QueueNode(node.moves + "R", node.x + 1U, node.y));

    return false;
}

std::string SolvePart1(const std::string &input)
{
    queue.push(QueueNode("", 0, 0));
    while (!InternalSolvePart1(input));

    return bestSolution;
}

unsigned SolvePart2(const std::string &input)
{
    queue2.push(QueueNode("", 0, 0));
    while (!InternalSolvePart2(input));

    return worstSolution;
}

int main()
{
    double startTime = clock();

    std::string input("dmypynyp");

    auto part1 = SolvePart1(input);
    auto part2 = SolvePart2(input);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(part1 == "RDRDUDLRDR");
    assert(part2 == 386);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    return 0;
}

// Merge the two solve methods

// Other general clean-up
