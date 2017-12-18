#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <queue>

class Process
{
public:
    enum State { Normal, Terminated, Blocked };

    static void bind_send_receive(Process &p1, Process &p2);

    Process(std::vector<std::string> &program, unsigned id = 0);
    void do_instruction();

    inline auto get_state() { return state; }
    inline auto get_last_value_sent() { return last_value_sent; }
    inline auto get_queue_sends() { return queue_sends; }

private:
    long long evaluate_expression(const std::string &expr);

    unsigned id, curr, queue_sends;
    long long last_value_sent;
    State state;
    std::vector<std::string> &program;
    std::map<char, long long> registers;
    std::queue<long long> my_q, *other_q;
};

void Process::bind_send_receive(Process &a, Process &b)
{
    a.other_q = &b.my_q;
    b.other_q = &a.my_q;
}

Process::Process(std::vector<std::string> &program, unsigned id) :
    id(id), curr(0), queue_sends(0), last_value_sent(-1), state(Normal), program(program), other_q(nullptr)
{
    registers['p'] = id;
}

void Process::do_instruction()
{
    std::istringstream ss(program[curr]);
    std::string opcode, op1, op2;
    ss >> opcode >> op1 >> op2;

    if (opcode == "snd")
    {
        last_value_sent = evaluate_expression(op1);
        if (other_q)
            other_q->push(last_value_sent);
        queue_sends++;
    }
    else if (opcode == "set")
    {
        registers[op1[0]] = evaluate_expression(op2);
    }
    else if (opcode == "add")
    {
        registers[op1[0]] += evaluate_expression(op2);
    }
    else if (opcode == "mul")
    {
        registers[op1[0]] *= evaluate_expression(op2);
    }
    else if (opcode == "mod")
    {
        registers[op1[0]] %= evaluate_expression(op2);
    }
    else if (opcode == "rcv")
    {
        if (other_q)
        {
            if (my_q.empty())
            {
                state = Blocked;
                curr--;
            }
            else
            {
                state = Normal;
                registers[op1[0]] = my_q.front();
                my_q.pop();
            }
        }
        else
        {
            if (evaluate_expression(op1))
                state = Terminated;
        }
    }
    else if (opcode == "jgz")
    {
        if (evaluate_expression(op1) > 0)
            curr = unsigned(curr + evaluate_expression(op2) - 1);
    }

    if (++curr >= program.size())
        state = Terminated;
}

long long Process::evaluate_expression(const std::string &expr)
{
    return isalpha(expr[0]) ? registers[expr[0]] : _atoi64(expr.c_str());
}

auto read_program(const std::string &filename)
{
    std::ifstream f(filename);
    std::string instruction;
    std::vector<std::string> program;

    while (getline(f, instruction))
        program.push_back(instruction);

    return program;
}

long long do_part1(std::vector<std::string> &program)
{
    Process p(program);

    while (p.get_state() == Process::Normal)
        p.do_instruction();

    return p.get_last_value_sent();
}

unsigned do_part2(std::vector<std::string> &program)
{
    Process p0(program, 0), p1(program, 1);
    Process::bind_send_receive(p0, p1);

    while (p0.get_state() == Process::Normal || p1.get_state() == Process::Normal)
    {
        p0.do_instruction();
        p1.do_instruction();
    }

    return p1.get_queue_sends();
}

int main()
{
    auto program = read_program("input-test.txt");
    assert(do_part1(program) == 4);
    assert(do_part2(program) == 1);

    program = read_program("input.txt");
    auto part1 = do_part1(program);
    auto part2 = do_part2(program);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 9423);
    assert(part2 == 7620);
    return 0;
}
