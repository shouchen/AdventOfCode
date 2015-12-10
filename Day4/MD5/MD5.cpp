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

unsigned GetLongestUnsignedLongLong()
{
    std::ostringstream oss;
    oss << ULLONG_MAX;
    return oss.str().length();
}

void SolveForInput(const std::string &input)
{
    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        std::vector<char> buffer(input.length() + GetLongestUnsignedLongLong() + 1);

        auto inputLength = sprintf_s(&buffer[0], buffer.capacity(), "%s", input.c_str());
        auto remainingBufferSize = buffer.capacity() - inputLength;
        auto suffixStart = &buffer[0] + inputLength;

        auto foundFiveZeros = false;

        for (unsigned long long i = 0; i < ULLONG_MAX; i++)
        {
            int numericSuffixSize = sprintf_s(suffixStart, remainingBufferSize, "%llu", i);

            std::string hashed = GetHashText(hProv, &buffer[0], inputLength + numericSuffixSize);
            if (hashed[0] == '0' && hashed[1] == '0' && hashed[2] == '0' && hashed[3] == '0' && hashed[4] == '0')
            {
                if (!foundFiveZeros)
                {
                    std::cout << "five zeros: " << i << " (" << hashed << ")" << std::endl;
                    foundFiveZeros = true;
                }

                if (hashed[5] == '0')
                {
                    std::cout << "six zeros: " << i << " (" << hashed << ")" << std::endl;
                    break;
                }
            }
        }

        CryptReleaseContext(hProv, 0);
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    SolveForInput("bgvyzdsv");
}
