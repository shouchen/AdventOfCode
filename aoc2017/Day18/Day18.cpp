#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <queue>

std::vector<std::string> program;

struct ProcessPart1
{
    enum State { Normal, Terminated, Blocked };

    static void bind(ProcessPart1 &p1, ProcessPart1 &p2);

    ProcessPart1(unsigned instance = 0);

    long long evaluate_expression(const std::string &expr);
    void do_instruction();

    void perform_snd(const std::string &expr1, const std::string &expr2);
    void perform_set(const std::string &expr1, const std::string &expr2);
    void perform_add(const std::string &expr1, const std::string &expr2);
    void perform_mul(const std::string &expr1, const std::string &expr2);
    void perform_mod(const std::string &expr1, const std::string &expr2);
    virtual void perform_rcv(const std::string &expr1, const std::string &expr2);
    void perform_jgz(const std::string &expr1, const std::string &expr2);

    unsigned instance, curr, queue_sends;
    long long last_value_sent;
    std::map<char, long long> registers;
    std::queue<long long> myq, *otherq;
    State state;
};

struct ProcessPart2 : public ProcessPart1
{
    ProcessPart2(unsigned instance) : ProcessPart1(instance) {}
    void perform_rcv(const std::string &expr1, const std::string &expr2);
};

void ProcessPart1::bind(ProcessPart1 &a, ProcessPart1 &b)
{
    a.otherq = &b.myq;
    b.otherq = &a.myq;
}

ProcessPart1::ProcessPart1(unsigned instance) :
    instance(instance), curr(0), queue_sends(0), last_value_sent(-1), otherq(nullptr), state(Normal)
{
    registers['p'] = instance;
}

long long ProcessPart1::evaluate_expression(const std::string &expr)
{
    return isalpha(expr[0]) ? registers[expr[0]] : _atoi64(expr.c_str());
}

void ProcessPart1::perform_snd(const std::string &expr1, const std::string &expr2)
{
    last_value_sent = evaluate_expression(expr1);
    if (otherq) otherq->push(last_value_sent);
    queue_sends++;
}

void ProcessPart1::perform_set(const std::string &expr1, const std::string &expr2)
{
    registers[expr1[0]] = evaluate_expression(expr2);
}

void ProcessPart1::perform_add(const std::string &expr1, const std::string &expr2)
{
    registers[expr1[0]] += evaluate_expression(expr2);
}

void ProcessPart1::perform_mul(const std::string &expr1, const std::string &expr2)
{
    registers[expr1[0]] *= evaluate_expression(expr2);
}

void ProcessPart1::perform_mod(const std::string &expr1, const std::string &expr2)
{
    registers[expr1[0]] %= evaluate_expression(expr2);
}

void ProcessPart1::perform_rcv(const std::string &expr1, const std::string &expr2)
{
    if (evaluate_expression(expr1) == 0)
        return;

    state = Terminated;
}

void ProcessPart2::perform_rcv(const std::string &expr1, const std::string &expr2)
{
    if (myq.empty())
    {
        state = Blocked;
        curr--;
        return;
    }

    auto reg = expr1[0];
    registers[reg] = myq.front();
    myq.pop();
    state = Normal;
}

void ProcessPart1::perform_jgz(const std::string &expr1, const std::string &expr2)
{
    if (evaluate_expression(expr1) > 0)
        curr = unsigned(curr + evaluate_expression(expr2) - 1);
}

void ProcessPart1::do_instruction()
{
    std::istringstream ss(program[curr]);
    std::string instruction, op1, op2;
    ss >> instruction >> op1 >> op2;

    if (instruction == "snd")
        perform_snd(op1, op2);
    else if (instruction == "set")
        perform_set(op1, op2);
    else if (instruction == "add")
        perform_add(op1, op2);
    else if (instruction == "mul")
        perform_mul(op1, op2);
    else if (instruction == "mod")
        perform_mod(op1, op2);
    else if (instruction == "rcv")
        perform_rcv(op1, op2);
    else if (instruction == "jgz")
        perform_jgz(op1, op2);

    if (++curr >= program.size())
        state = Terminated;
}

long long do_part1()
{
    ProcessPart1 p(0);

    while (p.state == ProcessPart1::Normal)
        p.do_instruction();

    return p.last_value_sent;
}

unsigned do_part2()
{
    ProcessPart2 p0(0), p1(1);
    ProcessPart2::bind(p0, p1);

    while (p0.state == ProcessPart1::Normal || p1.state == ProcessPart1::Normal)
    {
        p0.do_instruction();
        p1.do_instruction();
    }

    return p1.queue_sends;
}

int main()
{
    std::ifstream f("input.txt");
    std::string instruction;

    while (getline(f, instruction))
        program.push_back(instruction);

    auto part1 = do_part1();
    auto part2 = do_part2();

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 9423);
    assert(part2 == 7620);
    return 0;
}
