#include "stdafx.h"
#include <cassert>
#include <iostream>

inline void generate_a(unsigned &x) { x = (x * 16807ULL) % 2147483647; }
inline void generate_b(unsigned &x) { x = (x * 48271ULL) % 2147483647; }

inline auto judge(unsigned a, unsigned b) { return (a & 0xffff) == (b & 0xffff); }

auto do_part1(unsigned a, unsigned b, unsigned n)
{
    auto count = 0U;

    while (n--)
    {
        generate_a(a);
        generate_b(b);

        if (judge(a, b))
            count++;
    }

    return count;
}

auto do_part2(unsigned a, unsigned b, unsigned n)
{
    auto count = 0U;

    while (n--)
    {
        do generate_a(a); while (a & 0x3);
        do generate_b(b); while (b & 0x7);

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
