#include "stdafx.h"
#include <string>
#include <algorithm>
#include <cassert>

std::string DoFirstPart(const std::string &a)
{
    char *b = new char[a.length() + 1];
    b[a.length()] = '\0';

    // Copy
    for (auto i = 0U; i < a.length(); i++)
        b[i] = a[i];

    // Invert
    for (auto i = 0U, j = a.length() - 1; i < j; i++, j--)
        std::swap(b[i], b[j]);
    
    // Toggle
    for (auto i = 0U; i < a.length(); i++)
        b[i] = static_cast<char>((b[i] == '1') ? '0' : '1');

    auto retval = a + "0" + b;
    delete[] b;
    return retval;
}

std::string FillDisk(std::string initialState, unsigned diskLength)
{
    while (initialState.length() < diskLength)
    {
        initialState = DoFirstPart(initialState);
    }

    return initialState.substr(0, diskLength);
}

std::string FindChecksum(const std::string &s)
{
    std::string cs;

    for (auto i = 0U; i < s.length() - 1; i += 2)
        cs.push_back((s[i] == s[i + 1]) ? '1' : '0');

    return (cs.length() & 0x1) ? cs : FindChecksum(cs);
}

int main()
{
    // Unit tests
    assert(DoFirstPart("1") == "100");
    assert(DoFirstPart("0") == "001");
    assert(DoFirstPart("11111") == "11111000000");
    assert(DoFirstPart("111100001010") == "1111000010100101011110000");
    assert(FindChecksum("110010110100") == "100");

    auto fill = FillDisk("10000", 20);
    assert(fill == "10000011110010000111");
    auto cs = FindChecksum(fill);
    assert(cs == "01100");

    // Part One
    fill = FillDisk("10011111011011001", 272);
    cs = FindChecksum(fill);
    assert(cs == "10111110010110110");

    // Part Two
    fill = FillDisk("10011111011011001", 35651584);
    cs = FindChecksum(fill);
    assert(cs == "01101100001100100");

    return 0;
}
