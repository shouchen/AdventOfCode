#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cassert>

struct GateInputs
{
    std::string op1, op2, operation; // operation "" means use the literal instead
    int value; // TODO: use value = -1 for TBD (instead of wiping the operation)
};

std::map<std::string, GateInputs> output; // maps outputs to the operation that generates their value

int get_value(const std::string &label)
{
    bool test = output.find(label) != output.end();
    assert(test);

    auto &curr = output[label];
    if (curr.operation == "")
    {
        assert(curr.value == 0 || curr.value == 1);
        return curr.value;
    }

    auto retval = 0;
    if (curr.operation == "AND")
        retval = get_value(curr.op1) & get_value(curr.op2);
    else if (curr.operation == "OR")
        retval = get_value(curr.op1) | get_value(curr.op2);
    else if (curr.operation == "XOR")
        retval = (get_value(curr.op1) ^ get_value(curr.op2)) & 0x1;
    else
        assert(false);

    curr.value = retval;
    curr.operation = "";
    assert(retval == 0 || retval == 1);
    return retval;
}

auto do_part1(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, label1, label2, label3, operation, arrow;
    auto colon = ':';
    auto n = 1;

    while (std::getline(file, line) && line.length())
    {
        std::istringstream iss(line);
        iss >> label1 >> n; label1.pop_back(); // colon
        output[label1] = { "", "", "", n };
    }

    while (file >> label1 >> operation >> label2 >> arrow >> label3)
        output[label3] = GateInputs{ label1, label2, operation, 0 };

    auto retval = 0LL;

    for (auto z = 45; z >=0; z--) // TODO: automate this 45
    {
        std::string zlabel{ 'z', char(z / 10 + '0'), char(z % 10 + '0') };
        //std::cout << zlabel << "  ";

        retval = (retval << 1) | get_value(zlabel);
        //std::cout << retval << std::endl;
    }

    // debug
    //for (auto &elem : output)
    //    std::cout << elem.first << ":" << get_value(elem.first) << std::endl;

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
