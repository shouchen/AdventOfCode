#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cassert>
#include <algorithm>

std::string salt = "zpqevtbw";
//std::string salt = "abc";
const int times = 2016; // CHANGE TO 0 FOR PART 1 OR 2016 FOR PART 2


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

void process_file(const std::string &filename, std::string &answer1, std::string &answer2)
{
    std::ifstream f(filename);
    std::string input;

    f >> input;

    answer2 = "        ";
    auto start = 0ULL;

    for (;;)
    {
        auto hashed = GetNextHashWithFiveLeadingZeros(input, start);
        auto fifth = hashed[5], sixth = hashed[6];

        if (answer1.length() < 8)
            answer1.push_back(fifth);

        if (fifth >= '0' && fifth <= '7')
        {
            auto position = fifth - '0';
            if (answer2[position] == ' ')
            {
                answer2[position] = sixth;

                if (answer2.find(' ') == std::string::npos)
                    break;
            }
        }
    }
}

std::string Part2AdditionalHashing(const std::string &input, unsigned times)
{
    std::string hashed = input;

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        for (auto i = 0U; i < times; i++)
            hashed = GetHashText(hProv, &hashed[0], hashed.length());

        CryptReleaseContext(hProv, 0);
    }

    return hashed;
}

std::string Foo(const std::string &input, long long unsigned suffix)
{
    std::string hashed;

    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        std::vector<char> buffer(input.length() + GetLengthLongestUnsignedLongLong() + 1);

        auto inputLength = sprintf_s(&buffer[0], buffer.capacity(), "%s", input.c_str());
        auto remainingBufferSize = buffer.capacity() - inputLength;
        auto suffixStart = &buffer[0] + inputLength;

        auto numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%llu", suffix);

        hashed = GetHashText(hProv, &buffer[0], inputLength + numericSuffixSize);

        CryptReleaseContext(hProv, 0);
    }

    return hashed;
}


//std::vector<char> triples;
//std::vector<std::set<char>> quintuples;
std::vector<std::string> hashlog;

//bool CheckKey(int start)
//{
//    auto c = triples[start];
//    if (c)
//    {
//        for (int i = 1; i <= 1000; i++)
//        {
//            std::string five; five += c; five += c; five += c; five += c; five += c;
//            if (quintuples[start + i].find(c) != quintuples[start + i].end())
//                return true;
//        }
//    }
//
//    return false;
//}

std::string &GetHash(unsigned index)
{
    if (index >= hashlog.size())
    {
        for (auto i = hashlog.size(); i <= index; i++)
        {
            auto hash = Part2AdditionalHashing(Foo(salt, i), times);
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
        std::cout << "curr = " << curr << std::endl;

        char mychar = '\0';

        for (auto i = 0U; i <= hash.length() - 3; i++)
        {
            if (hash[i] == hash[i + 1] && hash[i] == hash[i + 2])
            {
                mychar = hash[i];
                break; // only consider first
            }
        }

        if (mychar)
        {
            bool lookForQuintuples = true;
            for (int i = 1; i <= 1000 && lookForQuintuples; i++)
            {
                auto hash2 = GetHash(curr + i);

                for (auto j = 0U; (j <= hash2.length() - 5) && lookForQuintuples; j++)
                {
                    if (hash2[j] == mychar &&
                        hash2[j + 1] == mychar &&
                        hash2[j + 2] == mychar &&
                        hash2[j + 3] == mychar &&
                        hash2[j + 4] == mychar)
                    {
                        lookForQuintuples = false;
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
    } while (answer);

    //assert(answer == 16106); // part 1
    //assert(answer == 22423); // part 2
    return 0;
}
