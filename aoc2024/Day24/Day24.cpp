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

struct GateInputs // Part 1
{
    std::string op1, op2, operation;
    int value = TBD;
};

struct Operation // Part 2
{
    std::string w1, w2, out, op;
};

int get_value(const std::string &label, std::map<std::string, GateInputs> &map) // Part 1
{
    auto &curr = map[label];
    if (curr.value == TBD)
    {
        if      (curr.operation == "AND") curr.value =  get_value(curr.op1, map) & get_value(curr.op2, map);
        else if (curr.operation == "OR")  curr.value =  get_value(curr.op1, map) | get_value(curr.op2, map);
        else if (curr.operation == "XOR") curr.value = (get_value(curr.op1, map) ^ get_value(curr.op2, map)) & 0x1;
    }

    return curr.value;
}

inline auto is_input_wire(const std::string &w) { return w.front() == 'x' || w.front() == 'y'; }
inline auto is_output_wire(const std::string &w) { return w.front() == 'z'; }

auto solve(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line, label1, label2, label3, op, arrow;
    std::vector<Operation> operations;
    std::map<std::string, GateInputs> output_to_op;
    std::unordered_map<std::string, std::vector<Operation>> inputs_to_op;
    auto n = 0, max_z = 0;
    std::pair<long long, std::string> retval;

    while (std::getline(file, line) && line.length())
    {
        std::istringstream iss(line);
        iss >> label1 >> n; label1.pop_back(); // colon
        output_to_op[label1] = { "", "", "", n };
    }

    while (file >> label1 >> op >> label2 >> arrow >> label3)
    {
        output_to_op[label3] = GateInputs{ label1, label2, op, TBD }; // part1

        auto operation = Operation{ label1, label2, label3, op };
        operations.push_back(operation);

        inputs_to_op[label1].push_back(operation);
        inputs_to_op[label2].push_back(operation);

        if (operation.out.front() == 'z')
        {
            const auto idx = std::stoi(operation.out.substr(1, operation.out.size() - 1));
            max_z = std::max(int(idx), max_z);
        }
    }

    for (auto it = output_to_op.rbegin(); it->first.front() == 'z'; it++)
        retval.first = (retval.first << 1) | get_value(it->first, output_to_op);

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
            if (idx == 0 || idx == max_z) continue;
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
