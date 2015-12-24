// Circuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cassert>


std::map<std::string, std::string> wires;
std::map<std::string, unsigned short> cache;

unsigned short Evaluate(const std::string &expr)
{
    auto andPos = expr.find(" AND ");
    auto orPos = expr.find(" OR ");
    auto notPos = expr.find("NOT ");
    auto lshiftPos = expr.find(" LSHIFT ");
    auto rshiftPos = expr.find(" RSHIFT ");

    unsigned short retval = 0;

    if (andPos != -1)
        retval = Evaluate(expr.substr(0, andPos)) & Evaluate(expr.substr(andPos + strlen(" AND ")));
    else if (orPos != -1)
        retval = Evaluate(expr.substr(0, orPos)) | Evaluate(expr.substr(orPos + strlen(" OR ")));
    else if (notPos != -1)
        retval = ~Evaluate(expr.substr(notPos + strlen("NOT ")));
    else if (lshiftPos != -1)
        retval = Evaluate(expr.substr(0, lshiftPos)) << Evaluate(expr.substr(lshiftPos + strlen(" LSHIFT ")));
    else if (rshiftPos != -1)
        retval = Evaluate(expr.substr(0, rshiftPos)) >> Evaluate(expr.substr(rshiftPos + strlen(" RSHIFT ")));
    else if (isdigit(expr[0]))
        retval = atoi(expr.c_str());
    else
    {
        retval = cache.find(expr) == cache.end() ? (cache[expr] = Evaluate(wires[expr])) : cache[expr];
    }

    return retval;
}

void ReadInput(const char *filename)
{
    std::ifstream f(filename);
    for (std::string line; getline(f, line);)
    {
        auto arrow = line.find(" -> ");
        wires[line.substr(arrow + 4)] = line.substr(0, arrow);
    }
}

void DoPart1()
{
    unsigned short a = Evaluate("a");
    std::cout << "part one: " << a << std::endl;
    assert(a == 3176);
}

void DoPart2()
{
    cache.clear();
    wires["b"] = "3176";

    unsigned short a = Evaluate("a");
    std::cout << "part two: " << a << std::endl;
    assert(a == 14710);
}

void _tmain(int argc, _TCHAR *argv[])
{
    ReadInput("Input.txt");
    DoPart1();
    DoPart2();
}
