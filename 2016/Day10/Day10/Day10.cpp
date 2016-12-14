#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

class IValueReceiver
{
public:
    virtual void ReceiveValue(unsigned value) = 0;
    virtual ~IValueReceiver() {}
};

class Bot : public IValueReceiver
{
public:
    Bot(unsigned number = UINT_MAX) : number(number) {}

    unsigned GetNumber() { return this->number; }
    void SetLower(IValueReceiver *lower) { this->lower = lower;  }
    void SetHigher(IValueReceiver *higher) { this->higher = higher; }
    void ReceiveValue(unsigned value) { values.push_back(value); }

    bool DoWorkIfReady(unsigned &lowerValue, unsigned &higherValue)
    {
        if (values.size() != 2) return false;

        lowerValue = std::min(values[0], values[1]);
        higherValue = std::max(values[0], values[1]);

        lower->ReceiveValue(lowerValue);
        higher->ReceiveValue(higherValue);

        values.clear();
        return true;
    }

private:
    unsigned number;
    std::vector<unsigned> values;
    IValueReceiver *lower = nullptr, *higher = nullptr;
};

class Output : public IValueReceiver
{
public:
    Output(unsigned number) : number(number) {}
    void ReceiveValue(unsigned value) { this->value = value; }
    unsigned GetValue() { return this->value;  }

private:
    unsigned number;
    unsigned value = 0;
};

class Solver
{
public:
    void Solve(
        const std::string &filename,
        unsigned lowerCompared,
        unsigned higherCompared,
        unsigned &botThatComparesThoseTwo,
        unsigned &outputs123)
    {
        ProcessFile(filename);

        unsigned bot, lower, higher;
        while (ApplyAnInstruction(bot, lower, higher))
            if (lower == lowerCompared && higher == higherCompared)
                botThatComparesThoseTwo = bot;

        outputs123 = FindOrCreateOutput(0)->GetValue() * FindOrCreateOutput(1)->GetValue() * FindOrCreateOutput(2)->GetValue();
    }

private:
    Bot *FindOrCreateBot(unsigned bot)
    {
        auto pos = bots.find(bot);
        if (pos != bots.end())
            return pos->second.get();

        bots[bot] = std::move(std::make_unique<Bot>(bot));
        return bots[bot].get();
    }

    Output *FindOrCreateOutput(unsigned output)
    {
        auto pos = outputs.find(output);
        if (pos != outputs.end())
            return pos->second.get();

        outputs[output] = std::move(std::make_unique<Output>(output));
        return outputs[output].get();
    }

    bool ApplyAnInstruction(unsigned &number, unsigned &lower, unsigned &higher)
    {
        for (auto &pair : bots)
        {
            number = pair.second->GetNumber();
            if (pair.second->DoWorkIfReady(lower, higher))
                return true;
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

                auto b = FindOrCreateBot(n1);

                if (botOrOutput1 == "bot")
                {
                    b->SetLower(FindOrCreateBot(n2));
                    b->SetHigher(
                        (botOrOutput2 == "bot")
                            ? (IValueReceiver *)FindOrCreateBot(n3)
                            : (IValueReceiver *)FindOrCreateOutput(n3));
                }
                else
                {
                    b->SetLower(FindOrCreateOutput(n2));
                    b->SetHigher(
                        (botOrOutput2 == "bot")
                            ? (IValueReceiver *)FindOrCreateBot(n3)
                            : (IValueReceiver *)FindOrCreateOutput(n3));
                }
            }
        }
    }

private:
    std::map<unsigned, std::unique_ptr<Bot>> bots;
    std::map<unsigned, std::unique_ptr<Output>> outputs;
    unsigned botThatComparesThoseTwo = INT_MAX;
};

int main()
{
    Solver solver;
    unsigned part1, part2;
    solver.Solve("input.txt", 17, 61, part1, part2);

    std::cout << "Part One: " << part1 << std::endl;
    std::cout << "Part Two: " << part2 << std::endl;

    assert(answer1 == 27);
    assert(answer2 == 13727);
    return 0;
}
