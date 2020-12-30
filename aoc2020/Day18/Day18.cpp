#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <cassert>

auto operator_precedence_part1(char op)
{
    return (op == '(' || op == ')') ? 0 : 1;
}

auto operator_precedence_part2(char op)
{
    if (op == '(' || op == ')')
        return 0;

    return (op == '*') ? 1 : 2;
}

std::string postfixify(const std::string &line, int operator_precedence(char))
{
    std::stringstream ss(line);
    auto c = ' ';
    std::string postfix;
    std::stack<char> operator_stack;

    while (ss >> c)
    {
        if (isdigit(c))
        {
            postfix.push_back(c);
            postfix.push_back(' ');
        }
        else if (c == '(')
        {
            operator_stack.push(c);
        }
        else if (c == ')')
        {
            while (!operator_stack.empty() && operator_stack.top() != '(')
            {
                postfix.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.pop();
        }
        else
        {
            while (!operator_stack.empty() && operator_stack.top() != '(' &&
                   operator_precedence(c) <= operator_precedence(operator_stack.top()))
            {
                postfix.push_back(operator_stack.top());
                postfix.push_back(' ');
                operator_stack.pop();
            }
            operator_stack.push(c);
        }
    }

    while (!operator_stack.empty())
    {
        postfix.push_back(' ');
        postfix.push_back(operator_stack.top());
        operator_stack.pop();
    }

    return postfix;
}

auto evaluate(const std::string &expression)
{
    std::stack<long long> operands;

    for (auto c : expression)
    {
        if (isdigit(c))
        {
            operands.push(c - '0');
        }
        else if (c != ' ')
        {
            const auto rhs = operands.top();
            operands.pop();
            const auto lhs = operands.top();
            operands.pop();
            operands.push((c == '+') ? (lhs + rhs) : (lhs * rhs));
        }
    }

    return operands.top();
}

auto do_part(const std::string &filename, bool part2)
{
    std::ifstream file(filename);
    std::string s;
    auto retval = 0LL;

    while (std::getline(file, s))
        retval += evaluate(postfixify(s, part2 ? operator_precedence_part2 : operator_precedence_part1));

    return retval;
}

int main()
{
    auto part1 = do_part("input.txt", false);
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = do_part("input.txt", true);
    std::cout << "Part 2: " << part2 << std::endl;

    assert(part1 == 11004703763391);
    assert(part2 == 290726428573651);
    return 0;
}
