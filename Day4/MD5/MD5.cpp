// MD5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

std::string GetHashText(HCRYPTPROV hProv, const void *data, const size_t dataSize)
{
    std::ostringstream oss;
    HCRYPTPROV hHash = NULL;
    DWORD cbHashSize = 0, dwCount = sizeof(DWORD);

    if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) &&
        CryptHashData(hHash, static_cast<const BYTE *>(data), dataSize, 0) &&
        CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&cbHashSize, &dwCount, 0))
    {
        std::vector<BYTE> buffer(cbHashSize);
        if (CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<BYTE *>(&buffer[0]), &cbHashSize, 0))
        {
            for (std::vector<BYTE>::const_iterator iter = buffer.begin(); iter != buffer.end(); ++iter)
            {
                oss.fill('0');
                oss.width(2);
                oss << std::hex << static_cast<const int>(*iter);
            }
        }
    }

    return oss.str();
}

void SolveForInput(const string &input)
{
    HCRYPTPROV hProv = NULL;
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        bool foundFiveZeros = false;

        for (unsigned long long i = 0; i < ULLONG_MAX; i++)
        {
            char buffer[100];
            int size = sprintf_s(buffer, sizeof(buffer), "%s%llu", input.c_str(), i);

            string output = GetHashText(hProv, buffer, size);
            if (output[0] == '0' && output[1] == '0' && output[2] == '0' && output[3] == '0' && output[4] == '0')
            {
                if (!foundFiveZeros)
                {
                    cout << "five zeros: " << i << " " << output << endl;
                    foundFiveZeros = true;
                }

                if (output[5] == '0')
                {
                    cout << "six zeros: " << i << " " << output << endl;
                    break;
                }
            }
        }
    }
}

void _tmain(int argc, _TCHAR *argv[])
{
    SolveForInput("bgvyzdsv");
}
