#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

std::string salt = "zpqevtbw";
const int extraTimes = 2016; // CHANGE TO 0 FOR PART 1 OR 2016 FOR PART 2

typedef BYTE Hash[16];
typedef char HexHash[32];

void GetHashText(HCRYPTPROV hProv, const void *data, const size_t dataSize, HexHash hashed)
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

std::string HashInputWithSuffix(const std::string &input, unsigned suffix, unsigned times)
{
    char hashed[33];
    hashed[32] = '\0';

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        // Longest Unsigned is 10 chars, add one for null terminator
        std::vector<char> inBuffer(input.length() + 11);
        
        auto inputLength = sprintf_s(&inBuffer[0], inBuffer.capacity(), "%s", input.c_str());
        auto remainingBufferSize = inBuffer.capacity() - inputLength;
        auto suffixStart = &inBuffer[0] + inputLength;
        auto numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%u", suffix);

        GetHashText(hProv, &inBuffer[0], inputLength + numericSuffixSize, hashed);

        for (auto i = 0U; i < times; i++)
            GetHashText(hProv, hashed, 32, hashed);

        CryptReleaseContext(hProv, 0);
    }

    return hashed;
}

std::vector<std::string> hashCache;

std::string &GetHash(unsigned index)
{
    if (index >= hashCache.size())
    {
        for (auto i = hashCache.size(); i <= index; i++)
        {
            auto hash = HashInputWithSuffix(salt, i, extraTimes);
            hashCache.push_back(hash);
        }
    }

    return hashCache[index];
}

char FindFirstTriple(const std::string &s)
{
    for (auto i = 0U; i <= s.length() - 3; i++)
        if (s[i] == s[i + 1] && s[i] == s[i + 2])
            return s[i];

    return '\0';
}

bool HasQuintuple(unsigned start, char triple)
{
    for (int i = 1; i <= 1000; i++)
    {
        auto hash2 = GetHash(start + i);

        for (auto j = 0U; j <= hash2.length() - 5; j++)
        {
            if (hash2[j] == triple &&
                hash2[j + 1] == triple &&
                hash2[j + 2] == triple &&
                hash2[j + 3] == triple &&
                hash2[j + 4] == triple)
            {
                return true;
            }
        }
    }

    return false;
}

int main()
{
    int hashes = 0;
    unsigned answer = 0;

    for (auto curr = 0; !answer; curr++)
    {
        auto hash = GetHash(curr);
        char triple = FindFirstTriple(hash);

        if (triple && HasQuintuple(curr, triple))
        {
            hashes++;
            std::cout << "KEY #" << hashes << " is " << curr << std::endl;
            if (hashes == 64)
            {
                answer = curr;
                std::cout << "Answer: " << answer << std::endl;
            }
        }
    }

    //assert(answer == 16106); // part 1
    //assert(answer == 22423); // part 2
    return 0;
}
