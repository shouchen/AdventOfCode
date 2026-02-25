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

std::string bestSolution;
unsigned worstSolution;
const auto startX = 0U, startY = 0U, goalX = 3U, goalY = 3U, maxX = 3U, maxY = 3U;

void Solve(const std::string &input, std::string &moves, unsigned x, unsigned y)
{
    if (x == goalX && y == goalY)
    {
        if (moves.length() < bestSolution.length() || bestSolution.length() == 0)
            bestSolution = moves;

        if (moves.length() > worstSolution)
            worstSolution = moves.length();

        return;
    }

    auto hash = HashInput(input + moves);

    // Recurse through all possible next steps.
    if (y > 0 && hash[0] > 'a')
    {
        moves.push_back('U');
        Solve(input, moves, x, y - 1);
        moves.pop_back();
    }

    if (y < maxY && hash[1] > 'a')
    {
        moves.push_back('D');
        Solve(input, moves, x, y + 1);
        moves.pop_back();
    }

    if (x > 0 && hash[2] > 'a')
    {
        moves.push_back('L');
        Solve(input, moves, x - 1, y);
        moves.pop_back();
    }

    if (x < maxX && hash[3] > 'a')
    {
        moves.push_back('R');
        Solve(input, moves, x + 1, y);
        moves.pop_back();
    }
}

void Solve(const std::string &input)
{
    std::string moves;
    Solve(input, moves, startX, startY);
}

int main()
{
    double startTime = clock();

    Solve("dmypynyp");

    std::cout << "Part One: " << bestSolution << std::endl;
    std::cout << "Part Two: " << worstSolution << std::endl;

    assert(bestSolution == "RDRDUDLRDR");
    assert(worstSolution == 386);

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}
