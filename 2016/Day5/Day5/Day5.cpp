// Day5.cpp : Defines the entry point for the console application.
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

void NextFiveZeros(const std::string &input, unsigned long long &start, char &sixth, char &seventh)
{
    char retval = ' ';

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        std::vector<char> buffer(input.length() + GetLengthLongestUnsignedLongLong() + 1);

        auto inputLength = sprintf_s(&buffer[0], buffer.capacity(), "%s", input.c_str());
        auto remainingBufferSize = buffer.capacity() - inputLength;
        auto suffixStart = &buffer[0] + inputLength;
        auto prefix = std::string(5, '0');

        for (auto curr = start; curr < ULLONG_MAX; curr++)
        {
            int numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%llu", curr);

            std::string hashed = GetHashText(hProv, &buffer[0], inputLength + numericSuffixSize);
            if (hashed.substr(0, 5) == prefix)
            {
                std::cout << "h> " << hashed << std::endl;
                sixth = hashed[5];
                seventh = hashed[5 + 1];
                start = curr + 1;
                break;
            }
        }

        CryptReleaseContext(hProv, 0);
    }
}

int main()
{
    auto input = "reyedfim";
    std::string answer1, answer2;
    char buffer[] = "        ";
    char sixth, seventh;
    auto start = 0ULL;

    for (;;)
    {
        NextFiveZeros(input, start, sixth, seventh);

        if (answer1.length() < 8)
        {
            answer1 += sixth;
            std::cout << "1> " << answer1 << std::endl;
        }

        if (sixth >= '0' && sixth <= '7')
        {
            int position = sixth - '0';
            if (buffer[position] == ' ')
            {
                buffer[position] = seventh;
                answer2 = buffer;
                std::cout << "2> " << answer2 << std::endl;
            }

            if (buffer[0] != ' ' && buffer[1] != ' ' && buffer[2] != ' ' && buffer[3] != ' ' &&
                buffer[4] != ' ' && buffer[5] != ' ' && buffer[6] != ' ' && buffer[7] != ' ')
            {
                break;
            }
        }
    }

    std::cout << "Part One: " << answer1 << std::endl;
    assert(answer1 == "f97c354d");

    std::cout << "Part Two: " << answer2 << std::endl;
    assert(answer2 == "863dde27");

    return 0;
}
