// Wrapping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>

unsigned PaperNeeded(unsigned length, unsigned width, unsigned height)
{
    auto side1 = length * width;
    auto side2 = length * height;
    auto side3 = width * height;

    return 2 * (side1 + side2 + side3) + std::min({ side1, side2, side3 });
}

unsigned RibbonNeeded(unsigned length, unsigned width, unsigned height)
{
    return 2 * (length + width + height - std::max({ length, width, height })) + length * width * height;
}

void _tmain(int argc, char *argv[])
{
    unsigned length, width, height, totalPaper = 0, totalRibbon = 0;

    std::ifstream f;
    f.open("Input.txt");

    char x;
    while (f >> length >> x >> width >> x >> height)
    {
        totalPaper += PaperNeeded(length, width, height);
        totalRibbon += RibbonNeeded(length, width, height);
    }

    std::cout << "paper = " << totalPaper << std::endl;
    std::cout << "ribbon = " << totalRibbon << std::endl;
}
