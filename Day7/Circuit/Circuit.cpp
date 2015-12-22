// Circuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cassert>

using namespace std;

map<string, string> wires;


int depth;
unsigned short Evaluate(const string &expr)
{
    assert(expr.length());
//    cout << expr << " (depth " << ++depth << ")" << endl;

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
        auto val = wires.find(expr);
        assert(val != wires.end());

        retval = Evaluate(val->second);
    }

    --depth;
    return retval;
}

int _tmain(int argc, _TCHAR* argv[])
{
    ifstream f("Input.txt");
    for (string line; getline(f, line);)
    {
        auto arrow = line.find(" -> ");
        wires[line.substr(arrow + 4)] = line.substr(0, arrow);
    }

    assert(wires.size() == 339);

    unsigned short a = Evaluate("a");

    cout << a;
    return a; // 8192 is too high
}
