#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cassert>

using Hash = BYTE[16];
using HexHash = char[32];

static void GetHashText(HCRYPTPROV hProv, const void *data, const DWORD dataSize, HexHash hashed)
{
    HCRYPTPROV hHash = NULL;

    if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        if (CryptHashData(hHash, reinterpret_cast<const BYTE *>(data), dataSize, 0))
        {
            Hash buffer;
            DWORD cbHashSize = sizeof(Hash);

            if (CryptGetHashParam(hHash, HP_HASHVAL, buffer, &cbHashSize, 0))
            {
                static const char hexdigit[] = "0123456789abcdef";

                char *curr = hashed;
                for (auto i = 0U; i < cbHashSize; i++)
                {
                    auto x = buffer[i];
                    *curr++ = hexdigit[(x >> 4) & 0xf];
                    *curr++ = hexdigit[x & 0xf];
                }
            }
        }

        CryptDestroyHash(hHash);
    }
}

static std::string HashInputWithPath(const std::string &input, const std::string &path)
{
    char hashed[33];
    hashed[32] = '\0';

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        std::string buffer = input + path;

        //auto inputLength = sprintf_s(&inBuffer[0], inBuffer.capacity(), "%s", salt.c_str());
        //auto remainingBufferSize = inBuffer.capacity() - inputLength;
        //auto suffixStart = &inBuffer[0] + inputLength;
        //auto numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%u", suffix);

        GetHashText(hProv, buffer.c_str(), buffer.length(), hashed);

        //for (auto i = 0U; i < extraTimes; i++)
        //    GetHashText(hProv, hashed, 32, hashed);

        CryptReleaseContext(hProv, 0);
    }

    return hashed;
}

struct QueueNode
{
    QueueNode(const std::string moves, unsigned x, unsigned y) : moves(moves), x(x), y(y) {}
    std::string moves;
    unsigned x = 0, y = 0;

    bool operator<(const QueueNode &rhs) const
    {
        auto leftAStar = moves.length() + (3 - x) + (3 - y);
        auto rightAStar = rhs.moves.length() + (3 - rhs.x) + (3 - rhs.y);

        if (leftAStar > rightAStar) return true;
        if (leftAStar < rightAStar) return false;

        if (moves > rhs.moves) return true;
        if (moves < rhs.moves) return false;

        if (x > rhs.x) return true;
        if (x < rhs.x) return false;

        if (y > rhs.y) return true;
        if (y < rhs.y) return false;

        return false;
    }
};

std::priority_queue<QueueNode> queue;
std::string bestSolution;

bool Solve(const std::string &input)
{
    if (queue.empty())
        return true;

    QueueNode node = queue.top();
    queue.pop();

    //std::cout << "Working on (" << node.x << "," << node.y << ") " << node.moves << std::endl;

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
    auto hash = HashInputWithPath(input, node.moves);

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

int main()
{
    std::string input("dmypynyp");

    queue.push(QueueNode("", 0, 0));
    while (!Solve(input));

    std::cout << "Part One: " << bestSolution << std::endl;

    assert(bestSolution == "RDRDUDLRDR");

    return 0;
}
