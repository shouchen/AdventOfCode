// JsonPart2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void _tmain(int argc, _TCHAR *argv[])
{
    std::ifstream f("Input.txt");
    string line;
    f >> line;

    line = "{\"a\":1,\"b\":2,\"c\":3,\"d\":{\"e\":\"red\"},\"f\":75}";

    // Find :"red"
    auto red = 0;
    while ((red = line.find(":\"red\"")) > 0)
    {
        // Back up to prev open brace
        do --red; while (line[red] != '{');
        auto start = red;

        // Find corresponding close brace
        unsigned depth = 0;
        do red++; while (line[red] != '}');

        // Cut out everything between braces
        line = line.substr(0, start + 1) + line.substr(red);
        cout << line << endl;
    }
}
