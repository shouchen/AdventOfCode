// LookAndSay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

string ProcessString(const char *str)
{
    ostringstream out;

    while (*str)
    {
        auto prevChar = *str;
        auto count = 0U;

        while (*str == prevChar)
            str++, count++;

        out << count << prevChar;
    }

    return out.str();
}

void _tmain(int argc, _TCHAR* argv[])
{
    string str = "3113322113";

    for (int i = 0; i < 40; i++)
        str = ProcessString(str.c_str());

    cout << "Answer for 40x is " << str.length() << endl;

    for (int i = 0; i < 10; i++)
        str = ProcessString(str.c_str());

    cout << "Answer for 50x is " << str.length() << endl;
}
