#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cassert>

constexpr auto TBD = -1;

struct GateInputs
{
    std::string op1, op2, operation;
    int value = TBD;
};

std::map<std::string, GateInputs> output;

int get_value(const std::string &label)
{
    auto &curr = output[label];
    if (curr.value == TBD)
    {
        if      (curr.operation == "AND") curr.value =  get_value(curr.op1) & get_value(curr.op2);
        else if (curr.operation == "OR")  curr.value =  get_value(curr.op1) | get_value(curr.op2);
        else if (curr.operation == "XOR") curr.value = (get_value(curr.op1) ^ get_value(curr.op2)) & 0x1;
    }

    return curr.value;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, label1, label2, label3, operation, arrow;
    auto colon = ':';
    auto n = 0;
    auto retval = 0LL;

    while (std::getline(file, line) && line.length())
    {
        std::istringstream iss(line);
        iss >> label1 >> n; label1.pop_back(); // colon
        output[label1] = { "", "", "", n };
    }

    while (file >> label1 >> operation >> label2 >> arrow >> label3)
        output[label3] = GateInputs{ label1, label2, operation, TBD };

    for (auto it = output.rbegin(); it->first.front() == 'z'; it++)
        retval = (retval << 1) | get_value(it->first);

    return retval;
}

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (file >> line)
        ;

    return -1;
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 42410633905894);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    //assert(part2 == );

    return 0;
}
