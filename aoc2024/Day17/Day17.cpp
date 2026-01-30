#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

using Program = std::vector<int>;
using Output = std::vector<int>;

// This is not a general solution, but is based on an analysis of what the input program is
// doing. This means that it caters only to the known input data and doesn't work with the
// example test data, for example. A general solution to Part 2 is not really feasible.
// 
// For Part 2, the main thing to notice is that the program is a loop that processes three
// least-significant bits of A in each iteration, then shifts A right by those three bits until
// A is zero. Note also that since this is only three bits, we need only try 0..7 in the below
// approach (i.e., modulo 8 arithmetic).
// 
// The Part 2 solution uses recursion that finds a number "n" that produces the last digit of
// the input program. Then it left shifts n by 3 places. Next, it tries 0..7 in the reverse
// engineered program code to see what produces the last 2 digits of the program. Then, it does
// the same for the last 3 digits, etc.

auto run_program(long long n)
{
    Output output;
    auto a = n, b = 0LL, c = 0LL;

    do
    {   // The reverse-engineered input program:
        b = a & 0x7;                // 00  bst A
        b ^= 0x2;                   // 02  bxl 2
        c = a >> b;                 // 04  cdv B
        b ^= 0x3;                   // 06  bxl 3
        b ^= c;                     // 08  bxc
        output.push_back(b & 0x7);  // 10  out (B & 0x7)
        a >>= 3;                    // 12  adv 3
    } while (a);                    // 14  jnz 0

    return output;
}

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string register_, regname_colon, program_colon;
    auto comma = ',';
    auto a = 0, b = 0, c = 0, n = 0;
    Program program;

    file >> register_ >> regname_colon >> a;
    file >> register_ >> regname_colon >> b;
    file >> register_ >> regname_colon >> c;
    file >> program_colon;

    while (file >> n)
    {
        program.push_back(n);
        file >> comma;
    }

    return std::make_pair(a, program);
}

auto recur(long long n, const Program &program, long long &retval)
{
    if (run_program(n) == program)
    {
        retval = n;
        return true;
    }

    n = n << 3;
    for (auto i = 0; i < program.size(); i++)
    {
        // If the output generated so far matches the corresponding part of program, keep going.
        auto output = run_program(n);
        if (std::equal(output.begin(), output.end(), program.end() - output.size()) && recur(n, program, retval))
            return true;
        n++;
    }

    return false;
}

auto do_part1(const std::string &filename)
{
    auto a = read_data(filename).first;
    auto output = run_program(a);

    std::ostringstream oss;
    std::copy(output.begin(), output.end() - 1, std::ostream_iterator<int>(oss, ","));
    oss << output.back();
    return oss.str();
}

auto do_part2(const std::string &filename)
{
    Program program = read_data(filename).second;
    auto retval = -1LL;
    recur(0, program, retval);
    return retval;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == "3,7,1,7,2,1,0,6,3");

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == 37221334433268);

    return 0;
}
