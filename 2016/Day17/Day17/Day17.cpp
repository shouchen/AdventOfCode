#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <iostream>
#include <string>
#include <queue>
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
    unsigned x, y;
};

std::queue<QueueNode> queue;
std::string bestSolution;
unsigned worstSolution;

bool Solve(const std::string &input)
{
    if (queue.empty())
        return true;

    QueueNode node = queue.front();
    queue.pop();

    if (node.x == 3 && node.y == 3)
    {
        if (node.moves.length() < bestSolution.length() || bestSolution.length() == 0)
            bestSolution = node.moves;

        if (node.moves.length() > worstSolution)
            worstSolution = node.moves.length();

        return false;
    }

    // Get hash of <input>+pathSoFar;
    auto hash = HashInput(input + node.moves);

    // Schedule that node to be processed if it works.
    if (node.y > 0 && hash[0] > 'a')
        queue.push(QueueNode(node.moves + "U", node.x, node.y - 1U));

    if (node.y < 3 && hash[1] > 'a')
        queue.push(QueueNode(node.moves + "D", node.x, node.y + 1U));

    if (node.x > 0 && hash[2] > 'a')
        queue.push(QueueNode(node.moves + "L", node.x - 1U, node.y));

    if (node.x < 3 && hash[3] > 'a')
        queue.push(QueueNode(node.moves + "R", node.x + 1U, node.y));

    return false;
}

int main()
{
    double startTime = clock();

    queue.push(QueueNode("", 0, 0));
    while (!Solve("dmypynyp"));

    std::cout << "Part One: " << bestSolution << std::endl;
    std::cout << "Part Two: " << worstSolution << std::endl;

    assert(bestSolution == "RDRDUDLRDR");
    assert(worstSolution == 386);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}
