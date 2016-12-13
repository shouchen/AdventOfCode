#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

struct IValueReceiver
{
    virtual void ReceiveValue(unsigned value) = 0;
};

struct Bot : public IValueReceiver
{
    Bot(unsigned number) : number(number) {}
    void ReceiveValue(unsigned value) { values.push_back(value); }

    unsigned number;
    std::vector<unsigned> values;
    IValueReceiver *lower = nullptr, *higher = nullptr;
};

struct Output : public IValueReceiver
{
    Output(unsigned number) : number(number) {}
    void ReceiveValue(unsigned value) { this->value = value; }

    unsigned number;
    unsigned value = 0;
};

std::map<unsigned, Bot *> bots;
std::map<unsigned, Output *> outputs;

int botThatCompares17And61 = -1;

Bot *FindOrCreateBot(unsigned bot)
{
    Bot *b = nullptr;

    auto pos = bots.find(bot);
    if (pos == bots.end())
        bots[bot] = b = new Bot(bot);
    else
        b = pos->second;

    return b;
}

Output *FindOrCreateOutput(unsigned output)
{
    Output *o = nullptr;

    auto pos = outputs.find(output);
    if (pos == outputs.end())
        outputs[output] = o = new Output(output);
    else
        o = pos->second;

    return o;
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

            b->lower->ReceiveValue(lower);
            b->higher->ReceiveValue(higher);

            b->values.clear();
            b->lower = b->higher = nullptr;

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
            FindOrCreateBot(n1)->ReceiveValue(value);
        }
        else if (word == "bot")
        {
            f >> n1 >> gives >> low >> to >> botOrOutput1 >> n2 >> and
              >> high >> to >> botOrOutput2 >> n3;

            if (botOrOutput1 == "bot")
            {
                if (botOrOutput2 == "bot")
                {
                    auto b = FindOrCreateBot(n1);
                    b->lower = FindOrCreateBot(n2);
                    b->higher = FindOrCreateBot(n3);
                }
                else
                {
                    auto b = FindOrCreateBot(n1);
                    b->lower = FindOrCreateBot(n2);
                    b->higher = FindOrCreateOutput(n3);
                }
            }
            else
            {
                if (botOrOutput2 == "bot")
                {
                    auto b = FindOrCreateBot(n1);
                    b->lower = FindOrCreateOutput(n2);
                    b->higher = FindOrCreateBot(n3);
                }
                else
                {
                    auto b = FindOrCreateBot(n1);
                    b->lower = FindOrCreateOutput(n2);
                    b->higher = FindOrCreateOutput(n3);
                }
            }
        }
    }

    while (ApplyAnInstruction());
}

int main()
{
    ProcessFile("input.txt");

    auto answer1 = botThatCompares17And61;
    auto answer2 = FindOrCreateOutput(0)->value * FindOrCreateOutput(1)->value * FindOrCreateOutput(2)->value;

    std::cout << "Part One: " << answer1 << std::endl;
    std::cout << "Part Two: " << answer2 << std::endl;

    assert(answer1 == 27);
    assert(answer2 == 13727);
    return 0;
}
