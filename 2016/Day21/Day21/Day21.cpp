#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cassert>

std::string input = "abcdefgh";

std::string garbled = "fbgdceah";

void SwapPositions(unsigned p1, unsigned p2)
{
    std::swap(input[p1], input[p2]);
}

void SwapLetters(char letter1, char letter2)
{
    std::vector<unsigned> p1, p2;

    for (int i = 0; i < input.length(); i++)
        if (input[i] == letter1)
            p1.push_back(i);
        else if (input[i] == letter2)
            p2.push_back(i);

    for (int i = 0; i < p1.size(); i++)
        input[p1[i]] = letter2;

    for (int i = 0; i < p2.size(); i++)
        input[p2[i]] = letter1;
}

void Reverse(unsigned p1, unsigned p2)
{
    while (p1 < p2)
        std::swap(input[p1++], input[p2--]);
}

void RotateLeft(unsigned n)
{
    while (n--)
    {
        char temp = input[0];
        for (int i = 0; i < input.length() - 1; i++)
            input[i] = input[i + 1];
        input[input.length() - 1] = temp;
    }
}

void RotateRight(unsigned n)
{
    RotateLeft(input.length() - n);
}

void Move(unsigned p1, unsigned p2)
{
    char temp = input[p1];
    input = input.substr(0, p1) + input.substr(p1 + 1);

    input.insert(p2, 1, temp);
}

void RotateBasedOn(char letter1)
{
    unsigned pos = input.find(letter1);
    RotateRight(1 + pos);

    if (pos >= 4)
        RotateRight(1);
}

void ReadInputPart1(const std::string &filename)
{
    std::ifstream f(filename);
    std::string word, with, position, positions, letter, through, direction, step, to, on, of;
    unsigned p1, p2, n;
    char letter1, letter2;

    while (f >> word)
    {
        if (word == "swap")
        {
            f >> word;

            if (word == "position")
            {
                f >> p1 >> with >> position >> p2;
                SwapPositions(p1, p2);
            }
            else if (word == "letter")
            {
                f >> letter1 >> with >> letter >> letter2;
                SwapLetters(letter1, letter2);
            }
            else
                assert(false);
        }
        else if (word == "reverse")
        {
            f >> positions >> p1 >> through >> p2;
            Reverse(p1, p2);
        }
        else if (word == "rotate")
        {
            f >> word;
            if (word == "left")
            {
                f >> n >> step;
                RotateLeft(n);
            }
            else if (word == "right")
            {
                f >> n >> step;
                RotateRight(n);
            }
            else if (word == "based")
            {
                f >> on >> position >> of >> letter >> letter1;
                RotateBasedOn(letter1);
            }
            else
                assert(false);
        }
        else if (word == "move")
        {
            f >> position >> p1 >> to >> position >> p2;
            Move(p1, p2);
        }
        else
            assert(false);
    }
}


void RotateLeftFoo(std::string &foo,unsigned n)
{
    while (n--)
    {
        char temp = foo[0];
        for (int i = 0; i < foo.length() - 1; i++)
            foo[i] = foo[i + 1];
        foo[foo.length() - 1] = temp;
    }
}

void RotateRightFoo(std::string &foo, unsigned n)
{
    RotateLeftFoo(foo, foo.length() - n);
}

std::string RotateBasedOnLetterFoo(std::string foo, char letter1)
{
    unsigned pos = foo.find(letter1);
    RotateRightFoo(foo,1 + pos);

    if (pos >= 4)
        RotateRightFoo(foo,1);

    return foo;
}

void ReadInputPart2(const std::string &filename)
{
    std::ifstream f(filename);
    std::string word, with, position, positions, letter, through, direction, step, to, on, of;
    unsigned p1, p2, n;
    char letter1, letter2;

    while (f >> word)
    {
        if (word == "swap")
        {
            f >> word;

            if (word == "position")
            {
                f >> p1 >> with >> position >> p2;
                SwapPositions(p1, p2);
            }
            else if (word == "letter")
            {
                f >> letter1 >> with >> letter >> letter2;
                SwapLetters(letter1, letter2);
            }
            else
                assert(false);
        }
        else if (word == "reverse")
        {
            f >> positions >> p1 >> through >> p2;
            Reverse(p1, p2);
        }
        else if (word == "rotate")
        {
            f >> word;
            if (word == "left")
            {
                f >> n >> step;
                RotateRight(n);
            }
            else if (word == "right")
            {
                f >> n >> step;
                RotateLeft(n);
            }
            else if (word == "based")
            {
                f >> on >> position >> of >> letter >> letter1;
                auto temp = input;
                for (;;)
                {
                    if (RotateBasedOnLetterFoo(temp, letter1) == input)
                    {
                        input = temp;
                        break;
                    }
                    
                    RotateLeftFoo(temp, 1);
                }
            }
            else
                assert(false);
        }
        else if (word == "move")
        {
            f >> position >> p1 >> to >> position >> p2;
            Move(p2, p1);
        }
        else
            assert(false);
    }
}

int main()
{
    double startTime = clock();

    //std::ifstream f("input.txt");
    //std::string s;
    //std::vector<std::string> line;
    //while (f)
    //{
    //    char buffer[500];
    //    f.getline(buffer, 500);
    //    line.push_back(buffer);
    //}

    //std::ofstream f2("input2.txt");
    //for (int i = line.size() - 1; i >= 0; i--)
    //    f2 << line[i] << std::endl;;

    //ReadInputPart1("input.txt");

    input = garbled;
    ReadInputPart2("input2.txt");

    //unsigned part2 = 0;

    //auto part1 = Scramble("abcdefgh");

    //std::cout << "Part One: " << part1 << std::endl;
    //std::cout << "Part Two: " << part2 << std::endl;

    //std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    //assert(part1 == 32259706);
    //assert(part2 == 113);
    return 0; // "gcedfahb" part1
}
// "hegbdcfa" part2

