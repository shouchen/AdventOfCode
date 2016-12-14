#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cassert>
#include <algorithm>

std::string salt = "zpqevtbw";
//std::string salt = "abc";
const int extraTimes = 2016; // CHANGE TO 0 FOR PART 1 OR 2016 FOR PART 2


static const char hexdigit[] = "0123456789abcdef";

void GetHashText(HCRYPTPROV hProv, const void *data, const size_t dataSize, char hashed[32])
{
    HCRYPTPROV hHash = NULL;

    if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        DWORD cbHashSize = 0, dwCount = sizeof(DWORD);

        if (CryptHashData(hHash, static_cast<const BYTE *>(data), dataSize, 0) &&
            CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&cbHashSize, &dwCount, 0))
        {
            assert(cbHashSize == 16);
            BYTE buffer[16];

            if (CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<BYTE *>(&buffer[0]), &cbHashSize, 0))
            {
                char *curr = hashed;
                for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
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

std::string GetHashString(HCRYPTPROV hProv, const void *data, const size_t dataSize)
{
    char hashed[33];
    GetHashText(hProv, data, dataSize, hashed);
    hashed[32] = '\0';
    return hashed;
}

inline std::string Part2AdditionalHashing(const std::string &input, unsigned times)
{
    auto inputLength = input.length();
    assert(inputLength == 32);

    char hashed[33];
    for (int i = 0; i < 32; i++)
        hashed[i] = input[i];
    hashed[32] = '\0';

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        for (auto i = 0U; i < times; i++)
            GetHashText(hProv, &hashed[0], 32, &hashed[0]);

        CryptReleaseContext(hProv, 0);
    }

    return hashed;
}

std::string HashInputWithSuffix(const std::string &input, unsigned suffix)
{
    std::string hashed;

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        // Longest Unsigned is 10 chars, add one for null terminator
        std::vector<char> buffer(input.length() + 11);

        auto inputLength = sprintf_s(&buffer[0], buffer.capacity(), "%s", input.c_str());
        auto remainingBufferSize = buffer.capacity() - inputLength;
        auto suffixStart = &buffer[0] + inputLength;

        auto numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%u", suffix);

        hashed = GetHashString(hProv, &buffer[0], inputLength + numericSuffixSize);

        CryptReleaseContext(hProv, 0);
    }

    return hashed;
}


//std::vector<std::set<char>> quintuples;
std::vector<std::string> hashlog;

std::string &GetHash(unsigned index)
{
    if (index >= hashlog.size())
    {
        for (auto i = hashlog.size(); i <= index; i++)
        {
            auto hash = Part2AdditionalHashing(HashInputWithSuffix(salt, i), extraTimes);
            hashlog.push_back(hash);
        }
    }

    return hashlog[index];
}

int main()
{
    int hashes = 0;
    int curr = 0;
    unsigned answer = 0;

    do
    {
        auto hash = GetHash(curr);
        //std::cout << "curr = " << curr << std::endl;

        char mychar = '\0';

        for (auto i = 0U; i <= hash.length() - 3; i++)
        {
            if (hash[i] == hash[i + 1] && hash[i] == hash[i + 2])
            {
                mychar = hash[i];
                break; // only consider first triple
            }
        }

        if (mychar)
        {
            bool lookForQuintuple = true;
            for (int i = 1; i <= 1000 && lookForQuintuple; i++)
            {
                auto hash2 = GetHash(curr + i);

                for (auto j = 0U; (j <= hash2.length() - 5) && lookForQuintuple; j++)
                {
                    if (hash2[j] == mychar &&
                        hash2[j + 1] == mychar &&
                        hash2[j + 2] == mychar &&
                        hash2[j + 3] == mychar &&
                        hash2[j + 4] == mychar)
                    {
                        lookForQuintuple = false;
                        hashes++;
                        std::cout << "KEY #" << hashes << " is " << curr << std::endl;
                        if (hashes == 64)
                        {
                            answer = curr;
                            std::cout << "Answer: " << answer << std::endl;
                        }
                    }
                }
            }
        }

        curr++;
    } while (!answer);

    //assert(answer == 16106); // part 1
    //assert(answer == 22423); // part 2
    return 0;
}
