#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
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

struct Operation
{
    std::string w1, w2, out, op;
};

inline auto is_input_wire(const std::string &w) { return w.front() == 'x' || w.front() == 'y'; }
inline auto is_output_wire(const std::string &w) { return w.front() == 'z'; }

auto do_part2(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line) && line.length()); // ignore top section

    std::vector<Operation> operations;
    std::unordered_map<std::string, std::vector<Operation>> map;
    std::string label1, label2, label3, op, arrow;
    auto max_z = 0;

    while (file >> label1 >> op >> label2 >> arrow >> label3)
    {
        auto operation = Operation{ label1, label2, label3, op };
        operations.push_back(operation);

        map[label1].push_back(operation);
        map[label2].push_back(operation);

        if (operation.out.front() == 'z')
        {
            const auto idx = std::stoi(operation.out.substr(1, operation.out.size() - 1));
            max_z = std::max(int(idx), max_z);
        }
    }

    for (auto &[wire, ops] : map)
    {
        std::sort(std::begin(ops), std::end(ops), [](const auto &op1, const auto &op2) {
            return op1.op < op2.op;
        });
    }

    std::set<std::string> wrong_outputs;
    for (const auto &operation : operations)
    {
        const auto &w1 = operation.w1, &w2 = operation.w2, &out = operation.out, &op = operation.op;
        const auto is_input1 = is_input_wire(w1), is_input2 = is_input_wire(w2), is_output = is_output_wire(out);

        if (is_output)
        {
            const auto idx = std::stoi(out.substr(1, out.size() - 1));
            if (idx == 0 || idx == max_z) continue;
        }

        if (is_input1 && !is_input2)
            wrong_outputs.insert(out);

        if (op == "XOR")
        {
            if (is_input1 && is_output)
                wrong_outputs.insert(out);

            if (is_input1 && (map[out][0].op != "AND" || map[out][1].op != "XOR"))
                wrong_outputs.insert(out);

            if (!is_input1 && !is_output)
                wrong_outputs.insert(out);
        }
        else if (op == "AND" && (map[out].size() < 1 || map[out][0].op != "OR"))
        {
            wrong_outputs.insert(out);
        }
        else if (op == "OR")
        {
            if (is_input1 || is_input2)
                wrong_outputs.insert(out);

            if (!map.contains(out) || (map[out].size() != 2 || map[out][0].op != "AND" || map[out][1].op != "XOR"))
                wrong_outputs.insert(out);
        }
    }

    for (const auto &op : operations)
        if (op.w1 == "x00" || op.w2 == "x00" || op.w1 == "y00" || op.w2 == "y00")
            wrong_outputs.erase(op.out);

    std::ostringstream oss;
    std::string separator;

    for (const auto &elem : wrong_outputs)
    {
        oss << separator << elem;
        separator = ",";
    }

    return oss.str();
}

int main()
{
    auto part1 = do_part1("input.txt");
    std::cout << "Part One: " << part1 << std::endl;
    assert(part1 == 42410633905894);

    auto part2 = do_part2("input.txt");
    std::cout << "Part Two: " << part2 << std::endl;
    assert(part2 == "cqm,mps,vcv,vjv,vwp,z13,z19,z25");

    return 0;
}
