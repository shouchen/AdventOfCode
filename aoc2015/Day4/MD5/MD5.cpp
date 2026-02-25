// MD5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cassert>

std::string GetHashText(HCRYPTPROV hProv, const void *data, const size_t dataSize)
{
    std::ostringstream oss;
    HCRYPTPROV hHash = NULL;

    if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        DWORD cbHashSize = 0, dwCount = sizeof(DWORD);

        if (CryptHashData(hHash, static_cast<const BYTE *>(data), dataSize, 0) &&
            CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&cbHashSize, &dwCount, 0))
        {
            std::vector<BYTE> buffer(cbHashSize);

            if (CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<BYTE *>(&buffer[0]), &cbHashSize, 0))
            {
                for (std::vector<BYTE>::const_iterator iter = buffer.begin(); iter != buffer.end(); ++iter)
                {
                    oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<const int>(*iter);
                }
            }
        }

        CryptDestroyHash(hHash);
    }

    return oss.str();
}

unsigned GetLengthLongestUnsignedLongLong()
{
    std::ostringstream oss;
    oss << ULLONG_MAX;
    return oss.str().length();
}

unsigned long long SolveForInput(const std::string &input, unsigned numZeros, unsigned long long start = 0ULL)
{
    auto curr = start;

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        std::vector<char> buffer(input.length() + GetLengthLongestUnsignedLongLong() + 1);

        auto inputLength = sprintf_s(&buffer[0], buffer.capacity(), "%s", input.c_str());
        auto remainingBufferSize = buffer.capacity() - inputLength;
        auto suffixStart = &buffer[0] + inputLength;
        auto prefix = std::string(numZeros, '0');

        for (curr = start; curr < ULLONG_MAX; curr++)
        {
            int numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%llu", curr);

            std::string hashed = GetHashText(hProv, &buffer[0], inputLength + numericSuffixSize);
            if (hashed.substr(0, numZeros) == prefix)
                break;
        }

        CryptReleaseContext(hProv, 0);
    }

    return curr;
}

void _tmain(int argc, _TCHAR *argv[])
{
    auto input = "bgvyzdsv";

    auto part1 = SolveForInput(input, 5);
    std::cout << "part one: " << part1 << std::endl;
    assert(part1 == 254575);

    auto part2 = SolveForInput(input, 6, part1);
    std::cout << "part two: " << part2 << std::endl;
    assert(part2 == 1038736);
}
