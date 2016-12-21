#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cassert>

class Puzzle
{
public:
    Puzzle(const std::string &filename)
    {
        std::ifstream f(filename);
        for (std::string line; getline(f, line);)
            m_steps.push_back(line);
    }

    std::string DoSteps(std::string input)
    {
        for (auto step : m_steps)
            PerformStep(step, input, false);

        return input;
    }

    std::string UndoSteps(std::string input)
    {
        for (auto curr = m_steps.rbegin(); curr != m_steps.rend(); curr++)
            PerformStep(*curr, input, true);

        return input;
    }

private:
    void PerformStep(std::string step, std::string &input, bool undo)
    {
        std::stringstream ss(step);
        std::string word, with, position, positions, letter, through, direction, steps, to, on, of;
        unsigned p1, p2, n;
        char letter1, letter2;

        if (ss >> word)
        {
            if (word == "swap")
            {
                ss >> word;
                if (word == "position")
                {
                    ss >> p1 >> with >> position >> p2;
                    SwapPositions(input, p1, p2);
                }
                else if (word == "letter")
                {
                    ss >> letter1 >> with >> letter >> letter2;
                    SwapLetters(input, letter1, letter2);
                }
            }
            else if (word == "reverse")
            {
                ss >> positions >> p1 >> through >> p2;
                Reverse(input, p1, p2);
            }
            else if (word == "rotate")
            {
                ss >> word;
                if (word == "left")
                {
                    ss >> n >> steps;
                    undo ? RotateRight(input, n) : RotateLeft(input, n);
                }
                else if (word == "right")
                {
                    ss >> n >> steps;
                    undo ? RotateLeft(input, n) : RotateRight(input, n);
                }
                else if (word == "based")
                {
                    ss >> on >> position >> of >> letter >> letter1;
                    undo ? UnrotateBasedOnLetter(input, letter1) : RotateBasedOnLetter(input, letter1);
                }
            }
            else if (word == "move")
            {
                ss >> position >> p1 >> to >> position >> p2;
                undo ? Move(input, p2, p1) : Move(input, p1, p2);
            }
        }
    }

    void SwapPositions(std::string &s, unsigned p1, unsigned p2)
    {
        std::swap(s[p1], s[p2]);
    }

    void SwapLetters(std::string &s, char letter1, char letter2)
    {
        for (auto &c : s)
            if (c == letter1)
                c = letter2;
            else if (c == letter2)
                c = letter1;
    }

    void Reverse(std::string &s, unsigned p1, unsigned p2)
    {
        std::reverse(s.begin() + p1, s.begin() + p2 + 1);
    }

    void RotateLeft(std::string &s, unsigned n)
    {
        std::rotate(s.begin(), s.begin() + n, s.end());
    }

    void RotateRight(std::string &s, unsigned n)
    {
        std::rotate(s.begin(), s.begin() + s.length() - n, s.end());
    }

    void Move(std::string &s, unsigned p1, unsigned p2)
    {
        auto temp = s[p1];
        s = s.substr(0, p1) + s.substr(p1 + 1);
        s.insert(p2, 1, temp);
    }

    void RotateBasedOnLetter(std::string &s, char letter1)
    {
        unsigned pos = s.find(letter1);
        auto times = (pos + ((pos >= 4) ? 2 : 1)) % s.length();
        RotateRight(s, times);
    }

    void UnrotateBasedOnLetter(std::string &s, char letter)
    {
        std::string input = s, rotatedBasedOnLetter;

        do
        {
            RotateLeft(s, 1);
            rotatedBasedOnLetter = s;
            RotateBasedOnLetter(rotatedBasedOnLetter, letter);
        } while (rotatedBasedOnLetter != input);
    }

    std::vector<std::string> m_steps;
};

int main()
{
    double startTime = clock();

    Puzzle puzzle("input.txt");

    auto part1 = puzzle.DoSteps("abcdefgh");
    auto part2 = puzzle.UndoSteps("fbgdceah");

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    std::cout << std::endl << "It took " << (clock() - startTime) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    assert(part1 == "gcedfahb");
    assert(part2 == "hegbdcfa");
    return 0;
}
