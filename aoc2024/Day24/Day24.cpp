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

int get_value(const std::string &w, WireToOperation &output_to_op, WireToValue wtv)
{
    auto it = wtv.find(w);
    if (it != wtv.end())
        return it->second;

    auto &curr = output_to_op[w];
    auto value = 0;

    if (curr.op == "AND")
        value =  get_value(curr.w1, output_to_op, wtv) & get_value(curr.w2, output_to_op, wtv);
    else if (curr.op == "OR")
        value =  get_value(curr.w1, output_to_op, wtv) | get_value(curr.w2, output_to_op, wtv);
    else if (curr.op == "XOR")
        value = (get_value(curr.w1, output_to_op, wtv) ^ get_value(curr.w2, output_to_op, wtv)) & 0x1;

    return wtv[w] = value;
}

auto get_wrong_outputs(std::vector<Operation> &operations, WireToOperations &inputs_to_op, int max_z)
{
    for (auto &[wire, ops] : inputs_to_op)
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

            if (idx == 0 || idx == max_z)
                continue;
        }

        if (is_input1 && !is_input2)
            wrong_outputs.insert(out);

        if (op == "XOR")
        {
            if (is_input1 && is_output)
                wrong_outputs.insert(out);

            if (is_input1 && (inputs_to_op[out][0].op != "AND" || inputs_to_op[out][1].op != "XOR"))
                wrong_outputs.insert(out);

            if (!is_input1 && !is_output)
                wrong_outputs.insert(out);
        }
        else if (op == "AND" && (inputs_to_op[out].size() < 1 || inputs_to_op[out][0].op != "OR"))
        {
            wrong_outputs.insert(out);
        }
        else if (op == "OR")
        {
            if (is_input1 || is_input2)
                wrong_outputs.insert(out);

            if (!inputs_to_op.contains(out) || (inputs_to_op[out].size() != 2 || inputs_to_op[out][0].op != "AND" || inputs_to_op[out][1].op != "XOR"))
                wrong_outputs.insert(out);
        }
    }

    for (const auto &op : operations)
        if (op.w1 == "x00" || op.w2 == "x00" || op.w1 == "y00" || op.w2 == "y00")
            wrong_outputs.erase(op.out);

    return wrong_outputs;
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

    // read input graph
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
    auto wrong_outputs = get_wrong_outputs(operations, inputs_to_op, max_z);

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
