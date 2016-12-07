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

std::string GetNextHashWithFiveLeadingZeros(const std::string &input, unsigned long long &start)
{
    std::string hashed;

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
            auto numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%llu", curr);

            hashed = GetHashText(hProv, &buffer[0], inputLength + numericSuffixSize);
            if (hashed.substr(0, 5) == prefix)
            {
                start = curr + 1;
                break;
            }
        }

        CryptReleaseContext(hProv, 0);
    }

    return hashed;
}

int main()
{
    auto input = "reyedfim"; // TODO: read from a file
    std::string answer1, answer2;
    char buffer[] = "        ";
    auto start = 0ULL;

    for (;;)
    {
        auto hashed = GetNextHashWithFiveLeadingZeros(input, start);

        if (answer1.length() < 8)
        {
            answer1.push_back(hashed[5]);
        }

        if (hashed[5] >= '0' && hashed[5] <= '7')
        {
            auto position = hashed[5] - '0';
            if (buffer[position] == ' ')
            {
                buffer[position] = hashed[6];
                answer2 = buffer;
            }

            if (buffer[0] != ' ' && buffer[1] != ' ' && buffer[2] != ' ' && buffer[3] != ' ' &&
                buffer[4] != ' ' && buffer[5] != ' ' && buffer[6] != ' ' && buffer[7] != ' ')
            {
                break;
            }
        }
    }

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == "f97c354d");
    assert(answer2 == "863dde27");
    return 0;
}
