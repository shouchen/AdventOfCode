#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

class Solver
{
public:
    static unsigned Solve(const std::string &salt, unsigned extraTimes)
    {
        Solver::salt = salt;
        Solver::extraTimes = extraTimes;

        auto hashes = 0U, curr = 0U;
        hashCache.clear();

        for (;;)
        {
            char triple = FindFirstTriple(GetHash(curr));

            if (triple && HasQuintuple(curr, triple))
            {
                std::cout << "KEY #" << ++hashes << " is " << curr << std::endl;
                if (hashes == 64)
                    return curr;
            }

            curr++;
        }
    }

private:
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

    static std::string HashInputWithSuffix(const std::string &salt, unsigned suffix)
    {
        char hashed[33];
        hashed[32] = '\0';

        HCRYPTPROV hProv = NULL;
        if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        {
            // Longest Unsigned is 10 digits, then add one for null terminator
            std::vector<char> inBuffer(salt.length() + 11);

            auto inputLength = sprintf_s(&inBuffer[0], inBuffer.capacity(), "%s", salt.c_str());
            auto remainingBufferSize = inBuffer.capacity() - inputLength;
            auto suffixStart = &inBuffer[0] + inputLength;
            auto numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%u", suffix);

            GetHashText(hProv, &inBuffer[0], inputLength + numericSuffixSize, hashed);

            for (auto i = 0U; i < extraTimes; i++)
                GetHashText(hProv, hashed, 32, hashed);

            CryptReleaseContext(hProv, 0);
        }

        return hashed;
    }

    static std::string &GetHash(unsigned index)
    {
        if (index >= hashCache.size())
        {
            for (auto i = hashCache.size(); i <= index; i++)
            {
                auto hash = HashInputWithSuffix(salt, static_cast<unsigned>(i));
                hashCache.push_back(hash);
            }
        }

        return hashCache[index];
    }

    static char FindFirstTriple(const std::string &s)
    {
        for (auto i = 0U; i <= s.length() - 3; i++)
            if (s[i] == s[i + 1] && s[i] == s[i + 2])
                return s[i];

        return '\0';
    }

    static bool HasQuintuple(unsigned start, char triple)
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

    static std::vector<std::string> hashCache;
    static std::string salt;
    static unsigned extraTimes;
};

std::vector<std::string> Solver::hashCache;
std::string Solver::salt;
unsigned Solver::extraTimes;

int main()
{
    const std::string salt = "zpqevtbw";

    auto part1 = Solver::Solve(salt, 0);
    std::cout << "Part One: " << part1 << std::endl << std::endl;
    assert(part1 == 16106);

    auto part2 = Solver::Solve(salt, 2016);
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 22423);

    return 0;
}
