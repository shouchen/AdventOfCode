#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

using Program = std::vector<int>;
using Output = std::vector<int>;

struct Computer
{
    int a = 0, b = 0, c = 0, ip = 0;

    auto resolve_combo(int combo)
    {
        if (combo == 4) return a;  // register A
        if (combo == 5) return b;  // register B
        if (combo == 6) return c;  // register C
        return combo;              // literal
    }

    auto do_instruction(int opcode, int operand, Output &output)
    {
        switch (opcode)
        {
        case 0: // adv combo  (right shift A)
            a = a >> resolve_combo(operand);
            break;
        case 1: // bxl literal
            b ^= operand;
            break;
        case 2: // bst combo
            b = resolve_combo(operand) & 0x7;
            break;
        case 3: // jnz literal
            if (a) ip = operand;
            break;
        case 4: // bxc ignored
            b ^= c;
            break;
        case 5: // out combo
            output.push_back(resolve_combo(operand) & 0x7);
            break;
        case 6: // bdv combo    (B <== right-shifted A)
            b = a >> resolve_combo(operand);
            break;
        case 7: // cdv combo    (C <== right-shifted A)
            c = a >> resolve_combo(operand);
            break;
        default:
            assert(false);
        }
    }

    auto run_program(const std::vector<int> &program)
    {
        Output output;

        for (ip = 0; ip < program.size(); )
        {
            auto opcode = program[ip++];
            auto operand = program[ip++];
            do_instruction(opcode, operand, output);
        }

        return output;
    }
};

auto read_data(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, register_, regname_colon, program_colon;
    auto comma = ',';
    auto a = 0, b = 0, c = 0, n = 0, ip = 0;
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

    return std::make_pair(Computer{ a, b, c }, program);
}

auto do_part1(const std::string &filename)
{
    auto data = read_data(filename);
    Computer computer = data.first;
    Program program = data.second;

    auto output = computer.run_program(program);

    std::ostringstream oss;
    for (auto i = 0; i < output.size(); i++)
        oss << (i ? "," : "") << output[i];
    return oss.str();
}

// PART 2
// This is not a general solution, but is based on an analysis of what the input program is
// doing. The main thing to notice is that it's a loop that processes three least-significant
// bits of A in each iteration, then shifts A right by those three bits until A is zero. Note
// also that since this is only three bits, we need only try 0..7 in the below approach.
// 
// This approach uses recursion that finds a number "n" that produces the last digit of the
// input program. Then it left shifts n by 3 places. Next, it tries 0..7 in the reverse
// engineered program code to see what produces the last 2 digits of the program. Then, it does
// the same for the last 3 digits, etc.

auto run_program(long long n)
{
    Output output;
    auto a = n, b = 0LL, c = 0LL;

    do
    {
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
        auto output = run_program(n);

        // If the output generated so far matches the corresponding part of program, keep going.
        if (std::equal(output.begin(), output.end(), program.end() - output.size()) && recur(n, program, retval))
            return true;
        n++;
    }

    return false;
}

auto do_part2(const std::string &filename)
{
    auto data = read_data(filename);
    Program program = data.second;

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
