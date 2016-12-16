#include "stdafx.h"
#include <string>
#include <algorithm>
#include <cassert>

unsigned ComputeGroupSize(const std::string &s)
{
    // Round up to even length
    auto temp = s.length();
    if (temp & 0x1) temp++;

    while ((temp & 0x1) == 0)
        temp >>= 1;

    return s.length() / temp;
}

std::string FastFindChecksum(const std::string &s)
{
    auto groupSize = ComputeGroupSize(s);

    std::string cs;
    unsigned oneCount = 0;

    for (auto i = 0U; i < s.length(); i++)
    {
        if (s[i] == '1') oneCount++;

        // Emit checksum digit if time to (end of group or end of input)
        if (((i + 1) % groupSize == 0) || (i + 1) == s.length())
        {
            cs.push_back((oneCount & 0x1) ? '0' : '1');
            oneCount = 0;
        }
    }

    return cs;
}

void ApplyModifiedDragonCurve(std::string &a)
{
    a.push_back('0');

    for (int i = a.length() - 2; i >= 0; i--)
        a.push_back((a[i] == '1') ? '0' : '1');
}

std::string FillDisk(std::string &initialState, unsigned diskLength)
{
    while (initialState.length() < diskLength)
        ApplyModifiedDragonCurve(initialState);

    return initialState.substr(0, diskLength);
}

std::string BruteForceFindChecksum(const std::string &s)
{
    std::string cs;

    for (auto i = 0U; i < s.length() - 1; i += 2)
        cs.push_back((s[i] == s[i + 1]) ? '1' : '0');

    return (cs.length() & 0x1) ? cs : BruteForceFindChecksum(cs);
}

std::string BruteForceFindSolution(const std::string &s)
{
    auto fill = FillDisk(std::string("10011111011011001"), 272);
    return BruteForceFindChecksum(fill);
}

std::string FastFindSolution(const std::string &s)
{
    auto fill = FillDisk(std::string("10011111011011001"), 272);
    return FastFindChecksum(fill);
}

int main()
{
    // Unit tests
    std::string test = "1";
    ApplyModifiedDragonCurve(test);
    assert(test == "100");

    test = "0";
    ApplyModifiedDragonCurve(test);
    assert(test.compare("001") == 0);

    test = "11111";
    ApplyModifiedDragonCurve(test);
    assert(test.compare("11111000000") == 0);

    test = "111100001010";
    ApplyModifiedDragonCurve(test);
    assert(test.compare("1111000010100101011110000") == 0);

    assert(FastFindChecksum("110010110100") == "100");

    auto fill = FillDisk(std::string("10000"), 20);
    assert(fill == "10000011110010000111");
    auto cs = FastFindChecksum(fill);
    assert(cs == "01100");

    // Part One
    fill = FillDisk(std::string("10011111011011001"), 272);
    cs = FastFindChecksum(fill);
    assert(cs == "10111110010110110");

    // Part Two
    fill = FillDisk(std::string("10011111011011001"), 35651584);
    cs = FastFindChecksum(fill);
    assert(cs == "01101100001100100");

    return 0;
}
