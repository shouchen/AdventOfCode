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


std::vector<std::set<char>> triples;
std::vector<std::set<char>> quintuples;
std::vector<std::string> hashlog;

int hashes = 0;

bool CheckKey(int start)
{
    for (auto c : triples[start])
    {
        for (int i = 1; i <= 1000; i++)
        {
            std::string five; five += c; five += c; five += c; five += c; five += c;
            if (hashlog[start + i].find(five) != std::string::npos)
            //if (quintuples[start + i].find(c) != quintuples[start + i].end())
            {
                hashes++;
                std::cout << "#" << hashes << ": index " << start << " is " << hashlog[start] << " and index " << (start + i) << " is " << hashlog[start + i] << " with char " << c << std::endl;
                if (hashes == 64)
                    std::cout << std::endl;
                return true;
            }
        }
    }

    return false;
}

int main()
{
    std::string salt = "zpqevtbw";
    //std::string salt = "abc";

    int hashes = 0;
    int curr = 0;
    const int times = 0;
LOOP:
    auto hash = Foo(salt, curr);

    // Change for part1/2
    hash = Part2AdditionalHashing(hash, times);

    triples.push_back(std::set<char>());
    quintuples.push_back(std::set<char>());
    hashlog.push_back(hash);

    for (auto i = 0U; i <= hash.length() - 3; i++)
    {
        if (hash[i] == hash[i + 1] && hash[i] == hash[i + 2])
        {
            triples[curr].insert(hash[i]);
            break; // only consider first
        }
    }

    for (auto i = 0U; i <= hash.length() - 5; i++)
    {
        if (hash[i] == hash[i + 1] && hash[i] == hash[i + 2] && hash[i] == hash[i + 3] && hash[i] == hash[i + 4])
            quintuples[curr].insert(hash[i]);
    }

    if (curr > 1000)
    {
        auto temp = curr - 1000;
        if (CheckKey(temp))
        {
            //hashes++;
            //std::cout << "KEY #" << hashes << " is " << temp << std::endl;
            //if (hashes == 64)
            //    std::cout << std::endl;
        }
    }

    curr++;
    goto LOOP;

    //auto myhashcount = 0;
    //for (int i = 0; i < 23000; i++)
    //{
    //    auto hash1 = hashlog[i];
    //    for (int j = 0; j <= hashlog[i].length() - 3; j++)
    //    {
    //        if (hash1[j] == hash1[j + 1] && hash1[j] == hash1[j + 2])
    //        {
    //            for (int k = i + 1; k <= i + 1000; k++)
    //            {
    //                std::string tempstr; tempstr.push_back(hash1[j]); tempstr.push_back(hash1[j]); tempstr.push_back(hash1[j]); tempstr.push_back(hash1[j]); tempstr.push_back(hash1[j]);
    //                if (hashlog[k].find(tempstr) != std::string::npos)
    //                {
    //                    ++myhashcount;
    //                    std::cout << "NEW SOLUTION #" << myhashcount << " at " << i << " with " << hashlog[i] << " and " << hashlog[k] << std::endl;

    //                    break; // break
    //                }
    //            }

    //            j = hashlog[i].length();
    //        }
    //    }
    //}





    // 16106 for part one!

    //std::string answer1, answer2;
    //process_file("input.txt", answer1, answer2);

    //std::cout << "Part One: " << answer1 << std::endl;
    //std::cout << "Part Two: " << answer2 << std::endl;

    //assert(answer1 == "f97c354d");
    //assert(answer2 == "863dde27");
    return 0;
}
/* For real puzzle input, this gets a right solution:
#1 is 3483
#2 is 3536
#3 is 3672
#4 is 3683
#5 is 3747
#6 is 3772
#7 is 3868
#8 is 4288
#9 is 4299
#10 is 4307
#11 is 4329
#12 is 4370
#13 is 4415
#14 is 4442
#15 is 4467
#16 is 4473
#17 is 4548
#18 is 4659
#19 is 4786
#20 is 4810
#21 is 4940
#22 is 5093
#23 is 5430
#24 is 5543
#25 is 5872
#26 is 5928
#27 is 6074
#28 is 6075
#29 is 6086
#30 is 6098
#31 is 6099
#32 is 6138
#33 is 6213
#34 is 6393
#35 is 6650
#36 is 6865
#37 is 6873
#38 is 6960
#39 is 7558
#40 is 7591
#41 is 7776
#42 is 7854
#43 is 7977
#44 is 8031
#45 is 8132
#46 is 8141
#47 is 8749
#48 is 8876
#49 is 8982
#50 is 8993
#51 is 9115
#52 is 9185
#53 is 9273
#54 is 9320
#55 is 13517
#56 is 13583
#57 is 13625
#58 is 13648
#59 is 13726
#60 is 13882
#61 is 13914
#62 is 14061
#63 is 14212
#64 is 16106
*/
