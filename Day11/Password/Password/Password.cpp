// Password.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>


void IncrementPassword(char password[8])
{
    for (auto place = &password[7]; place >= password; --place)
    {
        if (*place == 'z')
        {
            *place = 'a';
        }
        else
        {
            (*place)++;
            break;
        }
    }
}

bool IsValidPassword(char password[8])
{
    bool hasIncreasingStraight = false;
    bool hasForbiddenLetter = false;
    int numLetterPairs = 0;

    for (int i = 0; i <= 5; i++)
    {
        if ((password[i + 1] == password[i] + 1) &&
            (password[i + 2] == password[i] + 2))
        {
            hasIncreasingStraight = true;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (password[i] == 'i' || password[i] == 'o' || password[i] == 'l')
        {
            hasForbiddenLetter = true;
        }
    }

    for (int i = 0; i < 7; i++)
    {
        if (password[i] == password[i + 1])
        {
            numLetterPairs++;
            i++;
        }
    }
         
    return hasIncreasingStraight && !hasForbiddenLetter && (numLetterPairs >= 2);
}

const char *FindNextPassword(char password[8])
{
    do
    {
        IncrementPassword(password);
    } while (!IsValidPassword(password));

    return password;
}

void _tmain(int argc, _TCHAR *argv[])
{
    char password[9] { "hxbxwxba" };

    std::cout << "part 1: " << FindNextPassword(password) << std::endl;
    std::cout << "part 2: " << FindNextPassword(password) << std::endl;
}
