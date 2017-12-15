#include "stdafx.h"
#include <cassert>
#include <iostream>

inline unsigned long long generate_a(unsigned long long &a) { return (a * 16807) % 2147483647; }
inline unsigned long long generate_b(unsigned long long &b) { return (b * 48271) % 2147483647; }

inline bool judge(unsigned long long a, unsigned long long b) { return (a & 0xffff) == (b & 0xffff ); }

unsigned do_part1(unsigned long long a, unsigned long long b, unsigned n)
{
    auto count = 0U;

    while (n--)
    {
        a = generate_a(a);
        b = generate_b(b);

        if (judge(a, b))
            count++;
    }

    return count;
}

unsigned do_part2(unsigned long long a, unsigned long long b, unsigned n)
{
    auto count = 0U;

    while (n--)
    {
        do a = generate_a(a); while (a & 0x3);
        do b = generate_b(b); while (b & 0x7);

        if (judge(a, b))
            count++;
    }

    return count;
}

int main()
{
    auto input_test_a = 65, input_test_b = 8921;
    assert(do_part1(input_test_a, input_test_b, 5) == 1);
    assert(do_part2(input_test_a, input_test_b, 1055) == 0);
    assert(do_part2(input_test_a, input_test_b, 1056) == 1);

    auto input_a = 699, input_b = 124;
    auto part1 = do_part1(input_a, input_b, 40000000);
    auto part2 = do_part2(input_a, input_b, 5000000);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 600);
    assert(part2 == 313);
    return 0;
}
