#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

struct Bot
{
    Bot(unsigned number) : number(number), lowToBot(-1), highToBot(-1), lowToOutput(-1), highToOutput(-1) {}
    unsigned number;
    std::vector<unsigned> values; // up to 2
    int lowToBot, highToBot, lowToOutput, highToOutput;
};

std::map<unsigned, Bot *> bots;
std::map<unsigned, unsigned> outputs;

int botThatCompares17And61 = -1;

void PutValueInBot(unsigned bot, unsigned value)
{
    for (auto &b : bots)
    {
        if (b.second->number == bot)
        {
            b.second->values.push_back(value);
            return;
        }
    }

    auto bb = new Bot(bot);
    bb->values.push_back(value);
    bots[bot] = std::move(bb);
}

void SetBotGives(unsigned bot, int lowToBot, int highToBot, int lowToOutput, int highToOutput)
{
    for (auto &b : bots)
    {
        if (b.second->number == bot)
        {
            if (lowToBot != -1) b.second->lowToBot = lowToBot;
            if (highToBot != -1) b.second->highToBot= highToBot;
            if (lowToOutput != -1) b.second->lowToOutput = lowToOutput;
            if (highToOutput != -1) b.second->highToOutput = highToOutput;
            return;
        }
    }

    Bot *bb = new Bot(bot);
    bb->lowToBot = lowToBot;
    bb->highToBot = highToBot;
    bb->lowToOutput = lowToOutput;
    bb->highToOutput = highToOutput;
    bots[bot] = bb;
}

bool ApplyAnInstruction()
{
    for (auto &pair : bots)
    {
        Bot *b = pair.second;
        if (b->values.size() == 2)
        {
            auto lower = std::min(b->values[0], b->values[1]);
            auto higher = std::max(b->values[0], b->values[1]);

            if (lower == 17 && higher == 61)
                botThatCompares17And61 = b->number;

            if (b->lowToBot != -1)
                PutValueInBot(b->lowToBot, lower);
            else if (b->lowToOutput != -1)
                outputs[b->lowToOutput] = lower;
            else
                assert(false);

            if (b->highToBot != -1)
                PutValueInBot(b->highToBot, higher);
            else if (b->highToOutput != -1)
                outputs[b->highToOutput] = higher;
            else
                assert(false);

            b->values.clear();
            b->lowToBot = b->highToBot = b->lowToOutput = b->highToOutput = -1;

            return true;
        }
    }

    return false;
}

void ProcessFile(const std::string &filename)
{
    std::ifstream f(filename);
    std::string word;

    while (f >> word)
    {
        int value, n1, n2, n3;
        std::string gives, goes, low, to, bot, botOrOutput1, botOrOutput2, and, high;

        if (word == "value")
        {
            f >> value >> goes >> to >> bot >> n1;
            PutValueInBot(n1, value);
        }
        else if (word == "bot")
        {
            f >> n1 >> gives >> low >> to >> botOrOutput1 >> n2 >> and
              >> high >> to >> botOrOutput2 >> n3;

            if (botOrOutput1 == "bot")
            {
                if (botOrOutput2 == "bot")
                    SetBotGives(n1, n2, n3, -1, -1);
                else
                    SetBotGives(n1, n2, -1, -1, n3);
            }
            else
            {
                if (botOrOutput2 == "bot")
                    SetBotGives(n1, -1, n3, n2, -1);
                else
                    SetBotGives(n1, -1, -1, n2, n3);
            }
        }
    }

    while (ApplyAnInstruction());
}

int main()
{
    ProcessFile("input.txt");

    auto answer1 = botThatCompares17And61;
    auto answer2 = outputs[0] * outputs[1] * outputs[2];

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == 27);
    assert(answer2 == 13727);
    return 0;
}
