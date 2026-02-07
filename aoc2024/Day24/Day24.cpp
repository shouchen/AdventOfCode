#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cassert>

struct Operation
{
    std::string w1, w2, out, op;
};

using WireToOperation = std::map<std::string, Operation>;
using WireToOperations = std::unordered_map<std::string, std::vector<Operation>>;
using WireToValue = std::map<std::string, int>;

inline auto is_input_wire(const std::string &w) { return w.front() == 'x' || w.front() == 'y'; }
inline auto is_output_wire(const std::string &w) { return w.front() == 'z'; }

int get_value(const std::string &w, WireToOperation &oto, WireToValue wtv)
{
    auto it = wtv.find(w);
    if (it != wtv.end())
        return it->second;

    auto &curr = oto[w];
    auto value = 0;

    // TODO: Switch from int to bool?
    if (curr.op == "AND")
        value =  get_value(curr.w1, oto, wtv) & get_value(curr.w2, oto, wtv);
    else if (curr.op == "OR")
        value =  get_value(curr.w1, oto, wtv) | get_value(curr.w2, oto, wtv);
    else if (curr.op == "XOR")
        value = (get_value(curr.w1, oto, wtv) ^ get_value(curr.w2, oto, wtv)) & 0x1;

    return wtv[w] = value;
}

auto is_wrong_output(const Operation &operation, WireToOperations &inputs_to_op, int max_z)
{
    auto &out = operation.out, &op = operation.op;
    auto is_input1 = is_input_wire(operation.w1), is_input2 = is_input_wire(operation.w2), is_output = is_output_wire(out);

    if (operation.w1 == "x00" || operation.w2 == "x00" || operation.w1 == "y00" || operation.w2 == "y00")
        return false;

    if (is_output)
    {
        auto idx = std::stoi(out.substr(1));

        if (idx == 0 || idx == max_z)
            return false;
    }

    if (is_input1 && !is_input2)
        return true;

    if (op == "XOR")
    {
        if (is_input1 == is_output)
            return true;

        if (is_input1 && (inputs_to_op[out][0].op != "AND" || inputs_to_op[out][1].op != "XOR"))
            return true;
    }
    else if (op == "AND")
    {
        if (inputs_to_op[out].size() < 1 || inputs_to_op[out][0].op != "OR")
            return true;
    }
    else if (op == "OR")
    {
        if (is_input1 || is_input2)
            return true;

        if (!inputs_to_op.contains(out) ||
            (inputs_to_op[out].size() != 2 || inputs_to_op[out][0].op != "AND" || inputs_to_op[out][1].op != "XOR"))
            return true;
    }

    return false;
}

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, w1, w2, w3, op, arrow;
    std::vector<Operation> operations;
    WireToOperation output_to_op;
    WireToOperations inputs_to_op;
    WireToValue wire_to_value;
    std::pair<long long, std::string> retval;
    auto max_z = 0;

    while (std::getline(file, line) && line.length())
    {
        auto colon = line.find(":");
        wire_to_value[line.substr(0, colon)] = std::stoi(line.substr(colon + 1));
    }

    while (file >> w1 >> op >> w2 >> arrow >> w3)
    {
        auto operation = Operation{ w1, w2, w3, op };
        operations.push_back(operation);

        inputs_to_op[w1].push_back(operation);
        inputs_to_op[w2].push_back(operation);
        output_to_op[w3] = operation;

        if (operation.out.front() == 'z')
            max_z = std::max(std::stoi(operation.out.substr(1)), max_z);
    }

    // part1
    for (auto it = output_to_op.rbegin(); it->first.front() == 'z'; it++)
        retval.first = (retval.first << 1) | get_value(it->first, output_to_op, wire_to_value);

    // part2
    for (auto &[wire, ops] : inputs_to_op)
    {
        std::sort(std::begin(ops), std::end(ops), [](const auto &op1, const auto &op2) {
            return op1.op < op2.op;
        });
    }

    std::set<std::string> wrong_outputs;
    for (const auto &op: operations)
        if (is_wrong_output(op, inputs_to_op, max_z))
            wrong_outputs.insert(op.out);

    std::ostringstream oss;
    std::string separator;

    for (const auto &elem : wrong_outputs)
    {
        oss << separator << elem;
        separator = ",";
    }

    retval.second = oss.str();
    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << "Part One: " << answer.first << std::endl;
    std::cout << "Part Two: " << answer.second << std::endl;

    assert(answer.first == 42410633905894);
    assert(answer.second == "cqm,mps,vcv,vjv,vwp,z13,z19,z25");
    return 0;
}
